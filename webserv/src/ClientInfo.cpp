/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:29:53 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 16:12:41 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientInfo.hpp"

// ---> Constructor and destructor --------------------------------------------

ClientInfo::ClientInfo(ServerInfo server, int socket) : _server(server), _socket(socket), _sentRequest(false){}
ClientInfo::~ClientInfo(){}

// ---> Public member functions ----------------------------------------------

void	ClientInfo::appendRequest(char const *buffer, size_t size)
{
	this->_lastRequest = std::time(NULL); 
	this->_sentRequest = false;
	this->_request.append(buffer, size);
}

bool	ClientInfo::isTimeout() const
{
	return (std::time(NULL) - this->_lastRequest > SENT_TIMEOUT);
}

bool	ClientInfo::isReadyToSend() const
{
	return (this->_sentRequest == true || this->_request.find(REQUEST_END) != std::string::npos);
}

void	ClientInfo::sendResponse()
{
	this->_sentRequest = true;
	this->_lastRequest = std::time(NULL);

	try
	{
		this->_checkRequest();
	}
	catch(const std::exception& e)
	{
		printInfo(e.what(), RED);
	}

	this->_request.clear();
	this->_sentRequest = false;
}

// ---> _checkRequest auxiliars ------------------------------------------------

void	ClientInfo::_checkRequest()
{
	std::stringstream	ss(this->_request);

	this->_checkFirstLine(ss);
	this->_checkAndGetHeaders(ss);

	// print headers
	for (stringMap::iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;


}

void	ClientInfo::_checkFirstLine(std::stringstream &ss)
{
	std::string		line;
	std::getline(ss, line);
	stringVector	parameters = getStringTokens(line, SPACE);

	if (parameters.size() != 3)
		throw std::runtime_error(RS_400);
	
	if (parameters.at(0) != GET && parameters.at(0) != DELETE && parameters.at(0) != POST)
		throw std::runtime_error(RS_501);
	this->_method = parameters.at(0);

	if (parameters.at(1).length() > MAX_URI_LENGHT)
		throw std::runtime_error(RS_414);
	
	if (parameters.at(1).find(RELATIVE_BACK) != std::string::npos || parameters.at(1) == LITERAL_BACK)
		throw std::runtime_error(RS_400);
	this->_resourceTarget = parameters.at(1);

	if (parameters.at(2) == HTTP_1_0)
		throw std::runtime_error(RS_505);
		
	if (parameters.at(2) != HTTP_1_1)
		throw std::runtime_error(RS_400);
}

void	ClientInfo::_checkAndGetHeaders(std::stringstream &ss)
{
	std::string line;
	while (std::getline(ss, line))
	{
		if (line.find(COLON) != std::string::npos)
		{
			std::string key = line.substr(0, line.find(COLON));
			std::string value = line.substr(line.find(COLON) + 1, line.find(NEWLINE));
			stringTrim(key, SPACES);
			stringTrim(value, SPACES);

			if (value.length() != 0 && key.length() != 0)
				this->_headers[key] = value;
			else
				throw std::runtime_error(RS_400);
		}
	}
}

// ---> Getters and setters ---------------------------------------------------

std::string const	&ClientInfo::getRequest() const{return (this->_request);}
ServerInfo const	&ClientInfo::getServer() const{return (this->_server);}
void				ClientInfo::changeServer(ServerInfo server){this->_server = server;}
