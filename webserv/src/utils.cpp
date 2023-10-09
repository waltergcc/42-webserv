/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:38:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/09 15:54:57 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

//	---> String Utils --------------------------------------------------------

std::string intToString(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

stringVector getStringTokens(std::string const &s, char c)
{
	stringVector tokens;
	std::stringstream ss(s);
	std::string token;

	while (std::getline(ss, token, c))
	{
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
}

void stringTrim(std::string &s, char const *set)
{
	s.erase(0, s.find_first_not_of(set));
	s.erase(s.find_last_not_of(set) + 1);
}

// ---> Boolean checkers -----------------------------------------------------

bool isValidKeyword(std::string const &s)
{
	return (s == ALLOW_M|| s == AUTOID || s == CGI_E|| s == CGI_P || s == MAX_SIZE 
		|| s == ERROR_P || s == HOST || s == INDEX || s == LISTEN || s == LOCATION
		|| s == RETURN || s == ROOT || s == SERVER || s == SERVER_N || s == TRY || s == UPLOAD);
}
