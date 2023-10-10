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

	printInfo(END_MSG, BLUE);
}

Service::~Service(){}

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
