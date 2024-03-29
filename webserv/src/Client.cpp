/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:29:53 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/11/09 17:14:48 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// ---> Constructor and destructor --------------------------------------------

Client::Client(Server server, int socket) : _server(server), _socket(socket), _sentRequest(false), _lastRequest(std::time(NULL)){}
Client::~Client(){}

// ---> Public member functions ----------------------------------------------

void	Client::appendRequest(char const *buffer, size_t size)
{
	this->_lastRequest = std::time(NULL);
	this->_sentRequest = false;
	this->_request.append(buffer, size);
}

bool	Client::isTimeout() const
{
	return (std::time(NULL) - this->_lastRequest > SENT_TIMEOUT);
}

bool	Client::isReadyToSend() const
{
	return (this->_sentRequest == true || this->_request.find(REQUEST_END) != std::string::npos);
}

void	Client::sendResponse()
{
	this->_sentRequest = true;
	this->_lastRequest = std::time(NULL);

	try
	{
		this->_checkRequest();
		std::string root = this->_server.getRoot();
		std::string resource = getPathWithoutSlashAtEnd(this->_resource);
		this->_checkLocation(root, resource, 0);
	}
	catch(const std::exception& e)
	{
		this->_writeErrorResponse(e.what());
	}

	this->_request.clear();
	this->_requestPayload.clear();
	this->_sentRequest = false;
}

// ---> _checkLocation auxiliars ---------------------------------------------

void	Client::_checkLocation(std::string &root, std::string &resource, size_t loopCount)
{
	if (loopCount > MAX_LOOP_COUNT)
		throw std::runtime_error(RS_508);

	locationMap::const_iterator location = this->_server.getLocations().begin();
	for (; location != this->_server.getLocations().end(); location++)
	{
		if (this->_locationIsRootAndResourceNot(location->first, resource))
			continue;
		if (!this->_resourceHasLocation(location->first, resource) && this->_method != DELETE)
			continue;
		if (!this->_methodMatches(location->second.methods))
		{
			if (!this->_hasValidDelete(resource))
				throw std::runtime_error(RS_405);
		}
		if (this->_hasRedirection(resource, root, loopCount, location->second.redirect, location->first))
			return;
		this->_updateRootIfLocationHasIt(root, location->second.root);
		if (this->_hasValidPath(resource, root, location->second))
			return;
		break;
	}
	if (this->_hasInvalidLocation(location, resource))
		throw std::runtime_error(RS_403);
	this->_updateResourceWhenHasCgiPath(resource, location);
	this->_methodsManager(root, resource, location->second);
}

bool	Client::_locationIsRootAndResourceNot(std::string const &location, std::string &resource)
{
	return (location == SLASH_STR && resource != SLASH_STR);
}

bool	Client::_resourceHasLocation(std::string const &location, std::string &resource)
{
	return (resource.find(location) != std::string::npos && isItSufix(resource, location));
}

bool	Client::_methodMatches(stringVector const &methods)
{
	stringVector::const_iterator method;
	for (method = methods.begin(); method != methods.end(); method++)
	{
		if (*method == this->_method)
			return true;
	}
	return false;
}

bool	Client::_hasValidDelete(std::string const &resource)
{
	if (this->_method == DELETE)
	{
		std::string path = getPathWithoutFilename(resource);
		locationMap::const_iterator location = this->_server.getLocations().begin();
		for (; location != this->_server.getLocations().end(); location++)
		{
			if (this->_resourceHasLocation(location->first, path))
			{
				if (this->_methodMatches(location->second.methods))
					return true;
				else
					return false;
			}
		}
		return false;
	}
	return false;
}

bool	Client::_hasRedirection(std::string &resource, std::string &root, size_t loopCount, std::string const &redirect, std::string const &location)
{
	if (redirect.length() == 0)
		return false;

	size_t pos = resource.find(location);
	std::string newResource = resource;

	newResource.replace(pos, location.length(), redirect);
	this->_checkLocation(root, newResource, loopCount + 1);
	return true;
}

void	Client::_updateRootIfLocationHasIt(std::string &root, std::string const &locationRoot)
{
	if (locationRoot.length() == 0)
		return;
	root = locationRoot;
}

bool	Client::_hasInvalidLocation(locationMap::const_iterator &location, std::string const &resource)
{
	if (location == this->_server.getLocations().end())
	{
		if (this->_method == POST)
			return true;

		if (this->_method == GET && this->_hasScriptExtension(resource))
			return true;
	}
	return false;
}

