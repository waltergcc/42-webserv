/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:15:56 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/06 00:16:50 by wcorrea-         ###   ########.fr       */
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
		if (this->_checkSpaces())
			continue;
		if (this->_checkComments())
			continue;
		if (this->_checkBrackets(token))
			return (token);
		if (this->_checkKeywords(token))
			return (token);
		token.value += this->_c;
		this->_c = this->_file.get();
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

bool FileChecker::_checkBrackets(Token &token)
{
	if (this->_c == OPEN_BRACKET_CHAR)
	{
		this->_bracket++;
		token.type = OPEN_BRACKET;
		token.value = this->_c;
		this->_c = this->_file.get();
		return true;
	}
	else if (this->_c == CLOSE_BRACKET_CHAR)
	{
		this->_bracket--;
		token.type = CLOSE_BRACKET;
		token.value = this->_c;
		this->_c = this->_file.get();
		return true;
	}
	return false;
}

static bool isValidKeyword(std::string const &s)
{
	return (s == "allow_methods" || s == "auto_index" || s == "cgi_ext" || s == "cgi_path"
		|| s == "client_max_body_size" || s == "error_page" || s == "host" || s == "index"
		|| s == "listen" || s == "location" || s == "return" || s == "root" || s == "server"
		|| s == "server_name" || s == "try_file" || s == "upload_to");
}

static std::string intToString(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

bool FileChecker::_checkKeywords(Token &token)
{
	if (std::isalpha(this->_c))
	{
		token.value += this->_c;
		this->_c = this->_file.get();
		
		while (!this->_file.eof() && (std::isalnum(this->_c) || this->_c == UNDERSCORE))
		{
			token.value += this->_c;
			this->_c = this->_file.get();
		}
		
		if (!isValidKeyword(token.value))
			throw std::runtime_error(ERR_INVALID_KEY(token.value, intToString(this->_line)));
		
		token.type = KEYWORD;
		if (token.value == "server")
			this->_hasServer = true;
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
