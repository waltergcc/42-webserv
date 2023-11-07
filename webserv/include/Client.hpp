/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:35:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/11/07 09:59:07 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include "Script.hpp"
class Client
{
	private:
		Server		_server;
		int			_socket;
		bool		_sentRequest;
		std::string	_request;
		time_t		_lastRequest;
		std::string	_requestPayload;
		std::string	_method;
		std::string _resource;
		stringMap	_headers;
		size_t		_contentLength;

		// _checkRequest auxiliars
		void	_checkRequest();
		void	_checkFirstLine(std::stringstream &ss);
		void	_checkAndGetHeaders(std::stringstream &ss);
		void	_checkAndGetPayload(std::stringstream &ss);
		size_t	_getValidContentLength(std::string const &length);

		// _checkLocation auxiliars
		void	_checkLocation(std::string &root, std::string &resource, size_t loopCount);
		bool	_locationIsRootAndResourceNot(std::string const &location, std::string &resource);
		bool	_resourceHasLocation(std::string const &location, std::string &resource);
		bool	_methodMatches(stringVector const &methods);
		bool	_hasValidDelete(std::string const &resource);
		bool	_hasRedirection(std::string &resource, std::string &root, size_t loopCount, std::string const &redirect, std::string const &location);
		void	_updateRootIfLocationHasIt(std::string &root, std::string const &locationRoot);
		bool	_hasInvalidLocation(locationMap::const_iterator &location, std::string const &resource);
		bool	_hasScriptExtension(std::string const &resource);
		bool	_hasValidPath(std::string const &resource, std::string const &root, location_t const &location);
		void	_writeResponseOnSocket(std::string const &filepath);
		void	_writeAutoIndexResponse(std::string const &path);
		void	_updateResourceWhenHasCgiPath(std::string &resource, locationMap::const_iterator &location);

		// _methodsManager auxiliars
		void		_methodsManager(std::string &root, std::string &resource, location_t const &location);
		void		_methodDelete(std::string const &root, std::string const &resource);
		void		_methodGet(std::string &root, std::string &resource, location_t const &location);
		std::string	_getPath(std::string const &root, std::string const &resource);
		void		_methodPost(std::string &resource, location_t const &location);

		// _createEnvironment auxiliars
		stringVector	_createEnvironment(std::string &resource, location_t const &location);
		std::string		_getAvaliableRoot(location_t const &location);
		std::string		_getFullPath(location_t const &location, std::string const &root, std::string const &resource);

		// Common auxiliars
		void	_writeErrorResponse(std::string const &error);

		Client();

	public:
		Client(Server server, int socket);
		~Client();

		void	appendRequest(char const *buffer, size_t size);
		bool	isTimeout() const;
		bool	isReadyToSend() const;
		void	sendResponse();

		// Getters and setters
		std::string const	&getRequest() const;
		Server const		&getServer() const;
		void				changeServer(Server server);
};
