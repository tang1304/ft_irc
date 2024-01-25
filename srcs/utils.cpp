/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:16:34 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/25 12:40:27 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../incs/irc.hpp"

std::size_t	len(std::string s)
{
	std::size_t i;

	for (i = 0; i < s.length(); i++)
		;
	return (i);
}

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

vecStr	splitCmd(const std::string &input, const std::string &delimiter){
	vecStr result;
	size_t pos = 0;
	size_t prevPos = 0;
	std::string tmp;

	while ((pos = input.find(delimiter, prevPos)) != std::string::npos){
		tmp = input.substr(prevPos, pos - prevPos);
		if (!tmp.empty())
			result.push_back(tmp);
		prevPos = pos + 1;
	}
	tmp = input.substr(prevPos, input.find("\r\n") - prevPos);
	if (!tmp.empty())
		result.push_back(tmp);
vecStr::iterator it = result.begin();
while (it != result.end()){
std::cout << *it << "." << std::endl;
it++;
}
	return result;
}