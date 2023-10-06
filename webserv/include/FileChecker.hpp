/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:15:14 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/06 00:52:37 by wcorrea-         ###   ########.fr       */
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
		size_t			_line;
		int				_bracket;
		bool			_hasServer;
		char			_c;

		FileChecker();
		void _checkExtension(std::string input);
		bool _checkNewLine();
		bool _checkSpaces();
		bool _checkComments();
		bool _checkBrackets(Token &token);
		bool _checkKeywords(Token &token);
		void _checkValue(Token &token);
		
	public:
		std::map<std::string, std::string>	configs;
		FileChecker(std::string input);
		~FileChecker();

		Token	getNextToken();
		int		getLine() const;
		int		getHasServer() const;
};
