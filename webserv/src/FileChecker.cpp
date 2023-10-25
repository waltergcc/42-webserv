/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:15:56 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/25 10:14:37 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileChecker.hpp"

//	---> Constructor and destructor --------------------------------------------

FileChecker::FileChecker(int ac, char **av)
{
	this->_checkArguments(ac, av);
	this->_checkExtension();
	this->_file.open(this->_filePath.c_str());

	if (!this->_file.is_open())
		throw std::runtime_error(ERR_OPEN + this->_filePath);
	
	this->_line = 1;
	this->_bracket = 0;
	this->_hasServer = false;
	this->_c = this->_file.get();
	this->_token = this->getNextToken();
}

FileChecker::~FileChecker()
{
	this->_file.close();
}

//	---> Public Methods --------------------------------------------------------

serverVector FileChecker::getServerConfigs()
{
	while (this->_token.type != END)
	{
		this->_parseServerBlock();
		this->_token = this->getNextToken();
	}
	return (this->_servers);
}

//	---> Private getServerConfigs auxiliar methods -----------------------------

void FileChecker::_parseServerBlock()
{
	std::string previous;
	bool		hasLocation = false;
	int			bracket = 0;

	if (this->_token.value != SERVER)
		throw std::runtime_error(ERR_BLOCK_START(this->_token.value, intToString(this->_line)));
	
	this->_configs.clear();
	while (true)
	{
		previous = this->_token.value;

		if (this->_token.value == SERVER)
			this->_token = this->getNextToken();
		
		if (previous == SERVER && this->_token.type != OPEN_BRACKET)
			throw std::runtime_error(ERR_SERVER_BLOCK);

		if (this->_token.type == OPEN_BRACKET)
			bracket++;
		else if (this->_token.type == CLOSE_BRACKET)
			bracket--;

		if (this->_token.value == LOCATION)
			this->_parseLocationBlock(hasLocation);
		else if (bracket == 0 && !hasLocation)
		{
			this->_servers.push_back(Server(this->_configs, this->_servers));
			return ;
		}
		else if (bracket == 0)
			return;

		this->_token = this->getNextToken();
	}
}

void FileChecker::_parseLocationBlock(bool &hasLocation)
{
	std::string tmp = this->_configs[LOCATION];

	if (!hasLocation)
	{
		hasLocation = true;
		this->_servers.push_back(Server(this->_configs, this->_servers));
		this->_configs.clear();
		this->_configs[LOCATION] = tmp;
	}

	this->_token = this->getNextToken();
	while (this->_token.type != CLOSE_BRACKET)
		this->_token = this->getNextToken();
	
	this->_servers.back().addLocation(this->_getLocation());
	this->_configs.clear();
}

locationPair FileChecker::_getLocation()
{
	location_t	location;

	if (this->_hasThis(ROOT))
		location.root = getPathWithSlashAtEnd(this->_getValue(ROOT));

	if (this->_hasThis(ALLOW_M))
		location.methods = getStringTokens(this->_getValue(ALLOW_M), SPACE);
		
	if (this->_hasThis(RETURN))
		location.redirect = this->_getValue(RETURN);
		
	if (this->_hasThis(AUTOID))
		location.autoindex = this->_getValue(AUTOID) == "on" ? true : false;
	else
		location.autoindex = false;
		
	if (this->_hasThis(TRY))
		location.tryFile = this->_getValue(TRY);
		
	if (this->_hasThis(CGI_P) && this->_hasThis(CGI_E))
	{
		location.hasCGI = true;
		location.cgiPath = getPathWithSlashAtEnd(this->_getValue(CGI_P));
	}
	else
		location.hasCGI = false;
		
	if (this->_hasThis(UPLOAD))
		location.uploadTo = getPathWithoutSlashAtBegin(this->_getValue(UPLOAD));
	
	stringTrim(this->_configs[LOCATION], SPACES);
	return std::make_pair<std::string, location_t>(this->_configs[LOCATION], location);			
}

bool FileChecker::_hasThis(std::string const &keyword)
{
	return (this->_configs.find(keyword) != this->_configs.end());
}

std::string	FileChecker::_getValue(std::string const &keyword)
{
	return this->_configs.find(keyword)->second;
}

