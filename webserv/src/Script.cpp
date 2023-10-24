/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Script.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 12:07:43 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/24 13:33:16 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Script.hpp"

// ---> Constructors and Destructors -------------------------------------------------- //

Script::Script(std::string const &extension, std::string const &request, stringVector const &environment, size_t size, std::string const &uploadTo)
{
	this->_extension = extension;
	this->_request = request;
	this->_environment = environment;
	this->_size = size;
	this->_uploadTo = uploadTo;
	this->_path = this->_getValidPath();
	this->_setArgvEnvp();

	// debug info
	for (size_t i = 0; this->_argv[i]; i++)
		std::cout << "argv[" << i << "] = " << this->_argv[i] << std::endl;
	
	for (size_t i = 0; this->_envp[i]; i++)
		std::cout << "envp[" << i << "] = " << this->_envp[i] << std::endl;

}

Script::~Script()
{
	if (this->_argv)
	{
		for (size_t i = 0; this->_argv[i]; i++)
			free(this->_argv[i]);
		delete [] this->_argv;
	}

	if (this->_envp)
	{
		for (size_t i = 0; this->_envp[i]; i++)
			free(this->_envp[i]);
		delete [] this->_envp;
	}
}

// ---> Private Constructors auxiliars -------------------------------------------------- //

std::string Script::_getValidPath()
{
	std::string path = this->_getScriptName();
	size_t		dot;

	if (path.empty())
		throw std::runtime_error(ERR_SCRIPT_NAME);
	
	dot = path.find_last_of(".");
	if (dot == std::string::npos)
		throw std::runtime_error(ERR_SCRIPT_NO_EXT(this->_extension));
	
	std::string tmp = path.substr(dot);
	if (tmp != this->_extension)
		throw std::runtime_error(ERR_SCRIPT_EXT(this->_extension, tmp));
	
	if (!isReadbleFile(path))
		throw std::runtime_error(ERR_SCRIPT_INV(path));

	return path;
}

std::string Script::_getScriptName()
{
	stringVector::const_iterator it = this->_environment.begin();
	for (; it != this->_environment.end(); it++)
	{
		if (it->find(SCRIPT_NAME) != std::string::npos)
			return it->substr(it->find("=") + 1);
	}
	return "";
}

void		Script::_setArgvEnvp()
{
	if (this->_extension == PYTHON_EXT)
	{
		this->_executable = PYTHON_BIN;
		this->_argv = new char*[4];
		this->_argv[0] = strdup(PYTHON_VERSION);
		this->_argv[1] = strdup(this->_path.c_str());
		this->_argv[2] = strdup(this->_uploadTo.c_str());
		this->_argv[3] = NULL;
	}

	this->_envp = new char*[this->_environment.size() + 1];
	for (size_t i = 0; i < this->_environment.size(); i++)
		this->_envp[i] = strdup(this->_environment[i].c_str());
	this->_envp[this->_environment.size()] = NULL;
}