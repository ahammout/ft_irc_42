/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:01:31 by ahammout          #+#    #+#             */
/*   Updated: 2023/10/05 13:58:53 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecuteCommands.hpp"

/*
    Command: PART
    Parameters: <channel> *( "," <channel> ) [ <Part Message> ]
*/

bool    mulipleChnnels(std::vector<string> &ChannelNames, std::vector<string> &Users, Message &ProcessMessage);

void     ExecuteCommands::part(ServerReactor &_serverReactor, Message &ProcessMessage, int clientSocket){
    cout << "Execute Part command " << endl;
    
}