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

struct serviceInfo
{
	addrinfo	parameters;
	addrinfo	*address;
	std::string	host;
	std::string	port;
	int			serverSocket;
	int			clientSocket;
	int			lastServerSocket;
	int			connectionSocket;
	int			id;
	short		mode;
	bool		launch;
};

typedef std::vector<pollfd>	pollfdVector;

class Service
{
	private:
		serverVector	_servers;
		size_t			_defaultServers;
		serviceInfo		_tmp;
		pollfdVector	_pollingRequests;

		// Constructor auxiliar
		size_t	_countDefaultServers();

		// setup auxiliars
		void	_initAddressParameters();
		void	_getSetupInfo(serverVector::iterator server);
		void	_setReuseableAddress();
		void	_convertHostToAddress();
		void	_bindAddressToSocket();
		void	_setSocketListening();

		// launch auxiliars
		void	_initPollingRequests();
		void	_pollingManager();
		void	_getLaunchInfo(int const i);
		bool	_hasDataToRead();
		bool	_isServerSocket();
		bool	_hasErrorRequest();
		bool	_hasHangUpRequest();
		bool	_hasInvalidRequest();
		bool	_isClientRequest();
		bool	_hasDataToSend();
		
		// Common auxiliars
		void	_addSocketInPollingRequests();
		void	_resetInfo();

		Service();

	public:
		Service(int ac, char **av);
		~Service();

		void setup();
		void launch();
};
