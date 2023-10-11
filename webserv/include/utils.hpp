/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:38:24 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/11 15:45:54 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"

// ---> Boolean checkers -----------------------------------------------------

bool			isValidKeyword(std::string const &s);

// ---> String Utils --------------------------------------------------------

std::string		intToString(int n);
stringVector	getStringTokens(std::string const &s, char c);
void			stringTrim(std::string &s, char const *set);
std::string		getPathWithSlashAtEnd(std::string const &path);
std::string		getPathWithoutSlashAtBegin(std::string const &path);

// ---> Time Utils ----------------------------------------------------------

std::string		getTime();
std::string		getTimeStamp();

// ---> File Utils ----------------------------------------------------------

bool			isReadbleFile(std::string const &path);
std::string		getFileContent(std::string const &path);
std::string		getFileType(std::string const &file);

// ---> Display Utils -----------------------------------------------------------

void			printInfo(std::string const &s, std::string const &color);