//	---> Private getNextToken & its auxiliar methods -----------------------------

Token	FileChecker::getNextToken()
{
	Token	token;
	
	while (!this->_file.eof())
	{
		if (this->_isNewLineMoveFoward())
			continue;
		if (this->_isSpaceMoveFoward())
			continue;
		if (this->_isCommentMoveFoward())
			continue;
		if (this->_isBracket(token))
			return (token);
		if (this->_isKeyword(token))
			return (token);
		token.value += this->_c;
		this->_c = this->_file.get();
	}
	
	if (this->_bracket != 0)
		throw std::runtime_error(ERR_UNCLOSED_BRACKETS);
	
	if (this->_hasServer == false)
		throw std::runtime_error(ERR_NO_SERVER_CONFIG);
	
	token.type = END;
	token.value = "";
	return (token);
}

bool FileChecker::_isNewLineMoveFoward()
{
	if (this->_c == NEWLINE)
	{
		this->_line++;
		this->_c = this->_file.get();
		return true;
	}
	return false;
}

bool FileChecker::_isSpaceMoveFoward()
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

bool FileChecker::_isCommentMoveFoward()
{
	if (this->_c == HASH)
	{
		while (!this->_file.eof() && this->_c != NEWLINE)
			this->_c = this->_file.get();
		return true;
	}
	return false;
}

bool FileChecker::_isBracket(Token &token)
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

bool FileChecker::_isKeyword(Token &token)
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
		if (token.value == SERVER)
			return (this->_hasServer = true), true;

		this->_getConfigContent(token.value);
		return true;
	}
	return false;
}

void FileChecker::_getConfigContent(std::string const &keyword)
{
	if (this->_isSpaceMoveFoward())
	{
		if (this->_c == SEMICOLON)
			throw std::runtime_error(ERR_MISSING_VALUE(keyword, intToString(this->_line)));
		
		std::string content = "";
		
		if (keyword == LOCATION)
			this->_getLocationConfig(keyword, content);
		else
			this->_getCommonConfig(keyword, content);

		this->_configs[keyword] = content;
	}
	else if (this->_c == SEMICOLON)
		throw std::runtime_error(ERR_MISSING_VALUE(keyword, intToString(this->_line)));
		
	this->_c = this->_file.get();
}

void FileChecker::_getLocationConfig(std::string const &keyword, std::string &content)
{
	while (!this->_file.eof() && this->_c != OPEN_BRACKET_CHAR)
	{
		if (this->_c == NEWLINE)
			throw std::runtime_error(ERR_MISSING_OPEN_BRACKET(intToString(this->_line)));
		
		content += this->_c;
		this->_c = this->_file.get();
		
		if (this->_c == OPEN_BRACKET_CHAR)
			this->_bracket++;

		if (content.empty() || (content.find_first_not_of(SPACES) == std::string::npos))
			throw std::runtime_error(ERR_MISSING_VALUE(keyword, intToString(this->_line)));
	}
}

void FileChecker::_getCommonConfig(std::string const &keyword, std::string &content)
{
	this->_isSpaceMoveFoward();
	while (!this->_file.eof() && this->_c != SEMICOLON)
	{
		if (this->_c == NEWLINE)
			throw std::runtime_error(ERR_SEMICOLON(intToString(this->_line)));
		if (std::isspace(this->_c) && keyword != ALLOW_M)
			throw std::runtime_error(ERR_MANY_VALUES(keyword, intToString(this->_line)));
			
		content += this->_c;
		this->_c = this->_file.get();
	}
}

//	---> Private Check arguments and extensions methods ---------------------------------

void FileChecker::_checkArguments(int ac, char **av)
{
	if (ac > 2)
		throw std::runtime_error(ERR_ARG);
	else if (ac == 2)
		this->_filePath = av[1];
	else
		this->_filePath = DEFAULT_CONF;
}

void FileChecker::_checkExtension()
{
	std::string filename; 
	size_t 		slash = this->_filePath.find_last_of("/");

	if (slash == std::string::npos)
		filename = this->_filePath;
	else
		filename = this->_filePath.substr(slash + 1);

	if (!hasThisExtension(filename, ".conf"))
		throw std::runtime_error("'" + filename + "'" + ERR_FILE);
}
