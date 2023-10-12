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

bool	g_shutdown = false;

// ---> Constructor and destructor --------------------------------------------

Service::Service(int ac, char **av)
{
	printInfo(START_MSG, GREEN);

	std::signal(SIGPIPE, SIG_IGN);
	std::signal(SIGINT, signalHandler);

	FileChecker	input(ac, av);
	this->_servers = input.getServerConfigs();
	this->_defaultServers = this->_countDefaultServers();

	// for (serverVector::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
	// 	std::cout << *server << std::endl;
	
	// std::cout << "Default servers: " << this->_defaultServers << std::endl << std::endl;
}

Service::~Service(){}

// ---> Public member functions ----------------------------------------------

void Service::setup()
{
	printInfo(SETUP_MSG, BLUE);

	this->_initAddressParameters();

	serverVector::iterator server = this->_servers.begin();
	for(; server != this->_servers.end(); server++)
	{
		if (!server->getIsDefault())
			continue;

		this->_getSetupInfo(server);
		this->_setReuseableAddress();
		this->_convertHostToAddress();
		this->_bindAddressToSocket();
		this->_setSocketListening();
		this->_addSocketInPollingRequests();

		printInfo(SET_SERVER_MSG(this->_tmp.host, this->_tmp.port), BLUE);

		this->_resetInfo();
	}
}

void Service::launch()
{
	printInfo("Launching server...", BLUE);

	while (g_shutdown == false)
	{
		this->_initPollingRequests();
		this->_pollingManager();	
	}
}

// ---> Launch private auxiliars -----------------------------------------------------

void Service::_initPollingRequests()
{
	if (poll(this->_pollingRequests.data(), this->_pollingRequests.size(), POLL_TIME_OUT) < 0 && g_shutdown == false)
		throw std::runtime_error(ERR_POLL_FAIL + std::string(std::strerror(errno)));
}

void Service::_pollingManager()
{
	for (size_t i = 0; i < this->_pollingRequests.size(); i++)
	{
		this->_getLaunchInfo(i);

		if (this->_hasDataToRead())
			continue;
		if (this->_hasErrorRequest())
			continue;
		if (this->_hasHangUpRequest())
			continue;
		if (this->_hasInvalidRequest())
			continue;
		if (this->_isClientRequest())
			continue;
		if (this->_hasDataToSend())
			continue;

		this->_resetInfo();
	}
}

void Service::_getLaunchInfo(int const i)
{
	this->_tmp.id = i;
	this->_tmp.clientSocket = i - this->_defaultServers;
	this->_tmp.serverSocket = this->_pollingRequests.at(i).fd;
	this->_tmp.mode = this->_pollingRequests.at(i).revents;
	this->_tmp.lastServerSocket = this->_servers.back().getSocket();
	this->_tmp.launch = true;
}

bool Service::_hasDataToRead()
{
	if (this->_tmp.mode & POLLIN)
	{
		if (this->_isServerSocket())
			this->_acceptConnection();
		else
			this->_readData();
		return true;
	}
	return false;
}

bool Service::_isServerSocket()
{
	serverVector::iterator server = this->_servers.begin();
	for (; server != this->_servers.end(); server++)
	{
		if (server->getSocket() == this->_tmp.serverSocket)
			return true;
	}
	return false;
}

void Service::_acceptConnection()
{
	std::cout << "accept connection" << std::endl;
}

void Service::_readData()
{
	std::cout << "read data" << std::endl;
}

void Service::_closeConnection()
{
	std::cout << "close connection" << std::endl;
}

bool Service::_hasErrorRequest()
{
	if (this->_tmp.mode & POLLERR)
	{
		printInfo(POLLERR_MSG, RED);
		this->_closeConnection();
		return true;
	}
	return false;
}

bool Service::_hasHangUpRequest()
{
	if (this->_tmp.mode & POLLHUP)
	{
		printInfo(POLLHUP_MSG, RED);
		this->_closeConnection();
		return true;
	}
	return false;
}

