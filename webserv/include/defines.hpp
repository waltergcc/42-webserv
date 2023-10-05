/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:52:10 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/05 23:50:29 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// standart libraries
#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>

// Default settings
#define DEFAULT_CONF "system/default.conf"

// Custom Outputs
#define RED		"\033[0;31m"
#define GREEN	"\033[0;32m"
#define RESET	"\033[0m"
#define CLEAR	"\033[2J\033[1;1H"

// Charsets
#define NEWLINE				'\n'
#define HASH				'#'
#define OPEN_BRACKET_CHAR	'{'
#define CLOSE_BRACKET_CHAR	'}'
#define UNDERSCORE			'_'

// Error messages
#define ERR_ARG			"Invalid arguments\n\tUsage: ./webserv [config_file]"
#define ERR_OPEN		"Couldn't open file "
#define ERR_FILE		" is a invalid file\n\tFile must have a name and must be .conf"
#define ERR_MANDATORY	"server_name, listen, host are mandatory fields"
