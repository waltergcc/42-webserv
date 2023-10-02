/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:15:14 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/02 01:42:49 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "defines.hpp"

class FileChecker
{
	private:
		std::ifstream	_file;
	
		FileChecker();
		void _checkExtension(std::string input);
		
	public:
		FileChecker(std::string input);
		~FileChecker();
};
