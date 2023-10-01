/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:52:10 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 16:12:32 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define DEFAULT_CONF "system/default.conf"

#define RED		"\033[0;31m"
#define RESET	"\033[0m"

#define ERR_ARG		"Invalid arguments"
#define ERR_OPEN	"Couldn't open file "
#define ERR_MANDATORY	"the following fields are mandatory in config file: server_name, listen, host"
