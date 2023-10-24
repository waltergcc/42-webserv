/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:35:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 16:09:38 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"
#include "ServerInfo.hpp"
#include "utils.hpp"

class ClientInfo
{
	private:
		ServerInfo	_server;
		int			_socket;
		bool		_sentRequest;
		std::string	_request;
		time_t		_lastRequest;
		std::string	_requestPayload;
		std::string	_method;
		std::string _resourceTarget;
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
		bool	_hasRedirection(std::string &resource, std::string &root, size_t loopCount, std::string const &redirect, std::string const &location);
		void	_updateRootIfLocationHasIt(std::string &resource, std::string &root, std::string const &location, std::string const &locationRoot);
		bool	_hasInvalidLocation(locationMap::const_iterator &location);
		bool	_hasValidPath(std::string const &resource, std::string const &root, location_t const &location);
		void	_writeResponseOnSocket(std::string const &filepath);
		void	_writeAutoIndexResponse(std::string const &path);
		void	_updateResourceIfNecessary(std::string &resource, std::string const &location);

		// _methodsManager auxiliars
		void			_methodsManager(std::string &root, std::string &resource, location_t const &location);
		void			_methodDelete(std::string const &root, std::string const &resource);
		void			_methodGet(std::string &root, std::string &resource, location_t const &location);
		stringVector	_createEnvironment(std::string &resource, location_t const &location);

		// Common auxiliars
		// void	_cleanRequest();
		void	_writeErrorResponse(std::string const &error);

		ClientInfo();

	public:
		ClientInfo(ServerInfo server, int socket);
		~ClientInfo();

		void	appendRequest(char const *buffer, size_t size);
		bool	isTimeout() const;
		bool	isReadyToSend() const;
		void	sendResponse();

		// Getters and setters
		std::string const	&getRequest() const;
		ServerInfo const	&getServer() const;
		void				changeServer(ServerInfo server);
};
