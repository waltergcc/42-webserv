/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:52:10 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/02 01:15:58 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>

using namespace std;

#define DEFAULT_CONF "system/default.conf"

#define RED		"\033[0;31m"
#define GREEN	"\033[0;32m"
#define RESET	"\033[0m"
#define CLEAR	"\033[2J\033[1;1H"

#define ERR_ARG			"Invalid arguments\n\tUsage: ./webserv [config_file]"
#define ERR_OPEN		"Couldn't open file "
#define ERR_FILE		" is a invalid file\n\tFile must have a name and must be .conf"
#define ERR_MANDATORY	"server_name, listen, host are mandatory fields"
