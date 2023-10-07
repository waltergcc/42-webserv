/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 14:35:16 by wcorrea-          #+#    #+#             */
/*   Updated: 2023/10/01 16:09:38 by wcorrea-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.hpp"
#include "FileChecker.hpp"

class Service
{
	private:
		Service();

		// Parse Input private methods
		void _checkArguments(int ac, char **av);
		void _parseConfigFile(std::string input);
		void _parseServerBlock(FileChecker &file, Token &token, bool location, int bracket);
		void _parseLocationBlock(FileChecker &file, Token &token, bool &location);
		
	public:
		Service(int ac, char **av);
		~Service();
};
