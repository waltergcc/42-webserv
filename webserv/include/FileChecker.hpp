/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:15:14 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/07 11:43:43 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"

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

class FileChecker
{
	private:
		std::ifstream	_file;
		std::string		_filePath;
		size_t			_line;
		int				_bracket;
		bool			_hasServer;
		char			_c;
		std::map<std::string, std::string>	_configs;

		// check arguments and extensions methods
		void _checkExtension(std::string input);
		void _checkArguments(int ac, char **av);

		// getServerConfigs auxiliar methods
		void _parseServerBlock(Token &token);
		void _parseLocationBlock(Token &token, bool &location);
		
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
		void printMapAndLines();
		
	public:
		FileChecker(int ac, char **av);
		~FileChecker();

		void getServerConfigs();
};
