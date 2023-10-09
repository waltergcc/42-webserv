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

// ---> Static functions ------------------------------------------------------

static std::string getValidPort(std::string const &port)
{
	int tmp = atoi(port.c_str());

	if (port.find_first_not_of(DECIMAL) != std::string::npos || tmp < 0 || tmp > 65535)
		throw std::runtime_error(ERR_PORT_INPUT(port));
	return (port);
}

static size_t getConvertedMaxSize(std::string const &size)
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

// ---> Constructor and destructor --------------------------------------------

ServerInfo::ServerInfo(stringMap &configs)
{
	this->_checkKeywords(configs);

	this->_serverName = configs[SERVER_N];
	this->_host = configs[HOST];
	this->_root = getPathWithSlashAtEnd(configs[ROOT]);
	this->_index = configs[INDEX];
	this->_errorPage = this->_checkAndGetErrorPage(configs[ERROR_P]);
	this->_port = getValidPort(configs[LISTEN]);
	this->_clientMaxBodySize = getConvertedMaxSize(configs[MAX_SIZE]);
	this->_errorResponse = this->_generateErrorResponse();
}
ServerInfo::~ServerInfo(){}

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

std::string ServerInfo::_checkAndGetErrorPage(std::string const &errorPage)
{
	std::string path = this->_root + errorPage;

	if (access(path.c_str(), R_OK) != 0)
		throw std::runtime_error(ERR_ERROR_PAGE);
	return (errorPage);
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


// ---> Public functions ------------------------------------------------------

void ServerInfo::addLocation(locationPair location){this->_locations.insert(location);}

void ServerInfo::printConfigs()
{
	std::cout << "ServerName: " << this->_serverName << std::endl;
	std::cout << "Port: " << this->_port << std::endl;
	std::cout << "Host: " << this->_host << std::endl;
	std::cout << "Root: " << this->_root << std::endl;
	std::cout << "Index: " << this->_index << std::endl;
	std::cout << "ClientMaxBodySize: " << this->_clientMaxBodySize << std::endl;
	std::cout << "ErrorPage: " << this->_errorPage << std::endl;
	
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
	std::cout << "Error Reponse ↓" << std::endl << this->_errorResponse << std::endl;
	std::cout << std::endl;
}
