/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:35:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 15:43:09 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <exception>
#include <fstream>
#include "defines.hpp"

class Server
{
	private:
		std::string _name;
		std::string _host;
		int _port;
		
		Server();
		void checkArguments(int ac, char **av);
		void checkConfigFile(std::string input);
		
	public:
		Server(int ac, char **av);
		~Server();
};
