/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:52:10 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/15 21:19:42 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// standart libraries
#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <map>
#include <vector>
#include <utility>
#include <limits>
#include <climits>
#include <csignal>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>

// Global variables
extern bool	g_shutdown;

// Default settings
#define DEFAULT_CONF	"system/default.conf"
#define EXT_CONF		".conf"
#define EXT_HTML		".html"
#define MAX_PORT		65535
#define MAX_SIZE_LIMIT	10737418240.0	// 10GB
#define MAX_PENDING		10				// Maximum number of pending connections
#define POLL_TIME_OUT	200				// 200ms
#define BUFFER_SIZE		2048			// 2KB
#define SENT_TIMEOUT	60				// 60s

// Custom Outputs
#define RED		"\033[0;31m"
#define GREEN	"\033[0;32m"
#define BLUE	"\033[0;34m"
#define RESET	"\033[0m"
#define CLEAR	"\033c"

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
#define DECIMAL				"0123456789"
#define SUFIX_BYTES			"bBkKmMgG"

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

// printInfo messages
#define START_MSG					"Starting Webserv Engine..."
#define END_MSG						"Webserv Engine shutdown complete"
#define SETUP_MSG					"Setting up servers..."
#define SET_SERVER_MSG(host, port)	"Server " + host + ":" + port + " setup complete"
#define SHUTDOWN_MSG				"Shutting down Webserv Engine..."
#define POLLERR_MSG					"Connection closed. Error: POLLERR"
#define POLLHUP_MSG					"Connection closed. Error: POLLHUP"
#define POLLNVAL_MSG				"Connection closed. Error: POLLNVAL"
#define CLOSE_MSG					"Connection closed"
#define TIMEOUT_MSG					"Connection closed. Timeout"

// FileChecker check input errors
#define ERR_ARG							"Invalid arguments\n\tUsage: ./webserv [config_file]"
#define ERR_FILE						" is a invalid file\n\tFile must have a name and must be .conf"
#define ERR_OPEN						"Couldn't open file "

// FileChecker getNextToken & its auxiliar methods errors
#define ERR_INVALID_KEY(token, line)	"Invalid keyword '" + token + "' at line " + line
#define ERR_MISSING_VALUE(token, line)		"Missing value for keyword '" + token + "' at line " + line
#define ERR_MISSING_OPEN_BRACKET(line)	"Missing '{' at line " + line
#define ERR_MANY_VALUES(token, line)	"Too many values for keyword '" + token + "' at line " + line
#define ERR_SEMICOLON(line)				"Missing ';' at line " + line
#define ERR_UNCLOSED_BRACKETS			"Unclosed brackets"
#define ERR_NO_SERVER_CONFIG			"No server configuration found"

// FileChecker getServerConfigs & its auxiliar methods errors
#define ERR_BLOCK_START(token, line)	"Invalid block start with '" + token + "' at line " + line + ". Expected 'server' keyword"
#define ERR_SERVER_BLOCK				"Missing '{' after 'server' keyword"

// ServerInfo constructor & its auxiliar methods errors
#define ERR_KEYWORD_MISSING(keyword)	"Missing keyword '" + keyword + "' in server block"
#define ERR_FORBIDDEN_KEYWORD(keyword)	"Keyword '" + keyword + "' is not allowed in server block"
#define ERR_DUPLICATE_NAME(name)		"Duplicate server name '" + name + "'. Server names must be unique"
#define ERR_DIRECTORY(path)				"'" + path + "' is not a valid directory"
#define ERR_PAGE_EXT(page)				"'" + page + "' is a invalid file. Pages must be .html"
#define ERR_PAGE_FIND(page)				"Couldn't open page '" + page + "'"
#define ERR_PORT_INPUT(port)			"'"+ port + "' is not a valid port number. Port must be a number between 0 and 65535"
#define ERR_MAX_SIZE_INPUT(size)		"'" + size + "' is not a valid size. Size must be a number positive or a number followed by a sufix (b - B, k - K, m - M, g - G)"
#define ERR_MAX_SIZE_RANGE(size)		"'" + size + "' is not a valid size. The max value allowed is 10G (10737418240 bytes)"
#define ERR_SOCKET(server)				"failed to create network socket for server " + server

// Service setServersAddress errors
#define ERR_SET_SOCKET					"setsockopt() failed: "
#define ERR_GET_ADDR_INFO				"getaddrinfo() failed: "
#define ERR_BIND_SOCKET					"bind() failed: "
#define ERR_LISTEN_SOCKET				"listen() failed: "

// Service launch errors
#define ERR_POLL_FAIL					"poll() failed"
#define ERR_ACCEPT_SOCKET				"accept() failed"
