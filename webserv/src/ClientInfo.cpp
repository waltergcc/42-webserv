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
		std::string root = this->_server.getRoot();
		std::string resource = this->_resourceTarget;
		this->_checkLocation(root, resource, 0);
	}
	catch(const std::exception& e)
	{
		printInfo(e.what(), RED);
	}

	this->_request.clear();
	this->_requestPayload.clear();
	this->_sentRequest = false;
}

// ---> _checkLocation auxiliars ---------------------------------------------

void	ClientInfo::_checkLocation(std::string &root, std::string &resource, size_t loopCount)
{
	if (loopCount > MAX_LOOP_COUNT)
		throw std::runtime_error(RS_508);
	
	this->_checkAllServerLocations(root, resource, loopCount);
	
	std::cout << "get until here at the end" << std::endl;
}

void	ClientInfo::_checkAllServerLocations(std::string &root, std::string &resource, size_t loopCount)
{
	std::cout << this->_server.getServerName() << std::endl;
	locationMap::const_iterator location;
	for (location = this->_server.getLocations().begin(); location != this->_server.getLocations().end(); location++)
	{
		std::cout << "resource: " << resource << std::endl;
		std::cout << "get here in check all server locations" << std::endl;
		if (this->_locationIsRootAndResourceNot(location->first, resource))
			continue;
		
		if (this->_resourceHasNotLocation(location->first, resource))
		{
			std::cout << "Resource hasn't location" << std::endl;
			continue;
		}
		std::cout << "Resource has location" << std::endl;

		if (!this->_methodMatches(location->second.methods))
			throw std::runtime_error(RS_405);
		std::cout << "Method match" << std::endl;

		if (this->_hasRedirection(resource, root, loopCount, location->second.redirect, location->first))
			return;
	}
}

bool	ClientInfo::_locationIsRootAndResourceNot(std::string const &location, std::string &resource)
{
	return (location == SLASH_STR && resource != SLASH_STR);
}

bool	ClientInfo::_resourceHasNotLocation(std::string const &location, std::string &resource)
{
	return (resource.find(getPathWithSlashAtEnd(location)) == std::string::npos && !isItSufix(location, resource));
}

bool	ClientInfo::_methodMatches(stringVector const &methods)
{
	stringVector::const_iterator method;
	for (method = methods.begin(); method != methods.end(); method++)
	{
		if (*method == this->_method)
			return true;
	}
	return false;
}

bool	ClientInfo::_hasRedirection(std::string &resource, std::string &root, size_t loopCount, std::string const &redirect, std::string const &location)
{
	if (redirect.length() == 0)
		return false;
	
	std::cout << "has redirection" << std::endl;
	
	size_t pos = resource.find(location);
	std::string newResource = resource;

	newResource.replace(pos, location.length(), redirect);
	this->_checkLocation(root, newResource, loopCount + 1);
	return true;
}

// ---> _checkRequest auxiliars ------------------------------------------------

void	ClientInfo::_checkRequest()
{
	std::stringstream	ss(this->_request);

	this->_requestPayload.clear();
	this->_headers.clear();
	this->_checkFirstLine(ss);
	this->_checkAndGetHeaders(ss);
	this->_checkAndGetPayload(ss);
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
		if (line.length() == 0)
			break;
		
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

void	ClientInfo::_checkAndGetPayload(std::stringstream &ss)
{
	if (this->_method == GET || this->_method == DELETE)
		return;
	else if (this->_method == POST && this->_headers.find(CONTENT_LENGTH) == this->_headers.end())
		throw std::runtime_error(RS_411);
	
	this->_contentLength = this->_getValidContentLength(this->_headers[CONTENT_LENGTH]);

	std::streampos pos = ss.tellg();
	std::stringstream binarySs(ss.str(), std::stringstream::in | std::stringstream::binary);
	binarySs.seekg(pos);
	this->_requestPayload.resize(this->_contentLength);
	binarySs.read(&this->_requestPayload[0], this->_contentLength);
}

size_t	ClientInfo::_getValidContentLength(std::string const &length)
{
	size_t tmp = std::atoi(length.c_str());

	if (length.find_first_not_of(DECIMAL) != std::string::npos)
		throw std::runtime_error(RS_409);

	if (tmp > this->_server.getClientMaxBodySize())
		throw std::runtime_error(RS_413);

	return tmp;
}

// ---> Getters and setters ---------------------------------------------------

std::string const	&ClientInfo::getRequest() const{return (this->_request);}
ServerInfo const	&ClientInfo::getServer() const{return (this->_server);}
void				ClientInfo::changeServer(ServerInfo server){this->_server = server;}
