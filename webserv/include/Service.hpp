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

struct bootInfo
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
		bootInfo		_tmp;
		pollfdVector	_pollfds;

		// Constructor auxiliar
		size_t	_countDefaultServers();

		// setupServers auxiliars
		void	_initAddressParameters();
		void	_getSocketInfo(serverVector::iterator server);
		void	_setReuseableAddress();
		void	_convertHostToAddress();
		void	_bindAddressToSocket();
		void	_setSocketListening();
		void	_addSocketToPollfds();
		void	_eraseTempInfo();
		
		Service();

	public:
		Service(int ac, char **av);
		~Service();

		void setupServers();
};
