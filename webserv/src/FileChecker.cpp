/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:15:56 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/02 01:17:23 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileChecker.hpp"

FileChecker::FileChecker(string input)
{
	this->_checkExtension(input);
	this->_file.open(input.c_str());

	if (!this->_file.is_open())
		throw runtime_error(ERR_OPEN + input);
}

FileChecker::~FileChecker()
{
	this->_file.close();
}

void FileChecker::_checkExtension(string input)
{
	string fileName; 
	size_t 		slash = input.find_last_of("/");

	if (slash == string::npos)
		fileName = input;
	else
		fileName = input.substr(slash + 1);

	size_t dot = fileName.find_last_of(".");
	if (dot == string::npos ||	fileName.substr(dot) != ".conf" || fileName.length() <= 5)
		throw runtime_error("'" + fileName + "'" + ERR_FILE);
}
