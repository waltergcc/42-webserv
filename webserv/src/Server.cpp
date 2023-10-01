/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:29:53 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 15:34:00 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int ac, char **av)
{
	this->checkConfigFile(ac, av);
}

Server::~Server(){}

void Server::checkConfigFile(int ac, char **av)
{
	std::string input;
	
	if (ac > 2)
		throw std::runtime_error(ERR_ARG);
	else if (ac == 2)
		input = av[1];
	else
		input = DEFAULT_CONF;
		
	std::ifstream	file(input.c_str());

	if (!file.is_open())
		throw std::runtime_error(ERR_OPEN + input);
	file.close();
}
