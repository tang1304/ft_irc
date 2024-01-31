/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:16:34 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/31 16:05:03 by tgellon          ###   ########lyon.fr   */
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

vecStr	splitCmds(std::string &input, const std::string &delimiter){
	vecStr		result;
	size_t		pos = 0;
	size_t		prevPos = 0;
	std::string	tmp;

	while ((pos = input.find(delimiter, prevPos)) != std::string::npos && pos != input.size() - 2){
		tmp = input.substr(prevPos, pos - prevPos);
		if (!tmp.empty())
			result.push_back(tmp);
		prevPos = pos + 2;
		tmp.clear();
	}
	tmp = input.substr(prevPos, input.size() - prevPos - 2);
	if (!tmp.empty())
		result.push_back(tmp);
vecStr::iterator it = result.begin();
for (; it != result.end(); it++){
std::cout << BLUE << "Cmd: " << *it << ". Size: " << it->size() << DEFAULT << std::endl;
	}
	return (result);
}

vecVecStr	splitCmd(vecStr &cmds, const std::string &delimiter){
	vecVecStr	result;
	size_t		colonPos = 0;
	size_t		pos = 0;
	size_t		prevPos = 0;
	vecStr		cmd;
	std::string	tmp;
	std::string	colonStr;

	itVecStr	it = cmds.begin();
	for (; it != cmds.end(); it++)
	{
		if ((colonPos = it->find(':')) != std::string::npos && (it->c_str()[colonPos - 1] == ' ')){
			colonStr = it->substr(colonPos + 1, it->find("\r\n") - colonPos);
			it->erase(colonPos - 1);
		}
		while ((pos = it->find(delimiter, prevPos)) != std::string::npos){
			tmp = it->substr(prevPos, pos - prevPos);
			if (!tmp.empty())
				cmd.push_back(tmp);
			prevPos = pos + 1;
		}
		tmp = it->substr(prevPos, it->find("\r\n") - prevPos);
		if (!tmp.empty())
			cmd.push_back(tmp);
		if (colonStr.size() > 0)
			cmd.push_back(colonStr);
		result.push_back(cmd);
	}
	return (result);
}