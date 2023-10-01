/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:35:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 16:09:38 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include "defines.hpp"

class Server
{
	private:
		std::string _name;
		std::string _host;
		std::string _port;
		
		Server();
		void checkArguments(int ac, char **av);
		void checkConfigFile(std::string input);
		
	public:
		Server(int ac, char **av);
		~Server();
};
