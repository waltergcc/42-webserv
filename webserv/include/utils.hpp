/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:38:24 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/09 15:53:45 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"

// string utils
std::string		intToString(int n);
stringVector	getStringTokens(std::string const &s, char c);
void			stringTrim(std::string &s, char const *set);

// boolean checkers
bool isValidKeyword(std::string const &s);
