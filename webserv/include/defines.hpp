/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:52:10 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/24 13:34:56 by wcorrea-         ###   ########.fr       */
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
#include <dirent.h>

// Global variables
extern bool	g_shutdown;

// Default settings
#define DEFAULT_CONF		"system/default.conf"
#define EXT_CONF			".conf"
#define EXT_HTML			".html"
#define CGI_OUTPUT_FILE		".cgi_output"
#define PYTHON_EXT			".py"
#define MAX_PORT			65535
#define MAX_SIZE_LIMIT		10737418240.0	// 10GB
#define MAX_PENDING			10				// Maximum number of pending connections
#define POLL_TIME_OUT		200				// 200ms
#define BUFFER_SIZE			2048			// 2KB
#define SENT_TIMEOUT		60				// 60s
#define MAX_URI_LENGHT		1024			// 1024 characters	
#define MAX_LOOP_COUNT		10				// 10 loops


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
#define COLON				':'
#define SEMICOLON			';'
#define SPACE				' '
#define SLASH				'/'
#define INTERROGATION		'?'
#define INTERROGATION_STR	"?"
#define SLASH_STR			"/"
#define SPACES				" \t\r\v\f"
#define CURSOR_NEWLINE		"\r\n"
#define DECIMAL				"0123456789"
#define SUFIX_BYTES			"bBkKmMgG"
#define RELATIVE_BACK		"../"
#define LITERAL_BACK		".."
#define CARRIAGE_RETURN		'\r'
#define STR_NEWLINE			"\n"
#define REQUEST_END			"\n\n\n"
#define REQUEST_HOST		"Host: "

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

// Check send request keywords
#define GET				"GET"
#define DELETE			"DELETE"
#define POST			"POST"
#define HTTP_1_0		"HTTP/1.0"
#define HTTP_1_1		"HTTP/1.1"
#define CONTENT_LENGTH	"Content-Length"
#define CONTENT_TYPE	"Content-Type"
#define SCRIPT_NAME		"SCRIPT_FILENAME="

// printInfo messages
#define START_MSG					"Starting Webserv Engine..."
#define END_MSG						"Webserv Engine shutdown complete"
#define SETUP_MSG					"Setting up servers..."
#define SET_SERVER_MSG(host, port)	"Server " + host + ":" + port + " setup complete"
#define LAUNCH_MSG					"Launching servers..."
#define SHUTDOWN_MSG				"Shutting down Webserv Engine..."
#define POLLERR_MSG					"Connection closed. Error: POLLERR"
#define POLLHUP_MSG					"Connection closed. Error: POLLHUP"
#define POLLNVAL_MSG				"Connection closed. Error: POLLNVAL"
#define CLOSE_MSG					"Connection closed"
#define TIMEOUT_MSG					"Connection closed. Timeout"

// SendResponse status
#define RS_200	"200 OK"
#define RS_400	"400 Bad Request"
#define RS_403	"403 Forbidden"
#define RS_404	"404 Not Found"
#define RS_405	"405 Method Not Allowed"
#define RS_409	"409 Conflict"
#define RS_411	"411 Length Required"
#define RS_413	"413 Payload Too Large"
#define RS_414	"414 URI Too Long"
#define RS_500	"500 Internal Server Error"
#define RS_501	"501 Not Implemented"
#define RS_505	"505 HTTP Version Not Supported"
#define RS_508	"508 Loop Detected"

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

// ScriptInfo constructor & its auxiliar methods errors
#define ERR_SCRIPT_NAME					"No SCRIPT_FILENAME found in environment"
#define ERR_SCRIPT_NO_EXT(extension)	"No extension found. File must have the extension '" + extension + "'"
#define ERR_SCRIPT_EXT(extension, tmp)	"Invalid extension. File must have the extension '" + extension + "' and not '" + tmp + "'"
#define ERR_SCRIPT_INV(script)			"Couldn't open script '" + script + "'"
