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
	cout << CLEAR << "Starting webserv..." << endl << endl;
	this->_checkArguments(ac, av);
	cout << GREEN << "Webserv .conf file read successfully!" << RESET << endl;
}

Service::~Service(){}

void Service::_checkConfigFile(string input)
{
	FileChecker	file(input);
}

void Service::_checkArguments(int ac, char **av)
{
	if (ac > 2)
		throw runtime_error(ERR_ARG);
	else if (ac == 2)
		this->_checkConfigFile(av[1]);
	else
		this->_checkConfigFile(DEFAULT_CONF);
}