/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:38:24 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/23 16:38:19 by wcorrea-         ###   ########.fr       */
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
std::string		getCorrectPath(std::string const &root, std::string const &resource);
bool			isItSufix(std::string const &s, std::string const &sufix);

// ---> Time Utils ----------------------------------------------------------

std::string		getTime();
std::string		getTimeStamp();

// ---> File Utils ----------------------------------------------------------

bool			isReadbleFile(std::string const &path);
bool			hasThisExtension(std::string const &file, std::string const &extension);
bool			directoryExists(std::string const &path);
std::string		getFileContent(std::string const &path);
std::string		getFileType(std::string const &file);
std::string		generateResponseOK(std::string const &path);

// ---> Display Utils -----------------------------------------------------------

void			printInfo(std::string const &s, std::string const &color);

// ---> Signal Utils -----------------------------------------------------------

void			signalHandler(int signum);
