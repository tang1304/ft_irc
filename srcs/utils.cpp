/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:16:34 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/29 12:13:12 by rrebois          ###   ########lyon.fr   */
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

vecPair	create_pair_cmd(vecStr &cmd)
{
	vecPair		chanKey;
	vecStr		chan;
	vecStr		key;
	itVecStr	itc;
	itVecStr	itk;

	if (cmd.size() > 1)
		chan = splitCmd(cmd[1], ",");
	if (cmd.size() > 2)
		key = splitCmd(cmd[2], ",");
	itk = key.begin();
	for (itc = chan.begin(); itc != chan.end(); itc++)
	{
		if (itk != key.end())
		{
			chanKey.push_back(std::make_pair(*itc, *itk));
			itk++;
		}
		else
			chanKey.push_back(std::make_pair(*itc, ""));
	}
	return (chanKey);
}

vecStr	splitCmd(std::string &input, const std::string &delimiter){
	vecStr		result;
	size_t		colonPos = 0;
	size_t		pos = 0;
	size_t		prevPos = 0;
	std::string	tmp;
	std::string	colonStr;

	if ((colonPos = input.find(':')) != std::string::npos && (input.c_str()[colonPos - 1] == ' ')){
		colonStr = input.substr(colonPos + 1, input.find("\r\n") - colonPos);
		input.erase(colonPos - 1);
	}
	while ((pos = input.find(delimiter, prevPos)) != std::string::npos){
		tmp = input.substr(prevPos, pos - prevPos);
		if (!tmp.empty())
			result.push_back(tmp);
		prevPos = pos + 1;
	}
	tmp = input.substr(prevPos, input.find("\r\n") - prevPos);
	if (!tmp.empty())
		result.push_back(tmp);
	if (colonStr.size() > 0)
		result.push_back(colonStr);
// vecStr::iterator it = result.begin();
// while (it != result.end()){
// std::cout << *it << "." << std::endl;
// it++;
// }
	return (result);
}
