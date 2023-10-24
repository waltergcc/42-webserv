/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScriptInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 12:07:43 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/24 13:03:13 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScriptInfo.hpp"

// ---> Constructors and Destructors -------------------------------------------------- //

ScriptInfo::ScriptInfo(std::string const &extension, std::string const &request, stringVector const &environment, size_t size, std::string const &uploadTo)
{
	this->_extension = extension;
	this->_request = request;
	this->_environment = environment;
	this->_size = size;
	this->_uploadTo = uploadTo;
	this->_path = this->_getValidPath();

	std::cout << "Extension: " << this->_extension << std::endl;
	std::cout << "Request: " << this->_request << std::endl;
	std::cout << "Environment: " << std::endl;
	for (stringVector::const_iterator it = this->_environment.begin(); it != this->_environment.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "Size: " << this->_size << std::endl;
	std::cout << "UploadTo: " << this->_uploadTo << std::endl;
	std::cout << "Path: " << this->_path << std::endl;
}

ScriptInfo::~ScriptInfo()
{
	std::cout << "ScriptInfo destroyed" << std::endl;
}

// ---> Private Constructors auxiliars -------------------------------------------------- //

std::string ScriptInfo::_getValidPath()
{
	std::string path = this->_getScriptName();

	if (path.empty())
		throw std::runtime_error(ERR_SCRIPT_NAME);

	return path;
}

std::string ScriptInfo::_getScriptName()
{
	stringVector::const_iterator it = this->_environment.begin();
	for (; it != this->_environment.end(); it++)
	{
		if (it->find(SCRIPT_NAME) != std::string::npos)
			return it->substr(it->find("=") + 1);
	}
	return "";
}