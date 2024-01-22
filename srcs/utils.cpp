/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:16:34 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/22 13:59:13 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "irc.hpp"

int	checkArgs(const std::string &port, const std::string &password){
	int	portValue = 0;

	if (password.empty())
		throw (std::invalid_argument("Error: the password can't be empty"));
	if (port.find_first_not_of("0123456789") != std::string::npos || port.empty() )
		throw (std::invalid_argument("Error: Wrong port format, must be a number"));
	std::istringstream	issPort(port);
	issPort >> portValue;
	if (issPort.fail() || portValue < 1024 || portValue > 65535)
		throw (std::invalid_argument("Error: Port number must be between 1024 and 65535"));
	return (portValue);
}