bool	Client::_hasScriptExtension(std::string const &resource)
{
	return (isItSufix(resource, PYTHON_EXT) || isItSufix(resource, PHP_EXT));
}

bool	Client::_hasValidPath(std::string const &resource, std::string const &root, location_t const &location)
{
	std::string path = getPathWithSlashAtEnd(getCorrectPath(root, resource));

	if (directoryExists(path) && this->_method == GET)
	{
		if (location.tryFile.size())
			this->_writeResponseOnSocket(getCorrectPath(root, location.tryFile));
		else if (location.autoindex)
			this->_writeAutoIndexResponse(path);
		else if (resource == SLASH_STR)
			this->_writeResponseOnSocket(getCorrectPath(root, this->_server.getIndex()));
		else
			throw std::runtime_error(RS_403);
		return true;
	}
	return false;
}

void	Client::_writeResponseOnSocket(std::string const &filepath)
{
	std::ifstream file(filepath.c_str(), std::ios::binary | std::ios::in);

	if (!file.is_open())
	{
		write(this->_socket, this->_server.getErrorResponse().c_str(), this->_server.getErrorResponse().length());
		printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + RS_404, RED);
		return;
	}
	file.close();

	std::string response = generateResponseOK(filepath);
	write(this->_socket, response.c_str(), response.length());
	printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + RS_200, GREEN);
}

void	Client::_writeAutoIndexResponse(std::string const &path)
{
	std::string htmlInfo;
	DIR *root;
	dirent *current;

	root = opendir(path.c_str());
	htmlInfo.append("<header>\n");
    htmlInfo.append("<nav class='navbar'>\n");
    htmlInfo.append("<div class='container'>\n");
    htmlInfo.append("<h1>webserv</h1>\n");
    htmlInfo.append("<ul class='navigation'>\n");
    htmlInfo.append("<a class='glow-on-hover' href='index.html'>HOME</a>\n");
    htmlInfo.append("<a class='glow-on-hover' href='get.py'>GET</a>\n");
    htmlInfo.append("<a class='glow-on-hover' href='post.html'>POST</a>\n");
    htmlInfo.append("<a class='glow-on-hover' href='delete.py'>DELETE</a>\n");
    htmlInfo.append("</ul>\n");
    htmlInfo.append("</div>\n");
    htmlInfo.append("</nav>\n");
    htmlInfo.append("</header>\n");
	htmlInfo.append("\t<h2>Autoindex of " + this->_resource + "</h2>\n");
	while ((current = readdir(root)) != NULL)
	{
		if (current->d_name == std::string(".") || current->d_name == std::string(".."))
			continue;
		htmlInfo.append("\t<a href=\"" + getPathWithoutSlashAtBegin(this->_resource) + "/" + current->d_name + "\">" + current->d_name + "</a><br>\n");
	}
	closedir(root);
	htmlInfo.append("\t<p>Go to:\n");
	htmlInfo.append("\t\t<a href=\"index.html\">Back to Home</a>\n");
	htmlInfo.append("\t</p>\n");

	std::string response = generateResponseWithCustomHTML(RS_200, getPathWithoutSlashAtBegin(this->_resource), htmlInfo);
	write(this->_socket, response.c_str(), response.length());
	printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + RS_200, GREEN);
	this->_request.clear();
}

void	Client::_updateResourceWhenHasCgiPath(std::string &resource, locationMap::const_iterator &location)
{
	if (location == this->_server.getLocations().end())
		return;

	if (location->second.cgiPath.empty())
		return;

	std::string toFind = getPathWithoutSlashAtBegin(location->second.cgiPath);
	if (this->_method == POST || (this->_method == GET && this->_hasScriptExtension(resource)))
	{
		size_t pos = resource.find(toFind);
		if (pos != std::string::npos)
			resource.erase(pos, toFind.length());
	}
}

// ---> _methodsManager auxiliars --------------------------------------------

void	Client::_methodsManager(std::string &root, std::string &resource, location_t const &location)
{
	(void)location;
	if (this->_method == GET)
		this->_methodGet(root, resource, location);
	else if (this->_method == POST)
		this->_methodPost(resource, location);
	else if (this->_method == DELETE)
		this->_methodDelete(root, resource);
}

