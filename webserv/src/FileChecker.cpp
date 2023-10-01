/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:15:56 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 23:41:23 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileChecker.hpp"

FileChecker::FileChecker(std::string input)
{
	this->checkExtension(input);
	this->file.open(input.c_str());

	if (!this->file.is_open())
		throw std::runtime_error(ERR_OPEN + input);
}

FileChecker::~FileChecker()
{
	this->file.close();
}

void FileChecker::checkExtension(std::string input)
{
	std::string fileName; 
	size_t 		slash = input.find_last_of("/");

	if (slash == std::string::npos)
		fileName = input;
	else
		fileName = input.substr(slash + 1);

	size_t dot = fileName.find_last_of(".");
	if (dot == std::string::npos ||	fileName.substr(dot) != ".conf" || fileName.length() <= 5)
		throw std::runtime_error(ERR_FILE);
}
