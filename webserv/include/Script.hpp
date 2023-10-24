/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Script.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 12:07:47 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/24 13:30:57 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"
#include "utils.hpp"

class Script
{
	private:
		std::string		_extension;
		std::string		_request;
		stringVector	_environment;
		size_t			_size;
		std::string		_uploadTo;
		std::string 	_path;
		char			**_envp;
		char			**_argv;
		std::string		_executable;

		// Constructor auxiliars
		std::string	_getValidPath();
		std::string _getScriptName();
		void		_setArgvEnvp();
		Script();
		
	public:
		Script(std::string const &extension, std::string const &request, stringVector const &environment, size_t size, std::string const &uploadTo);
		~Script();
};
