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

// ---> Public member functions ----------------------------------------------

void Service::bootServers()
{
	printInfo(BOOT_MSG, BLUE);

	this->_initAddressParameters();

	serverVector::iterator server = this->_servers.begin();
	for(; server != this->_servers.end(); server++)
	{
		if (!server->getIsDefault())
			continue;

		this->_getSocketInfo(server);
		this->_setReuseableAddress();
		this->_convertHostToAddress();
		this->_bindAddressToSocket();
		this->_setSocketListening();
		this->_addSocketToPollfds();

		printInfo(BOOTED_MSG(this->_tmp.host, this->_tmp.port), BLUE);
		
		this->_eraseTempInfo();
	}
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

void Service::_initAddressParameters()
{
	std::memset(&this->_tmp.parameters, 0, sizeof(this->_tmp.parameters));
	this->_tmp.parameters.ai_family = AF_INET;			// IPv4
	this->_tmp.parameters.ai_socktype = SOCK_STREAM;	// TCP
	this->_tmp.parameters.ai_protocol = IPPROTO_TCP;	// TCP
	this->_tmp.address = NULL;
}

void Service::_getSocketInfo(serverVector::iterator server)
{
	server->createSocket();
	this->_tmp.socket = server->getSocket();
	this->_tmp.host = server->getHost();
	this->_tmp.port = server->getPort();
}

void Service::_setReuseableAddress()
{
	int active = 1;

	if (setsockopt(this->_tmp.socket, SOL_SOCKET, SO_REUSEADDR, &active, sizeof(int)) < 0)
		throw std::runtime_error(ERR_SET_SOCKET + std::string(std::strerror(errno)));
}

void Service::_convertHostToAddress()
{
	if (getaddrinfo(this->_tmp.host.c_str(), this->_tmp.port.c_str(), &this->_tmp.parameters, &this->_tmp.address) != 0)
	{
		this->_eraseTempInfo();
		throw std::runtime_error(ERR_GET_ADDR_INFO + std::string(std::strerror(errno)));
	}
}

void Service::_bindAddressToSocket()
{
	if (this->_tmp.address)
	{
		if (bind(this->_tmp.socket, this->_tmp.address->ai_addr, this->_tmp.address->ai_addrlen) < 0)
			throw std::runtime_error(ERR_BIND_SOCKET + std::string(std::strerror(errno)));
	}
}

void Service::_setSocketListening()
{
	if (listen(this->_tmp.socket, MAX_PENDING) < 0)
		throw std::runtime_error(ERR_LISTEN_SOCKET + std::string(std::strerror(errno)));
}

void Service::_addSocketToPollfds()
{
	pollfd pollfd;

	pollfd.fd = this->_tmp.socket;	// Socket File Descriptor
	pollfd.events = POLLIN; 		// Input ready
	pollfd.revents = 0;				// Output ready
	this->_pollfds.push_back(pollfd);
}

void Service::_eraseTempInfo()
{
	freeaddrinfo(this->_tmp.address);
	this->_tmp.address = NULL;
	this->_tmp.socket = 0;
	this->_tmp.host.clear();
	this->_tmp.port.clear();
}
