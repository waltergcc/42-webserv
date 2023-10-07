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

// ---> Constructor and destructor --------------------------------------------

Service::Service(int ac, char **av)
{
	std::cout << CLEAR << "Starting webserv..." << std::endl << std::endl;
	this->_checkArguments(ac, av);
	std::cout << GREEN << "Webserv .conf file read successfully!" << RESET << std::endl;
}

Service::~Service(){}

// ---> Private Methods : Parse Input ------------------------------------------

void Service::_parseConfigFile(std::string input)
{
	FileChecker	file(input);
	
	Token		token = file.getNextToken();
	while (token.type != END)
	{
		if (token.value == "server")
			this->_parseServerBlock(file, token, false, 0);

		token = file.getNextToken();
	}
}

void Service::_parseServerBlock(FileChecker &file, Token &token, bool location, int bracket)
{
	std::string previous;

	while (true)
	{
		previous = token.value;

		if (token.value == "server")
			token = file.getNextToken();
		
		if (previous == "server" && token.type != OPEN_BRACKET)
			throw std::runtime_error(ERR_SERVER_BLOCK);

		if (token.type == OPEN_BRACKET)
			bracket++;
		else if (token.type == CLOSE_BRACKET)
			bracket--;

		if (token.value == "location")
			this->_parseLocationBlock(file, token, location);
		else if (bracket == 0 && !location)
		{
			file.printMapAndLines();
			return ;
		}
		else if (bracket == 0)
			return;

		token = file.getNextToken();
	}
}

void Service::_parseLocationBlock(FileChecker &file, Token &token, bool &location)
{
	std::string tmp = file.configs["location"];

	if (!location)
	{
		location = true;
		file.printMapAndLines();
		file.configs.clear();
		file.configs["location"] = tmp;
	}

	token = file.getNextToken();
	while (token.type != CLOSE_BRACKET)
		token = file.getNextToken();
	
	file.printMapAndLines();
	file.configs.clear();
}


void Service::_checkArguments(int ac, char **av)
{
	if (ac > 2)
		throw std::runtime_error(ERR_ARG);
	else if (ac == 2)
		this->_parseConfigFile(av[1]);
	else
		this->_parseConfigFile(DEFAULT_CONF);
}
