/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 15:04:15 by mwilsch           #+#    #+#             */
/*   Updated: 2023/07/14 17:25:59 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "message.hpp"

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define IMPLEMENTED_CMDS 2



/**
 * @brief Class to handle server-level operations
 * 
 */
class ServerReactor {
	private:
		int				_serverSocket;
		int				_kq;
		bool			_isShutdown;
		typedef void 	(ServerReactor::*cmd)(Message &message);
		cmd				_cmds[3];
		string			_connectionPassword;
		ClientManager	_clientManager;
		// ChannelManager	_channelManager;
	public:
		/*			Class Default Functions			*/
		
		ServerReactor();
		ServerReactor( int port, string connectionPassword );
		~ServerReactor();
		/*			Socket & Multiplexing			*/

		void	setupServerSocket( int port );
		void	setToNonBlocking(int fd);
		void	updateMoinitoring(int clientSocket, int filter, int flags);
		

		/*			EVENTS			*/
		void	acceptNewClient();
		void	recieveIncomingMessage( int clientSocket );
		//void	sendMessageToClient(int clientSocket, string message);
		 
		/*			SERVER T0 CLIENT COMMUICATION			*/  
		
	
		/*			COMMAND IMPLEMENTATION			*/
		void		execPass(Message &message);
		void		execNick(Message &message);
		// void		exceUser(Message &message);

		/*			MAIN			*/
		void	execute(Message &message);
		void	run();
		
			/*			UTILS			*/
		void	writeServerError(string functionName, string errorMessage, int errorNumber);
};


// FIXME: I'm not sure in which class these functions should be. It depends on when they need to be called.
// TODO: Think of a better way to exec parameters. Maybe a class?
	// Format // :<nick>!<user>@<host> <command> <channel> :<optionalMessage>
//void	sendAcknowledgement(int clientSocket, string nick, string user, string host, string command, string channel, string optionalMessage);
	// Format // :<server> <numericCode> <targetNick> <parameters> :<message>
//void	sendNumericReply(int clientSocket, string numericCode, string targetNick, string parameters, string message);