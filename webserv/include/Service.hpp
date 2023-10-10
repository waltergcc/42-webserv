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

class Service
{
	private:
		serverVector	_servers;
		size_t			_defaultServers;

		// Constructor auxiliar
		size_t	_countDefaultServers();

		// bootServers auxiliars
		void	_setServersAddress(addrinfo *parameters, addrinfo *itAdress);
		
		Service();

	public:
		Service(int ac, char **av);
		~Service();

		void bootServers();
};