void	Client::_methodDelete(std::string const &root, std::string const &resource)
{
	std::string file = getCorrectPath(root, resource);
	std::string response;

	if (remove(file.c_str()) == 0)
	{
		response = generateResponseWithCustomHTML(RS_200, "OK", "\t<h1>File deleted.</h1>\n");
		write(this->_socket, response.c_str(), response.length());
		printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + RS_200, GREEN);
	}
	else
	{
		response = this->_server.getErrorResponse();
		write(this->_socket, response.c_str(), response.length());
		printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + RS_404, RED);
	}
}

void	Client::_methodGet(std::string &root, std::string &resource, location_t const &location)
{
	if (this->_hasScriptExtension(resource))
	{
		std::string response;
		try
		{
			stringVector enviromnent = this->_createEnvironment(resource, location);
			Script cgi(location.cgiExtension, this->_request, enviromnent, 0, "");

			std::string title = convertScriptNameToTitle(resource);
			response = generateResponseWithCustomHTML(RS_200, title, getFileContent(CGI_OUTPUT_FILE));
			write(this->_socket, response.c_str(), response.length());
			printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + RS_200, GREEN);
		}
		catch(const std::exception& e)
		{
			response = generateResponseWithCustomHTML(RS_500, "Internal Server Error", "\t<h1>500 Internal Server Error</h1>\n");
			write(this->_socket, response.c_str(), response.length());
			printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + RS_500, RED);
			std::cerr << "           " << e.what() << std::endl;
		}
		return;
	}

	std::string path = this->_getPath(root, resource);

	if (directoryExists(path))
		throw std::runtime_error(RS_403);

	this->_writeResponseOnSocket(path);
}

void	Client::_methodPost(std::string &resource, location_t const &location)
{
	std::string response;
	std::string uploadPath;

	if (location.uploadTo.empty() || location.hasCGI == false)
		throw std::runtime_error(RS_405);

	try
	{
		uploadPath = getCorrectPath(this->_getAvaliableRoot(location), location.uploadTo);
		stringVector environment = this->_createEnvironment(resource, location);
		Script cgi(location.cgiExtension, this->_request, environment, this->_contentLength, uploadPath);

		response = generateResponseWithCustomHTML(RS_200, "Post Page", getFileContent(CGI_OUTPUT_FILE));
		write(this->_socket, response.c_str(), response.length());
		printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + RS_200, GREEN);
	}
	catch(const std::exception& e)
	{
		response = generateResponseWithCustomHTML(RS_500, "Internal Server Error", "\t<h1>500 Internal Server Error</h1>\n");
		write(this->_socket, response.c_str(), response.length());
		printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + RS_500, RED);
		std::cerr << "           " << e.what() << std::endl;
	}
}

std::string	Client::_getPath(std::string const &root, std::string const &resource)
{
	if (resource == SLASH_STR)
		return (getPathWithSlashAtEnd(getCorrectPath(root, resource)) + this->_server.getIndex());
	return (getCorrectPath(root, resource));
}

// ---> _createEnvironment auxiliars ------------------------------------------

stringVector	Client::_createEnvironment(std::string &resource, location_t const &location)
{
	stringVector environment;
	std::string root = this->_getAvaliableRoot(location);
	std::string path = this->_getFullPath(location, this->_server.getRoot(), resource);

	environment.push_back("SCRIPT_FILENAME=" + path);

	if (this->_headers.count(CONTENT_LENGTH) > 0)
		environment.push_back("CONTENT_LENGTH=" + this->_headers[CONTENT_LENGTH]);

	if (this->_headers.count(CONTENT_TYPE) > 0)
		environment.push_back("CONTENT_TYPE=" + this->_headers[CONTENT_TYPE]);

	if (location.uploadTo.length() > 0)
		environment.push_back("UPLOAD_PATH=" + getCorrectPath(root, location.uploadTo));

	if (this->_headers.count(USER_AGENT) > 0 && this->_method == POST)
	{
		if (this->_headers[USER_AGENT].find("curl") != std::string::npos)
			environment.push_back("PAYLOAD=" + this->_requestPayload);
	}

	environment.push_back("GATEWAY_INTERFACE=CGI/1.1");
	environment.push_back("REQUEST_METHOD=" + this->_method);
	environment.push_back("SERVER_PROTOCOL=HTTP/1.1");
	environment.push_back("SERVER_SOFTWARE=Webserv/1.0");
	environment.push_back("ROOT_FOLDER=" + root);

	return environment;
}

std::string		Client::_getAvaliableRoot(location_t const &location)
{
	if (location.root.length() > 0)
		return location.root;
	return this->_server.getRoot();
}

