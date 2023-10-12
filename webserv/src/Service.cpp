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

// ---> Static functions ------------------------------------------------------

void setCanReuseAddress(int socket, int active)
{
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &active, sizeof(int)) < 0)
		throw std::runtime_error(ERR_SET_SOCKET + std::string(std::strerror(errno)));
}

void convertHostToAddress(addrinfo *address, addrinfo *parameters, char const *host, char const *port)
{
	if (getaddrinfo(host, port, parameters, &address) != 0)
	{
		freeaddrinfo(address);
		address = NULL;
		throw std::runtime_error(ERR_GET_ADDR_INFO + std::string(std::strerror(errno)));
	}
}

void bindAdressToSocket(int socket, addrinfo *address)
{
	if (address)
	{
		if (bind(socket, address->ai_addr, address->ai_addrlen) < 0)
			throw std::runtime_error(ERR_BIND_SOCKET + std::string(std::strerror(errno)));
	}
}

void setSocketListening(int socket)
{
	if (listen(socket, MAX_PENDING) < 0)
		throw std::runtime_error(ERR_LISTEN_SOCKET + std::string(std::strerror(errno)));
}

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

// ---> Public member functions ----------------------------------------------

void Service::bootServers()
{
	printInfo(BOOT_MSG, BLUE);
	addrinfo	*address = NULL;
	addrinfo	parameters;

	std::memset(&parameters, 0, sizeof(parameters));
	parameters.ai_family = AF_INET;			// IPv4
	parameters.ai_socktype = SOCK_STREAM;	// TCP
	parameters.ai_protocol = IPPROTO_TCP;	// TCP

	this->_setServersAddress(&parameters, address);
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

void Service::_setServersAddress(addrinfo *parameters, addrinfo *address)
{
	serverVector::iterator it = this->_servers.begin();

	for(; it != this->_servers.end(); it++)
	{
		if (!it->getIsDefault())
			continue;

		it->createSocket();
		int socket = it->getSocket();

		setCanReuseAddress(socket, 1);
		convertHostToAddress(address, parameters, it->getHost().c_str(), it->getPort().c_str());
		bindAdressToSocket(socket, address);
		setSocketListening(socket);

		freeaddrinfo(address);
		address = NULL;

		this->_addSocketToPollfds(socket);
		printInfo(BOOTED_MSG(it->getHost(), it->getPort()), BLUE);
	}
}

void Service::_addSocketToPollfds(int socket)
{
	pollfd pollfd;

	pollfd.fd = socket;		// File descriptor
	pollfd.events = POLLIN; // Input ready
	pollfd.revents = 0;		// Output ready
	this->_pollfds.push_back(pollfd);
}
