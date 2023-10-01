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
	this->checkArguments(ac, av);
	std::cout << "Server name: [" << this->_name << "]" << std::endl;
	std::cout << "Server host: [" << this->_host << "]" << std::endl;
	std::cout << "Server port: [" << this->_port << "]" << std::endl;
}

Service::~Service(){}

void Service::checkConfigFile(std::string input)
{
	std::ifstream	file(input.c_str());
	std::string		line;

	if (!file.is_open())
		throw std::runtime_error(ERR_OPEN + input);
	while (getline(file, line))
	{
		std::string field;	
		std::string value;
		std::stringstream ss(line);

		std::getline(ss, field, ' ');
		ss >> value;
		
		if (field == "server_name")
			this->_name = value;
		else if (field == "listen")
			this->_port = value;
		else if (field == "host")
			this->_host = value;
	}
	file.close();
	
	if (this->_name.empty() || this->_port.empty() || this->_host.empty())
		throw std::runtime_error(ERR_MANDATORY);
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
