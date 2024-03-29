/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:38:24 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/11/09 17:08:20 by wcorrea-         ###   ########.fr       */
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
std::string		getPathWithoutSlashAtEnd(std::string const &path);
std::string		getPathWithoutSlashAtBegin(std::string const &path);
std::string		getCorrectPath(std::string const &root, std::string const &resource);
bool			isItSufix(std::string const &s, std::string const &sufix);
std::string		convertScriptNameToTitle(std::string const &scriptName);
std::string		getPathWithoutFilename(std::string const &path);
std::string		urlDecode(const std::string &input);


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
std::string		generateResponseWithCustomHTML(std::string const &code, std::string const &title, std::string const &body);

// ---> Display Utils -----------------------------------------------------------

void			printInfo(std::string const &s, std::string const &color);

// ---> Signal Utils -----------------------------------------------------------

void			signalHandler(int signum);
