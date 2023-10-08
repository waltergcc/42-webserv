/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:52:10 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/08 16:35:16 by wcorrea-         ###   ########.fr       */
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
#include <map>
#include <vector>
#include <utility>

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
#define SEMICOLON			';'
#define SPACE				' '
#define SLASH				'/'
#define SPACES				" \t\r\v\f"

// Common typedefs
typedef std::map<std::string, std::string>	stringMap;
typedef std::vector<std::string>			stringVector;

// keywords
#define ALLOW_M		"allow_methods"
#define AUTOID		"autoindex"
#define CGI_E		"cgi_ext"
#define CGI_P		"cgi_path"
#define MAX_SIZE	"client_max_body_size"
#define ERROR_P		"error_page"
#define	HOST		"host"
#define INDEX		"index"
#define LISTEN		"listen"
#define LOCATION	"location"
#define RETURN		"return"
#define ROOT		"root"
#define SERVER		"server"
#define SERVER_N	"server_name"
#define TRY 		"try_file"
#define UPLOAD		"upload_to"

// Error messages
#define ERR_ARG							"Invalid arguments\n\tUsage: ./webserv [config_file]"
#define ERR_OPEN						"Couldn't open file "
#define ERR_FILE						" is a invalid file\n\tFile must have a name and must be .conf"
#define ERR_MANDATORY					"server_name, listen, host are mandatory fields"
#define ERR_INVALID_KEY(token, line)	"Invalid keyword '" + token + "' at line " + line
#define ERR_MISSING_VALUE(token, line)		"Missing value for keyword '" + token + "' at line " + line
#define ERR_MISSING_OPEN_BRACKET(line)	"Missing '{' at line " + line
#define ERR_MANY_VALUES(token, line)	"Too many values for keyword '" + token + "' at line " + line
#define ERR_SEMICOLON(line)				"Missing ';' at line " + line
#define ERR_UNCLOSED_BRACKETS			"Unclosed brackets"
#define ERR_NO_SERVER_CONFIG			"No server configuration found"
#define ERR_SERVER_BLOCK				"Missing '{' after 'server' keyword"
#define ERR_KEYWORD_MISSING(keyword)	"Missing keyword '" + keyword + "' in server block"
#define ERR_FORBIDDEN_KEYWORD(keyword)	"Keyword '" + keyword + "' is not allowed in server block"
