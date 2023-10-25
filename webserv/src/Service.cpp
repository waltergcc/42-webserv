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
}

Service::~Service()
{
	for (size_t i = 0; i < this->_pollingRequests.size(); i++)
		close(this->_pollingRequests.at(i).fd);
	printInfo(END_MSG, GREEN);
}

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
	printInfo(LAUNCH_MSG, BLUE);

	while (g_shutdown == false)
	{
		this->_initPollingRequests();
		this->_pollingManager();
	}
}

void Service::printServersInfo()
{
	serverVector::iterator server = this->_servers.begin();
	for (; server != this->_servers.end(); server++)
	{
		std::cout << *server << std::endl;
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
		if (this->_hasBadRequest())
			continue;
		if (this->_isServerRequest())
			continue;
		if (this->_hasDataToSend())
			continue;

		this->_resetInfo();
	}
}

void Service::_getLaunchInfo(int const i)
{
	this->_tmp.id = i;
	this->_tmp.clientID = i - this->_defaultServers;
	this->_tmp.socket = this->_pollingRequests.at(i).fd;
	this->_tmp.mode = this->_pollingRequests.at(i).revents;
	this->_tmp.lastServerSocket = this->_servers.at(this->_defaultServers - 1).getSocket();
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
		if (server->getSocket() == this->_tmp.socket)
			return true;
	}
	return false;
}

void Service::_acceptConnection()
{
	this->_tmp.connectionSocket = accept(this->_tmp.socket, NULL, NULL);

	if (this->_tmp.connectionSocket < 0)
		throw std::runtime_error(ERR_ACCEPT_SOCKET);
	
	fcntl(this->_tmp.connectionSocket, F_SETFL, O_NONBLOCK);	// set socket to non-blocking
	this->_clients.push_back(Client(this->_servers.at(this->_tmp.id), this->_tmp.connectionSocket));

	this->_addSocketInPollingRequests();
}

void Service::_readData()
{
	char	buffer[BUFFER_SIZE] = {0};
	int		bytes = recv(this->_tmp.socket, buffer, BUFFER_SIZE, 0);

	if (bytes > 0)
		this->_clients.at(this->_tmp.clientID).appendRequest(buffer, bytes);
	else
		this->_closeConnection(CLOSE_MSG);
}

void Service::_closeConnection(std::string const &msg)
{
	close(this->_tmp.socket);
	this->_pollingRequests.erase(this->_pollingRequests.begin() + this->_tmp.id);
	this->_clients.erase(this->_clients.begin() + this->_tmp.clientID);
	printInfo(msg, RED);
}

bool Service::_hasBadRequest()
{
	if (this->_tmp.mode & POLLERR)			// POLLERR: error condition
	{
		this->_closeConnection(POLLERR_MSG);
		return true;
	}
	else if (this->_tmp.mode & POLLHUP)		// POLLHUP: hang up
	{
		this->_closeConnection(POLLHUP_MSG);
		return true;
	}
	else if (this->_tmp.mode & POLLNVAL)	// POLLNVAL: invalid request
	{
		this->_closeConnection(POLLNVAL_MSG);
		return true;
	}
	return false;
}

bool Service::_isServerRequest()
{
	return (this->_tmp.socket <= this->_tmp.lastServerSocket);
}

bool Service::_hasDataToSend()
{
	if (this->_tmp.mode & POLLOUT)
	{
		if (this->_clients.at(this->_tmp.clientID).isTimeout())
		{
			this->_closeConnection(TIMEOUT_MSG);
			return true;	
		}

		if (!this->_clients.at(this->_tmp.clientID).isReadyToSend())
			return true;
	
		this->_checkRequestedServer();
		this->_clients.at(this->_tmp.clientID).sendResponse();

		return true;
	}
	return false;
}

void Service::_checkRequestedServer()
{
	std::string	request = this->_clients.at(this->_tmp.clientID).getRequest();
	std::string	requestedServer;
	size_t 		pos;

	if ((pos = request.find(REQUEST_HOST)))
	{
		requestedServer = request.substr(pos + std::strlen(REQUEST_HOST));
		if ((pos = requestedServer.find(NEWLINE)))
			requestedServer = requestedServer.substr(0, pos);
	}
	else
		return;

	Server	defaultServer = this->_clients.at(this->_tmp.clientID).getServer();
	serverVector::iterator server = this->_servers.begin();

	for (; server != this->_servers.end(); server++)
	{
		if (requestedServer == server->getServerName() && server->getHost() == defaultServer.getHost())
			this->_clients.at(this->_tmp.clientID).changeServer(*server);
	}
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
	this->_tmp.socket = server->getSocket();
	this->_tmp.host = server->getHost();
	this->_tmp.port = server->getPort();
	this->_tmp.launch = false;
}

void Service::_setReuseableAddress()
{
	int active = 1;

	if (setsockopt(this->_tmp.socket, SOL_SOCKET, SO_REUSEADDR, &active, sizeof(int)) < 0)
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
		if (bind(this->_tmp.socket, this->_tmp.address->ai_addr, this->_tmp.address->ai_addrlen) < 0)
		{
			this->_resetInfo();
			throw std::runtime_error(ERR_BIND_SOCKET + std::string(std::strerror(errno)));
		}
	}
}

void Service::_setSocketListening()
{
	if (listen(this->_tmp.socket, MAX_PENDING) < 0)
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
		request.fd = this->_tmp.socket;
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
	this->_tmp.clientID = 0;
	this->_tmp.socket = 0;
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
