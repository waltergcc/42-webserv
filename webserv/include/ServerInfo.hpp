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
#include "utils.hpp"

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
		std::string _errorResponse;
		locationMap	_locations;
		bool		_isDefault;
		int			_socket;

		void		_checkKeywords(stringMap &configs);
		std::string _getValidName(std::vector<ServerInfo> const &servers, std::string const &name);
		std::string	_getValidPort(std::string const &port);
		std::string _getValidRoot(std::string const &root);
		std::string	_checkAndGetPage(std::string const &page);
		size_t		_getConvertedMaxSize(std::string const &size);
		std::string _generateErrorResponse();
		bool		_checkDefaultServer(std::vector<ServerInfo> const &servers);

		ServerInfo();

	public:
		ServerInfo(stringMap &configs, std::vector<ServerInfo> const &servers);
		~ServerInfo();

		void addLocation(locationPair location);
		void createSocket();
		void printConfigs();

		int			getSocket() const;
		std::string const &getHost() const;
		std::string const &getPort() const;
		bool		getIsDefault() const;
};
