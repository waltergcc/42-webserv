/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:38:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/12 14:36:03 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

// ---> Boolean checkers -----------------------------------------------------

bool isValidKeyword(std::string const &s)
{
	return (s == ALLOW_M|| s == AUTOID || s == CGI_E|| s == CGI_P || s == MAX_SIZE 
		|| s == ERROR_P || s == HOST || s == INDEX || s == LISTEN || s == LOCATION
		|| s == RETURN || s == ROOT || s == SERVER || s == SERVER_N || s == TRY || s == UPLOAD);
}

//	---> String Utils --------------------------------------------------------

std::string intToString(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

stringVector getStringTokens(std::string const &s, char c)
{
	stringVector tokens;
	std::stringstream ss(s);
	std::string token;

	while (std::getline(ss, token, c))
	{
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
}

void stringTrim(std::string &s, char const *set)
{
	s.erase(0, s.find_first_not_of(set));
	s.erase(s.find_last_not_of(set) + 1);
}

std::string getPathWithSlashAtEnd(std::string const &path)
{
	std::string tmp = path;

	if (tmp[tmp.length() - 1] != SLASH)
		tmp += "/";
	return tmp;
}

std::string getPathWithoutSlashAtBegin(std::string const &path)
{
	std::string tmp = path;

	if (tmp.at(0) == SLASH)
		tmp.erase(0, 1);
	return tmp;
}

// ---> Time Utils -----------------------------------------------------------

std::string getTime()
{
	time_t		now = time(0);
	struct tm	tstruct;
	char		buf[9];

	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
	return (buf);
}

std::string getTimeStamp()
{
	time_t		now = time(0);
	struct tm	tstruct;
	char		buf[80];

	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tstruct);
	return (buf);
}

// ---> File Utils -----------------------------------------------------------

bool isReadbleFile(std::string const &path)
{
	std::ifstream file(path.c_str());
	return (file.good());	
}

bool hasThisExtension(std::string const &file, std::string const &extension)
{
	size_t dot = file.find_last_of(".");
	
	if (dot == std::string::npos || file.substr(dot) != extension || file.length() <= extension.length())
		return false;
	return true;
}

bool directoryExists(std::string const &path)
{
	struct stat info;

	if (stat(path.c_str(), &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	return false;
}

std::string getFileContent(std::string const &path)
{
	std::string content;
	std::ifstream file(path.c_str(), std::ios::binary | std::ios::in);

	content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return (content);
}

std::string getFileType(std::string const &file)
{
	stringMap types;

	types["txt"] = "text/plain";
	types["html"] = "text/html";
	types["css"] = "text/css";

	types["js"] = "application/javascript";
	types["py"] = "application/python";

	types["jpg"] = "image/jpg";
	types["jpeg"] = "image/jpeg";
	types["png"] = "image/png";
	types["gif"] = "image/gif";

	if (file.find_last_of(".") != std::string::npos)
	{
		std::string extension = file.substr(file.find_last_of(".") + 1);
		if (types.find(extension) != types.end())
			return (types[extension]);
	}
	return ("text/plain");
}

// ---> Log Utils ------------------------------------------------------------

void printInfo(std::string const &s, std::string const &color)
{
	if (s == START_MSG)
		std::cout << CLEAR;
	
	if (s == SHUTDOWN_MSG)
		std::cout << std::endl;

	std::cout << color << "[" << getTime() << "] " << RESET << s << std::endl;
}

// ---> Signal Utils -----------------------------------------------------------

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		g_shutdown = true;
		printInfo(SHUTDOWN_MSG, RED);
	}
}
