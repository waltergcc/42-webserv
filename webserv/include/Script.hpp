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
		std::string	_getValidExtension(std::string const &extension);
		std::string	_getValidScriptPath();
		std::string _getScriptName();
		std::string	_getValidUploadTo(std::string const &uploadTo);
		void		_setArgvEnvp();
		void		_run();
		void		_setPipeAndFile(int &file, int (&fd)[2]);
		void		_executeBin(int &file, int (&fd)[2]);

		Script();
		
	public:
		Script(std::string const &extension, std::string const &request, stringVector const &environment, size_t size, std::string const &uploadTo);
		~Script();
};
