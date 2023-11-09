/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:38:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/11/09 17:09:37 by wcorrea-         ###   ########.fr       */
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

std::string getPathWithoutSlashAtEnd(std::string const &path)
{
	if (path == SLASH_STR)
		return path;

	std::string tmp = path;

	if (tmp[tmp.length() - 1] == SLASH)
		tmp.erase(tmp.length() - 1, 1);
	return tmp;
}

std::string getPathWithoutSlashAtBegin(std::string const &path)
{
	std::string tmp = path;

	if (tmp.at(0) == SLASH)
		tmp.erase(0, 1);
	return tmp;
}

std::string getCorrectPath(std::string const &root, std::string const &resource)
{
	if (resource == SLASH_STR)
		return root;
	return (getPathWithSlashAtEnd(root) + getPathWithoutSlashAtBegin(resource));
}

bool	isItSufix(std::string const &s, std::string const &sufix)
{
	if (s.length() < sufix.length())
		return false;
	return (s.substr(s.length() - sufix.length()) == sufix);
}

std::string convertScriptNameToTitle(std::string const &scriptName)
{
	std::string title = scriptName;

	if (title.find_last_of("/") != std::string::npos)
		title = title.substr(title.find_last_of("/") + 1);
	if (title.find_last_of(".") != std::string::npos)
		title = title.substr(0, title.find_last_of("."));
	
	for (size_t i = 0; i < title.length(); i++)
	{
		if (i == 0)
			title[i] = toupper(title[i]);
		else
			title[i] = tolower(title[i]);
	}
	
	return title + " page";
}

std::string getPathWithoutFilename(std::string const &path)
{
	std::string tmp = path;
	size_t slash = tmp.find_last_of("/");

	if (slash != std::string::npos)
		return tmp.substr(0, slash);
	return "/";
}

std::string urlDecode(const std::string &input)
{
	std::string tmp = input;
	size_t pos;

	while ((pos = tmp.find("%20")) != std::string::npos)
		tmp.replace(pos, 3, " ");
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

std::string generateResponseOK(std::string const &path)
{
	std::string response = 
		"HTTP/1.1 200 OK\n"
		"Date: " + getTimeStamp() + "\n" +
		"Server: Webserv/1.0.0 (Linux)\n" +
		"Connection: keep-alive\n" +
		"Content-Type: " + getFileType(path) + "; charset=utf-8\n" +
		"Content-Length: " + intToString(getFileContent(path).length()) + "\n\n";

	return (response + getFileContent(path));
}

std::string generateResponseWithCustomHTML(std::string const &code, std::string const &title, std::string const &body)
{
	std::string customHTML =
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>" + title + "</title>\n"
		"    <link rel=\"icon\" href=\"favicon.ico\" type=\"image/x-icon\">\n"
        "    <style>\n"
        "        body {\n"
        "            font-family: Arial, sans-serif;\n"
        "            background-color: #f4f4f4;\n"
        "            text-align: center;\n"
        "        }\n"
        "        .container {\n"
        "            margin: 100px auto;\n"
        "            padding: 20px;\n"
        "            background-color: #fff;\n"
        "            border-radius: 5px;\n"
        "            box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);\n"
        "        }\n"
        "        h1 {\n"
        "            color: #333;\n"
        "        }\n"
        "        p {\n"
        "            color: #666;\n"
        "        }\n"
		"		a {\n"
		"			color: #333;\n"
		"		}\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"container\">\n" + body +
        "    </div>\n"
        "</body>\n"
        "</html>\n";
	
	std::string response = 
		"HTTP/1.1 " + code + " " + title + "\n"
		"Date: " + getTimeStamp() + "\n" +
		"Server: Webserv/1.0.0 (Linux)\n" +
		"Connection: keep-alive\n" +
		"Content-Type: text/html; charset=utf-8\n" +
		"Content-Length: " + intToString(customHTML.length()) + "\n\n" + customHTML;

	return response;
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
		printInfo(SHUTDOWN_MSG, GREEN);
	}
}
