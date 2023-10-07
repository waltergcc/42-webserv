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

// ---> Constructor and destructor --------------------------------------------

static void startMessage()
{
	std::cout << CLEAR << "Starting webserv..." << std::endl << std::endl;
}

static void endMessage()
{
	std::cout << GREEN << "Webserv .conf file read successfully!" << RESET << std::endl;
}

Service::Service(int ac, char **av)
{
	startMessage();

	FileChecker	input(ac, av);
	this->_servers = input.getServerConfigs();

	endMessage();
}

Service::~Service(){}