std::string		Client::_getFullPath(location_t const &location, std::string const &root, std::string const &resource)
{
	std::string cgi = location.cgiPath;
	if (cgi.empty())
		return getCorrectPath(root, resource);

	std::string path = getCorrectPath(root, cgi);
	return getCorrectPath(path, resource);
}

// ---> _checkRequest auxiliars ------------------------------------------------

void	Client::_checkRequest()
{
	std::stringstream	ss(this->_request);

	this->_requestPayload.clear();
	this->_headers.clear();
	this->_checkFirstLine(ss);
	this->_checkAndGetHeaders(ss);
	this->_checkAndGetPayload(ss);
}

void	Client::_checkFirstLine(std::stringstream &ss)
{
	std::string		line;
	std::getline(ss, line);
	stringVector	parameters = getStringTokens(line, SPACE);

	if (parameters.size() != 3)
		throw std::runtime_error(RS_400);

	if (parameters.at(0) != GET && parameters.at(0) != DELETE && parameters.at(0) != POST)
		throw std::runtime_error(RS_501);
	this->_method = parameters.at(0);

	if (parameters.at(1).length() > MAX_URI_LENGHT)
		throw std::runtime_error(RS_414);

	if (parameters.at(1).find(RELATIVE_BACK) != std::string::npos || parameters.at(1) == LITERAL_BACK)
		throw std::runtime_error(RS_400);
	this->_resource = urlDecode(parameters.at(1));

	if (parameters.at(2) == HTTP_1_0)
		throw std::runtime_error(RS_505);

	if (parameters.at(2) != HTTP_1_1)
		throw std::runtime_error(RS_400);
}

void	Client::_checkAndGetHeaders(std::stringstream &ss)
{
	std::string line;
	while (std::getline(ss, line))
	{
		if (line == CARRIAGE_RETURN_STR)
			break;

		if (line.find(COLON) != std::string::npos)
		{
			std::string key = line.substr(0, line.find(COLON));
			std::string value = line.substr(line.find(COLON) + 1, line.find(NEWLINE));
			stringTrim(key, SPACES);
			stringTrim(value, SPACES);

			if (value.length() != 0 && key.length() != 0)
				this->_headers[key] = value;
			else
				throw std::runtime_error(RS_400);
		}
	}
}

void	Client::_checkAndGetPayload(std::stringstream &ss)
{
	if (this->_method == GET || this->_method == DELETE)
		return;
	else if (this->_method == POST && this->_headers.find(CONTENT_LENGTH) == this->_headers.end())
		throw std::runtime_error(RS_411);

	this->_contentLength = this->_getValidContentLength(this->_headers[CONTENT_LENGTH]);

	std::streampos pos = ss.tellg();
	std::stringstream binarySs(ss.str(), std::stringstream::in | std::stringstream::binary);
	binarySs.seekg(pos);
	this->_requestPayload.resize(this->_contentLength);
	binarySs.read(&this->_requestPayload[0], this->_contentLength);
}

size_t	Client::_getValidContentLength(std::string const &length)
{
	size_t tmp = std::atoi(length.c_str());

	if (length.find_first_not_of(DECIMAL) != std::string::npos)
		throw std::runtime_error(RS_409);

	if (tmp > this->_server.getClientMaxBodySize())
		throw std::runtime_error(RS_413);

	return tmp;
}

// ---> Common auxiliars -------------------------------------------------------

void	Client::_writeErrorResponse(std::string const &error)
{
	std::string body =
		"    <div class=\"error-container\">\n"
		"        <h1 class=\"error-heading\">Erro " + error + "</h1>\n"
		"        <p class=\"error-message\">Sorry, an error occurred during the processing of your request.</p>\n"
		"	 	 <p>Go to:\n"
		"			 <a href=\"index.html\">Back to Home</a>\n"
		"    </div>\n";

	std::string response = generateResponseWithCustomHTML(error, "Error", body);
	write(this->_socket, response.c_str(), response.length());

	if (!this->_method.empty() && !this->_resource.empty())
		printInfo(this->_server.getHost() + ":" + this->_server.getPort() + " " + this->_method + " " + this->_resource + " -> " + error, RED);
	else
		printInfo(error, RED);
}

// ---> Getters and setters ---------------------------------------------------

std::string const	&Client::getRequest() const{return (this->_request);}
Server const		&Client::getServer() const{return (this->_server);}
void				Client::changeServer(Server server){this->_server = server;}
