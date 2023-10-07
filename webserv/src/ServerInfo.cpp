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
	this->_serverName = configs["server_name"];
	this->_port = configs["port"];
	this->_host = configs["host"];
	this->_root = configs["root"];
	this->_index = configs["index"];
	this->_clientMaxBodySize = atoi(configs["client_max_body_size"].c_str());
	this->_errorPage = configs["error_page"];
}
ServerInfo::~ServerInfo(){}
