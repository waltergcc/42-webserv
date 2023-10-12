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

typedef std::vector<pollfd>	pollfdVector;

class Service
{
	private:
		serverVector	_servers;
		size_t			_defaultServers;
		pollfdVector	_pollfds;

		// Constructor auxiliar
		size_t	_countDefaultServers();

		// bootServers auxiliars
		void	_setServersAddress(addrinfo *parameters, addrinfo *address);
		void	_addSocketToPollfds(int socket);
		
		Service();

	public:
		Service(int ac, char **av);
		~Service();

		void bootServers();
};
