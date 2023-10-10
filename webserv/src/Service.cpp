/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:29:53 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 16:12:41 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Service.hpp"
#include "FileChecker.hpp"
#include "utils.hpp"

// ---> Constructor and destructor --------------------------------------------

Service::Service(int ac, char **av)
{
	printInfo(START_MSG, GREEN);

	FileChecker	input(ac, av);
	this->_servers = input.getServerConfigs();
	this->_defaultServers = this->_countDefaultServers();

	for (serverVector::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		it->printConfigs();
	
	std::cout << "Default servers: " << this->_defaultServers << std::endl << std::endl;
}

Service::~Service()
{
	printInfo(END_MSG, GREEN);
}

// ---> Private member functions ---------------------------------------------

size_t Service::_countDefaultServers()
{
	serverVector::iterator it = this->_servers.begin();
	size_t count = 0;

	for (; it != this->_servers.end(); it++)
	{
		if (it->getIsDefault() == true)
			count++;
	}
	return count;
}

void Service::_setServersAddress(addrinfo *parameters, addrinfo *itAdress)
{
	(void)parameters;
	(void)itAdress;
	std::cout << "	   --> Set server address called" << std::endl;
}

// ---> Public member functions ----------------------------------------------

void Service::bootServers()
{
	printInfo(BOOT_MSG, BLUE);
	addrinfo	*itAdress = NULL;
	addrinfo	parameters;

	std::memset(&parameters, 0, sizeof(parameters));
	parameters.ai_family = AF_INET;			// IPv4
	parameters.ai_socktype = SOCK_STREAM;	// TCP
	parameters.ai_protocol = IPPROTO_TCP;	// TCP

	this->_setServersAddress(&parameters, itAdress);
}
