/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 16:47:40 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/22 02:18:30 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

bool kickParser(std::vector<string> &ChannelNames, std::vector<string> &Users, Message &msg){

    if (msg.getParams().size() < 3)
        return (false);
    string param = msg.getParams()[0];
    for (unsigned int i = 0; i < msg.getParams().size(); i++){
        param = msg.getParams()[i];
        if (param[0] == '#'){
            ChannelNames.push_back(param);
        }
        else if (!ExecuteCommands::whiteCheck(param)){
            Users.push_back(param);
        }
    }
    if (Users.size() > ChannelNames.size() + 1)
            return (false);
		if (Users.size() == ChannelNames.size() + 1) {
            msg.setTrailing(Users[Users.size() - 1]);
    }
    return (true);
}

 // Kick #channel1 #channel2 #InvalidCh user1 user2 user3
void     ExecuteCommands::kick(ServerReactor &_server, Message &msg, int clientSocket){
    const string& nickSender = _server.getClientDataFast(clientSocket).getNickname();
    std::vector<string> ChannelNames;
    std::vector<string> Users;

    
    int stat = kickParser(ChannelNames, Users, msg);
    if (!stat){
        _server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(msg.getCommand()));
        throw std::exception();
    }
		const string &nick = _server.getClientDataFast(clientSocket).getNickname();
    for (unsigned int i = 0; i < ChannelNames.size(); i++){
        if (!_server.doesChannelExist(ChannelNames[i])){
            _server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHCHANNEL(nick, ChannelNames[i]));
            continue;
        }
        ChannelData &Channel = _server.getChannelManager().getChannelByName(ChannelNames[i]);
        if (!Channel.isCLient(clientSocket)){
            _server.sendNumericReply_FixLater(clientSocket, ERR_NOTONCHANNEL(nick, ChannelNames[i]));
            continue;
        }
        if (!Channel.isOperator(clientSocket)){
            _server.sendNumericReply_FixLater(clientSocket, ERR_CHANOPRIVSNEEDED(ChannelNames[i]));
            continue;
        }
        if (Users[i].c_str() == NULL){
            _server.sendNumericReply_FixLater(clientSocket, ERR_NEEDMOREPARAMS(msg.getCommand()));
            continue;
        }
        set<int> ChannelMembers = _server.getChannelManager().getChannelByName(ChannelNames[i]).getClientSockets();
        int kickedID = _server.getClientManager().MatchNickName(ChannelMembers, Users[i]);
        if (kickedID == -1){
			_server.sendNumericReply_FixLater(clientSocket, ERR_NOSUCHNICKCHANNEL(nickSender, Users[i]));
            continue;
        }
        vector<string> params;
        params.push_back(ChannelNames[i]);
        params.push_back(Users[i]);
        informMembers(Channel.getClientSockets(), _server.createMsg(_server.getClientDataFast(clientSocket), "KICK", params, msg.getTrailing()));
        Channel.removeClient(kickedID);
        if (Channel.isOperator(kickedID))
            Channel.removeOperator(kickedID);
    }
}

// #include "ExecuteCommands.hpp"

// bool kickParser(std::vector<string> &ChannelNames, std::vector<string> &Users, Message &ProcessMessage)
// {
//     string param = ProcessMessage.getParams()[0];
//     if (ProcessMessage.getParams().size() < 1)
//         return (false);
//     for (unsigned int i = 0; i < ProcessMessage.getParams().size(); i++)
//     {
//         param = ProcessMessage.getParams()[i];
//         if (param[0] == '#')
//         {
//             ChannelNames.push_back(param);
//         }
//         else if (!ExecuteCommands::whiteCheck(param))
//         {
//             Users.push_back(param);
//         }
//     }
//     if (Users.size() > ChannelNames.size())
//         return (false);
//     for (unsigned int i = Users.size(); i < ChannelNames.size(); i++)
//     {
//         Users.push_back("");
//     }
//     return (true);
// }

// /*
//     Search for the channels inside the channels countainer:
//         + channel doesn't exist: Generate a numeric reply of ERR and continue.
//         + channel exist search for the componenet user
//             + if the user is present then: kick his ass from the channel.
//                 + Check the trailing if it's present then inform all the members: by the kick reason
//                 + if the trailing is not present then JUST INFORM without specifyng the reason
//             + if the user doesn't exist then generate a specific numeric reply and continue.
// */

// // Kick #channel1 #channel2 #InvalidCh a user1 user2 user3
// void ExecuteCommands::kick(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket)
// {
//     std::vector<string> ChannelNames;
//     std::vector<string> Users;

//     int stat = kickParser(ChannelNames, Users, ProcessMessage);
//     if (!stat)
//     {
//         string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
//         send(clientSocket, Err.c_str(), Err.size(), 0);
//         throw std::exception();
//     }
//     else
//     {
//         for (unsigned int i = 0; i < ChannelNames.size(); i++)
//         {
//             if (_serverReactor.getChannelManager().channelExistence(ChannelNames[i]) == true)
//             {
//                 if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).isCLient(clientSocket))
//                 {
//                     if (_serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).isOperator(clientSocket))
//                     {
//                         if (Users[i].c_str() != NULL)
//                         {
//                             set<int> ChannelMembers = _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).getClientSockets();
//                             int kickedID = _serverReactor.getClientManager().MatchNickName(ChannelMembers, Users[i]);
//                             if (kickedID != -1)
//                                 _serverReactor.getChannelManager().getChannelByName(ChannelNames[i]).removeClient(kickedID);
//                             else
//                             {
//                                 string Err = ERR_USERNOTINCHANNEL(Users[i], ChannelNames[i]);
//                                 send(clientSocket, Err.c_str(), Err.size(), 0);
//                             }
//                             // NUMERIC REPLY TO INFORM ALL THE CHANNEL MEMBER. "COMMENT"
//                         }
//                         else
//                         {
//                             string Err = ERR_NEEDMOREPARAMS(ProcessMessage.getCommand());
//                             send(clientSocket, Err.c_str(), Err.size(), 0);
//                         }
//                     }
//                     else
//                     {
//                         string Err = ERR_CHANOPRIVSNEEDED(ChannelNames[i]);
//                         send(clientSocket, Err.c_str(), Err.size(), 0);
//                         throw std::exception();
//                     }
//                 }
//                 else
//                 {
//                     string Err = ERR_NOTONCHANNEL(ChannelNames[i]);
//                     send(clientSocket, Err.c_str(), Err.size(), 0);
//                     throw std::exception();
//                 }
//             }
//             else
//             {
//                 string Err = ERR_NOSUCHCHANNEL(ChannelNames[i]);
//                 send(clientSocket, Err.c_str(), Err.size(), 0);
//             }
//         }
//     }
// }