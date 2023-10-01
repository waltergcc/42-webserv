/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:52:10 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 23:45:44 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>

#define DEFAULT_CONF "system/default.conf"

#define RED		"\033[0;31m"
#define RESET	"\033[0m"

#define ERR_ARG			"Invalid arguments"
#define ERR_OPEN		"Couldn't open file "
#define ERR_FILE		"Invalid file. File must have a name and must be .conf"
#define ERR_MANDATORY	"server_name, listen, host are mandatory fields"
