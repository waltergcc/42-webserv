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

ServerInfo::ServerInfo(stringMap &configs)
{
	this->_checkKeywords(configs);

	this->_serverName = configs[SERVER_N];
	this->_port = configs[LISTEN];
	this->_host = configs[HOST];
	this->_root = configs[ROOT];
	this->_index = configs[INDEX];
	this->_clientMaxBodySize = atoi(configs[MAX_SIZE].c_str());
	this->_errorPage = configs[ERROR_P];
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
	std::cout << std::endl;
}
