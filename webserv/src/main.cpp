/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:32:18 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/28 11:39:28 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int ac, char **av)
{
	try
	{
		Service webserv(ac, av);
		// webserv.printServersInfo();
		webserv.setup();
		webserv.launch();
	}
	catch(const std::exception &e)
	{
		std::cerr << RED << "Error:\t" << e.what() << RESET << std::endl;
		return (EXIT_FAILURE);
	}
}
