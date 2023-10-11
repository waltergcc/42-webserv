/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:29:53 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 16:12:41 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerInfo.hpp"

// ---> Constructor and destructor --------------------------------------------

ServerInfo::ServerInfo(stringMap &configs, std::vector<ServerInfo> const &servers)
{
	this->_checkKeywords(configs);

	this->_serverName = this->_getValidName(servers, configs[SERVER_N]);
	this->_host = configs[HOST];
	this->_root = getPathWithSlashAtEnd(configs[ROOT]);
	this->_index = this->_checkAndGetPage(configs[INDEX]);
	this->_errorPage = this->_checkAndGetPage(configs[ERROR_P]);
	this->_port = this->_getValidPort(configs[LISTEN]);
	this->_clientMaxBodySize = this->_getConvertedMaxSize(configs[MAX_SIZE]);
	this->_errorResponse = this->_generateErrorResponse();
	this->_isDefault = this->_checkDefaultServer(servers);
	this->_socket = 0;
}
ServerInfo::~ServerInfo()
{
	if (this->_socket != 0)
		close(this->_socket);
}

// ---> Private functions -----------------------------------------------------

void ServerInfo::_checkKeywords(stringMap &configs)
{
	std::string const mustHave[] = {SERVER_N, LISTEN, HOST, ROOT, INDEX, MAX_SIZE, ERROR_P};
	std::string const forbidden[] = {ALLOW_M, AUTOID, CGI_E, CGI_P, TRY, UPLOAD};

	for (int i = 0; i < 7; i++)
	{
		if (configs.find(mustHave[i]) == configs.end())
			throw std::runtime_error(ERR_KEYWORD_MISSING(mustHave[i]));
	}

	for (int i = 0; i < 6; i++)
	{
		if (configs.find(forbidden[i]) != configs.end())
			throw std::runtime_error(ERR_FORBIDDEN_KEYWORD(forbidden[i]));
	}
}

std::string ServerInfo::_getValidName(std::vector<ServerInfo> const &servers, std::string const &name)
{
	std::vector<ServerInfo>::const_iterator previous = servers.begin();
	for (; previous != servers.end(); previous++)
	{
		if (previous->_serverName == name)
			throw std::runtime_error(ERR_DUPLICATE_NAME(name));
	}
	return name;
}

std::string ServerInfo::_getValidPort(std::string const &port)
{
	int tmp = atoi(port.c_str());

	if (port.find_first_not_of(DECIMAL) != std::string::npos || tmp < 0 || tmp > MAX_PORT)
		throw std::runtime_error(ERR_PORT_INPUT(port));
	return (port);
}

size_t ServerInfo::_getConvertedMaxSize(std::string const &size)
{
	std::string	c;
	std::string	sub;
	double		tmp;

	if (!isdigit(size[size.length() - 1]))
	{
		sub = size.substr(0, size.length() - 1);
		c = size.substr(size.length() - 1);
	}

	if ((c.empty() && size.find_first_not_of(DECIMAL) != std::string::npos)
		|| c.find_first_not_of(SUFIX_BYTES) != std::string::npos
		|| sub.find_first_not_of(DECIMAL) != std::string::npos)
		throw std::runtime_error(ERR_MAX_SIZE_INPUT(size));
	
	if (c.empty())
		tmp = std::atof(size.c_str());
	else if (c == "b" || c == "B")
		tmp = std::atof(sub.c_str());
	else if (c == "k" || c == "K")
		tmp = std::atof(sub.c_str()) * 1024;
	else if (c == "m" || c == "M")
		tmp = std::atof(sub.c_str()) * 1024 * 1024;
	else if (c == "g" || c == "G")
		tmp = std::atof(sub.c_str()) * 1024 * 1024 * 1024;

	if (tmp > MAX_SIZE_LIMIT)
		throw std::runtime_error(ERR_MAX_SIZE_RANGE(size));

	return static_cast<size_t>(tmp);
}

std::string ServerInfo::_checkAndGetPage(std::string const &page)
{
	if (!hasThisExtension(page, EXT_HTML))
		throw std::runtime_error(ERR_PAGE_EXT(page));

	std::string path = this->_root + page;

	if (!isReadbleFile(path))
		throw std::runtime_error(ERR_PAGE_FIND(page));
	return (page);
}

std::string ServerInfo::_generateErrorResponse()
{
	std::string path = this->_root + this->_errorPage;
	std::string response = 
		"HTTP/1.1 404 Not Found\n"
		"Date: " + getTimeStamp() + "\n" +
		"Server: Webserv/1.0.0 (Linux)\n" +
		"Content-Type: " + getFileType(path) + "\n" +
		"Content-Length: " + intToString(getFileContent(path).length()) + "\n\n";

	return (response + getFileContent(path));
}

bool ServerInfo::_checkDefaultServer(std::vector<ServerInfo> const &servers)
{
	std::vector<ServerInfo>::const_iterator previous = servers.begin();
	for (; previous != servers.end(); previous++)
	{
		if (previous->_host == this->_host && previous->_port == this->_port)
			return false;
	}
	return true;
}

// ---> Public functions ------------------------------------------------------

void ServerInfo::addLocation(locationPair location)
{
	this->_locations.insert(location);
}

void ServerInfo::createSocket()
{
	if (!this->_socket)
	{
		this->_socket = socket(AF_INET, SOCK_STREAM, 0);

		if (this->_socket < 0)
			throw std::runtime_error(ERR_SOCKET(this->_serverName));
	}
	else
		std::cout << "Socket already created" << std::endl;
}

void ServerInfo::printConfigs()
{
	std::cout << "ServerName: " << this->_serverName << std::endl;
	std::cout << "Port: " << this->_port << std::endl;
	std::cout << "Host: " << this->_host << std::endl;
	std::cout << "Root: " << this->_root << std::endl;
	std::cout << "Index: " << this->_index << std::endl;
	std::cout << "ClientMaxBodySize: " << this->_clientMaxBodySize << std::endl;
	std::cout << "ErrorPage: " << this->_errorPage << std::endl;
	std::cout << "IsDefault: " << this->_isDefault << std::endl;
	
	for (locationMap::iterator it = this->_locations.begin(); it != this->_locations.end(); it++)
	{
		std::cout << "location: " << it->first << std::endl;
		std::cout << "	Root: " << it->second.root << std::endl;
		std::cout << "	Methods: ";

		for (stringVector::iterator it2 = it->second.methods.begin(); it2 != it->second.methods.end(); it2++)
			std::cout << *it2 << " ";
		std::cout << std::endl;
		std::cout << "	Redirect: " << it->second.redirect << std::endl;
		std::cout << "	Autoindex: " << it->second.autoindex << std::endl;
		std::cout << "	TryFile: " << it->second.tryFile << std::endl;
		std::cout << "	HasCGI: " << it->second.hasCGI << std::endl;
		std::cout << "	CGIPath: " << it->second.cgiPath << std::endl;
		std::cout << "	CGIExtension: " << it->second.cgiExtension << std::endl;
		std::cout << "	UploadTo: " << it->second.uploadTo << std::endl;
		std::cout << std::endl;
	}
	// std::cout << "Error Reponse ↓" << std::endl << this->_errorResponse << std::endl;
	std::cout << std::endl;
}

// ---> Getters ---------------------------------------------------------------

bool ServerInfo::getIsDefault() const{return this->_isDefault;}
