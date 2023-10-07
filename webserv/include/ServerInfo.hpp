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
	std::string	root;
	std::string	redirect;
	std::string	try_file;
	std::string	uploadTo;
}				location_t;

typedef std::map<std::string, location_t> locationMap;

class ServerInfo
{
	private:
		std::string _serverName;
		std::string _port;
		std::string _host;
		std::string _root;
		std::string _index;
		size_t		_clientMaxBodySize;
		locationMap	_locations;

		ServerInfo();

	public:
		ServerInfo(stringMap &configs);
		~ServerInfo();
};
