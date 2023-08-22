/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: verdant <verdant@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 13:27:34 by mwilsch           #+#    #+#             */
/*   Updated: 2023/08/22 19:14:48 by verdant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

/*			CLASS DEFAULT FUNCTIONS			*/

Message::~Message( void ) {};

Message::Message( string rawMessage, map <string, CommandProperties> properties ) : _rawMessage(rawMessage), _properties(properties) {
	
	_isFatal = false;
	// Parse raw message
	extractCommand( );
	extractTrailing( );
	extractParams( );

	// Print extracted data
	printData();

	// Execute command
	// [...]
}

/*			GETTERS			*/

string Message::getCommand( void ) { 
	return _command; 
}

std::vector <string> Message::getParams( void ) {
	if (_params.empty())
		return (std::vector <string>());
	return _params;
}


/*			SETTERS			*/

// void Message::setResponseCode( string responseCode )  { 
// 	_isFatal = true;
// 	_responseCode = responseCode; 
// }

/*			MEMBER FUNCTIONS			*/

void Message::printData( void )
{
	std::cout << "Prefix: " << _prefix << std::endl;
	std::cout << "Command: " << _command << std::endl;
	std::cout << "Params: ";
	for (std::vector<string>::iterator it = _params.begin(); it != _params.end(); ++it)
	{
		std::cout << *it << " ";
		if (it == _params.end() - 1)
			std::cout << std::endl;
	}
	std::cout << "Trailing: " << _trailing << std::endl;
}
