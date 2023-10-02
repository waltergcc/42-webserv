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

Service::Service(int ac, char **av)
{
	std::cout << CLEAR << "Starting webserv..." << std::endl << std::endl;
	this->checkArguments(ac, av);
	std::cout << GREEN << "Webserv .conf file read successfully!" << RESET << std::endl;
}

Service::~Service(){}

void Service::checkConfigFile(std::string input)
{
	FileChecker	file(input);
}

void Service::checkArguments(int ac, char **av)
{
	if (ac > 2)
		throw std::runtime_error(ERR_ARG);
	else if (ac == 2)
		this->checkConfigFile(av[1]);
	else
		this->checkConfigFile(DEFAULT_CONF);
}
