/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:15:14 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/08 02:34:28 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"
#include "ServerInfo.hpp"

enum eToken
{
	KEYWORD,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	END
};
struct Token
{
	eToken		type;
	std::string	value;
};

typedef std::vector<ServerInfo> serverVector;

class FileChecker
{
	private:
		std::ifstream	_file;
		std::string		_filePath;
		size_t			_line;
		int				_bracket;
		bool			_hasServer;
		char			_c;
		Token			_token;
		stringMap		_configs;
		serverVector	_servers;

		// check arguments and extensions methods
		void _checkArguments(int ac, char **av);
		void _checkExtension();

		// getServerConfigs auxiliar methods
		void			_parseServerBlock();
		void			_parseLocationBlock(bool &location);
		locationPair	_getLocation();
		bool			_hasThis(std::string const &keyword);
		std::string		_getPathFixed(std::string const &keyword, bool isUpload);
		std::string		_getValue(std::string const &keyword);
		
		// getNextToken & its auxiliar methods
		Token getNextToken();
		bool _isNewLineMoveFoward();
		bool _isSpaceMoveFoward();
		bool _isCommentMoveFoward();
		bool _isBracket(Token &token);
		bool _isKeyword(Token &token);
		void _getConfigContent(std::string const &keyword);
		void _getLocationConfig(std::string const &keyword, std::string &content);
		void _getCommonConfig(std::string const &keyword, std::string &content);
		
		FileChecker();
		
	public:
		FileChecker(int ac, char **av);
		~FileChecker();

		serverVector getServerConfigs();
};
