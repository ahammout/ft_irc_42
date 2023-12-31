/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 00:53:10 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/25 15:54:43 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

bool    NickNameValidation(string param)
{
    int a = 0;

    for (unsigned int i = 0; i < param.size(); i++){
        if (isalpha(param[i]))
            a ++;
    }
    if (a == 0)
        return (false);
    return (true);
}

void ExecuteCommands::nick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
{
    ClientData  &client = _serverReactor.getClientManager().getClientData(clientSocket);
    string err;

    if (ProcessMessage.getParams().empty()){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NONICKNAMEGIVEN());
        throw std::exception();
    }
    map<int, ClientData>::iterator it;
    string nickName = ProcessMessage.getParams()[0];
    if (isdigit(nickName[0]) || !NickNameValidation(nickName)) {
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_ERRONEUSNICKNAME(nickName));
        throw std::exception();
    }
    for (string::iterator it = nickName.begin(); it != nickName.end(); it++) {
        if (!isalnum(*it) && *it == '_') {
            _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_ERRONEUSNICKNAME(nickName));
            throw std::exception();
        }
    }
    string oldNick =  client.getNickname();
    if (nickName.compare(client.getNickname()) != 0)
    {
        map <int, ClientData> &clientSet = _serverReactor.getClientManager().getClientBySocket();
        for (it = clientSet.begin(); it != clientSet.end(); it++){
            if ((it->second.getNickname().compare(nickName) == 0) && (it->second.getClientSocket() != clientSocket)){
                _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NICKNAMEINUSE(nickName));
                nickName.append("_");
                it = clientSet.begin();
            }
        }
        if (nickName.compare(oldNick) != 0){
            client.setNickname(nickName);
            if (client.isRegistered())
                _serverReactor.sendMsg(clientSocket, client.getClientInfo(), "NICK", nickName);
        }
        client.setRegisteration(true, 2);
        if (client.isRegistered() && oldNick == "*"){
            _serverReactor.sendNumericReply_FixLater(clientSocket, RPL_WELCOME(nickName));
        }
    }
    // _serverReactor.printUserInformation();
}

void     ExecuteCommands::user(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) {
    ClientData  &client = _serverReactor.getClientManager().getClientData(clientSocket);
    
    if (client.isRegistered()){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_ALREADYREGISTRED());
        throw std::exception();
    }
    if (ProcessMessage.getParams().size() < 4){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), ProcessMessage.getCommand()));
        throw std::exception();
    }
    client.setUsername(ProcessMessage.getParams()[0]);
    client.setmode(atoi(ProcessMessage.getParams()[1].c_str()));
    client.setUnused(ProcessMessage.getParams()[2]);
    client.setRealname(ProcessMessage.getParams()[3]);
    client.setRegisteration(true, 1);
    if (client.isRegistered())
         _serverReactor.sendNumericReply_FixLater(clientSocket, RPL_WELCOME(client.getNickname()));
}

void ExecuteCommands::pass(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket) {
    ClientData  &client = _serverReactor.getClientDataFast(clientSocket);
    if (ProcessMessage.getParams().size() < 1 || ProcessMessage.getParams()[0].empty()){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(client.getNickname(), ProcessMessage.getCommand()));
        throw std::exception();
    }
    if (client.getRegistration()[1] && client.getRegistration()[2]){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_ALREADYREGISTRED());
        throw std::exception();
    }
    client.setRegisteration(true, 0);
    if (_serverReactor.getServerPassword().compare(ProcessMessage.getParams()[0]) != 0){
        _serverReactor.sendNumericReply_FixLater(clientSocket, ERR_PASSWDMISMATCH());
        close(clientSocket);
        throw std::exception();
    }
}
