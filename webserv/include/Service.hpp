/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:35:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 16:09:38 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"
#include "FileChecker.hpp"

struct tmpInfo
{
	addrinfo	parameters;
	addrinfo	*address;
	std::string	host;
	std::string	port;
	int			socket;
	int			clientFd;
	int			serverFd;
	int			lastServerFd;
	int			id;
	short		mode;
	bool		launch;
	int			connectionSocket;
};

typedef std::vector<pollfd>	pollfdVector;

class Service
{
	private:
		serverVector	_servers;
		size_t			_defaultServers;
		tmpInfo			_tmp;
		pollfdVector	_pollingRequests;

		// Constructor auxiliar
		size_t	_countDefaultServers();

		// setup auxiliars
		void	_initAddressParameters();
		void	_getSocketInfo(serverVector::iterator server);
		void	_setReuseableAddress();
		void	_convertHostToAddress();
		void	_bindAddressToSocket();
		void	_setSocketListening();

		// launch auxiliars
		void	_initPollingRequests();
		void	_pollingManager();
		void	_getPollingInfo(int const i);
		bool	_hasDataToRead();
		bool	_serverExists();
		bool	_hasErrorRequest();
		bool	_hasHangUpRequest();
		bool	_hasInvalidRequest();
		bool	_isClientRequest();
		bool	_hasDataToSend();
		
		// Common auxiliars
		void	_addSocketInPollingRequests();
		void	_eraseTmpInfo();

		Service();

	public:
		Service(int ac, char **av);
		~Service();

		void setup();
		void launch();
};
