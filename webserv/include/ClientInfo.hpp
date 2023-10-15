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

		ClientInfo();

	public:
		ClientInfo(ServerInfo server, int socket);
		~ClientInfo();

		void				appendRequest(char const *buffer, size_t size);
		bool				isTimeout() const;
		bool				isReadyToSend() const;

		std::string const	&getRequest() const;
};