bool Service::_hasInvalidRequest()
{
	if (this->_tmp.mode & POLLNVAL)
	{
		printInfo(POLLNVAL_MSG, RED);
		this->_closeConnection();
		return true;
	}
	return false;
}

bool Service::_isClientRequest()
{
	return (this->_tmp.serverSocket < this->_tmp.lastServerSocket);
}

bool Service::_hasDataToSend()
{
	if (this->_tmp.mode & POLLOUT)
	{
		std::cout << "send data" << std::endl;
		return true;
	}
	return false;
}

// ---> Setup private auxiliars -------------------------------------------------------

void Service::_initAddressParameters()
{
	std::memset(&this->_tmp.parameters, 0, sizeof(this->_tmp.parameters));
	this->_tmp.parameters.ai_family = AF_INET;			// IPv4
	this->_tmp.parameters.ai_socktype = SOCK_STREAM;	// TCP
	this->_tmp.parameters.ai_protocol = IPPROTO_TCP;	// TCP
	this->_tmp.address = NULL;
}

void Service::_getSetupInfo(serverVector::iterator server)
{
	server->createSocket();
	this->_tmp.serverSocket = server->getSocket();
	this->_tmp.host = server->getHost();
	this->_tmp.port = server->getPort();
	this->_tmp.launch = false;
}

void Service::_setReuseableAddress()
{
	int active = 1;

	if (setsockopt(this->_tmp.serverSocket, SOL_SOCKET, SO_REUSEADDR, &active, sizeof(int)) < 0)
	{
		this->_resetInfo();
		throw std::runtime_error(ERR_SET_SOCKET + std::string(std::strerror(errno)));
	}
}

void Service::_convertHostToAddress()
{
	if (getaddrinfo(this->_tmp.host.c_str(), this->_tmp.port.c_str(), &this->_tmp.parameters, &this->_tmp.address) != 0)
	{
		this->_resetInfo();
		throw std::runtime_error(ERR_GET_ADDR_INFO + std::string(std::strerror(errno)));
	}
}

void Service::_bindAddressToSocket()
{
	if (this->_tmp.address)
	{
		if (bind(this->_tmp.serverSocket, this->_tmp.address->ai_addr, this->_tmp.address->ai_addrlen) < 0)
		{
			this->_resetInfo();
			throw std::runtime_error(ERR_BIND_SOCKET + std::string(std::strerror(errno)));
		}
	}
}

void Service::_setSocketListening()
{
	if (listen(this->_tmp.serverSocket, MAX_PENDING) < 0)
	{
		this->_resetInfo();
		throw std::runtime_error(ERR_LISTEN_SOCKET + std::string(std::strerror(errno)));
	}
}

// ---> Common private auxiliars -------------------------------------------------------

void Service::_addSocketInPollingRequests()
{
	pollfd request;

	if (this->_tmp.launch == true)
	{
		request.fd = this->_tmp.connectionSocket;
		request.events = POLLIN | POLLOUT;
	}
	else
	{
		request.fd = this->_tmp.serverSocket;
		request.events = POLLIN;
	}
	request.revents = 0;
	this->_pollingRequests.push_back(request);
}

void Service::_resetInfo()
{
	if (this->_tmp.address)
	{
		freeaddrinfo(this->_tmp.address);
		this->_tmp.address = NULL;
	}
	std::memset(&this->_tmp.parameters, 0, sizeof(this->_tmp.parameters));
	this->_tmp.host.clear();
	this->_tmp.port.clear();
	this->_tmp.id = 0;
	this->_tmp.clientSocket = 0;
	this->_tmp.serverSocket = 0;
	this->_tmp.lastServerSocket = 0;
	this->_tmp.mode = 0;
	this->_tmp.connectionSocket = 0;
	this->_tmp.launch = false;
}

// ---> Constructor auxiliars -------------------------------------------------------

size_t Service::_countDefaultServers()
{
	serverVector::iterator server = this->_servers.begin();
	size_t count = 0;

	for (; server != this->_servers.end(); server++)
	{
		if (server->getIsDefault() == true)
			count++;
	}
	return count;
}
