/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:29:53 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 15:45:28 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int ac, char **av)
{
	this->checkArguments(ac, av);
}

Server::~Server(){}

void Server::checkConfigFile(std::string input)
{
	std::ifstream	file(input.c_str());

	if (!file.is_open())
		throw std::runtime_error(ERR_OPEN + input);
	file.close();
}

void Server::checkArguments(int ac, char **av)
{
	if (ac > 2)
		throw std::runtime_error(ERR_ARG);
	else if (ac == 2)
		this->checkConfigFile(av[1]);
	else
		this->checkConfigFile(DEFAULT_CONF);
}
