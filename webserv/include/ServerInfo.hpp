/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:35:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 16:09:38 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"

typedef struct location_s
{
	std::string		root;
	stringVector	methods;
	std::string		redirect;
	bool			autoindex;
	std::string		tryFile;
	bool			hasCGI;
	std::string		cgiPath;
	std::string		cgiExtension;
	std::string		uploadTo;
}					location_t;

typedef std::map<std::string, location_t> locationMap;
typedef	std::pair<std::string, location_t> locationPair;

class ServerInfo
{
	private:
		std::string _serverName;
		std::string _port;
		std::string _host;
		std::string _root;
		std::string _index;
		size_t		_clientMaxBodySize;
		std::string _errorPage;
		locationMap	_locations;

		void _checkKeywords(stringMap &configs);
		std::string _CheckAndGetErrorPage(std::string const &errorPage);

		ServerInfo();

	public:
		ServerInfo(stringMap &configs);
		~ServerInfo();

		void addLocation(locationPair location);
		void printConfigs();
};
