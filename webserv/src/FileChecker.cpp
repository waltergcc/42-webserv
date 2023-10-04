/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:15:56 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/04 15:50:43 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileChecker.hpp"

//	---> Constructor and destructor --------------------------------------------

FileChecker::FileChecker(std::string input)
{
	this->_checkExtension(input);
	this->_file.open(input.c_str());

	if (!this->_file.is_open())
		throw std::runtime_error(ERR_OPEN + input);
	
	this->_line = 1;
	this->_bracket = 0;
	this->_hasServer = false;
	this->_c = this->_file.get();
}

FileChecker::~FileChecker()
{
	this->_file.close();
}
int		FileChecker::getLine() const
{
	return (this->_line);
}

//	---> Public Methods --------------------------------------------------------

Token	FileChecker::getNextToken()
{
	Token	token;
	
	while (!this->_file.eof())
	{
		if (this->_checkNewLine())
			continue;
		else if (this->_checkSpaces())
			continue;
		else if (this->_checkComments())
			continue;
		else
		{
			this->_c = this->_file.get();
			continue;
		}
	}
	token.type = END;
	token.value = "";
	return (token);
}

//	---> Private Auxiliar Methods ---------------------------------------------------

bool FileChecker::_checkNewLine()
{
	if (this->_c == NEWLINE)
	{
		this->_line++;
		this->_c = this->_file.get();
		return true;
	}
	return false;
}

bool FileChecker::_checkSpaces()
{
	if (std::isspace(this->_c))
	{
		while (!this->_file.eof() && std::isspace(this->_c))
		{
			if (this->_c == NEWLINE)
				break;
			this->_c = this->_file.get();
		}
		return true;
	}
	return false;
}

bool FileChecker::_checkComments()
{
	if (this->_c == HASH)
	{
		while (!this->_file.eof() && this->_c != NEWLINE)
			this->_c = this->_file.get();
		return true;
	}
	return false;
}

void FileChecker::_checkExtension(std::string input)
{
	std::string fileName; 
	size_t 		slash = input.find_last_of("/");

	if (slash == std::string::npos)
		fileName = input;
	else
		fileName = input.substr(slash + 1);

	size_t dot = fileName.find_last_of(".");
	if (dot == std::string::npos ||	fileName.substr(dot) != ".conf" || fileName.length() <= 5)
		throw std::runtime_error("'" + fileName + "'" + ERR_FILE);
}
