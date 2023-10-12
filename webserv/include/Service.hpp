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

struct setupInfo
{
	addrinfo	parameters;
	addrinfo	*address;
	int			socket;
	std::string	host;
	std::string	port;
};

typedef std::vector<pollfd>	pollfdVector;

class Service
{
	private:
		serverVector	_servers;
		size_t			_defaultServers;
		setupInfo		_tmp;
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
		void	_addSocketInPollingRequests();
		void	_eraseTempInfo();
		
		Service();

	public:
		Service(int ac, char **av);
		~Service();

		void setup();
};
