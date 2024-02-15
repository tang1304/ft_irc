/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 13:22:51 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/08 10:43:14 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int userCmd(int fd, vecStr &cmd, Server &serv)
{
	Client				user = serv.getClient(fd);
	std::string 		ERR;
	std::stringstream	ss;

	if (user.getName().empty())
	{
		ss << fd;
		ss >> ERR;
	}
	else
		ERR = user.getName();
	if (user.getRegistered())
		return (user.setBufferSend(ERR_ALREADYREGISTERED(ERR)), 1);
	if (!user.getPass())
		return (user.setBufferSend(ERR_PASSFIRST(ERR)), 1);
	if (user.getName().empty())
		return (user.setBufferSend(ERR_NICKFIRST(ERR)), 1);
	if (cmd.size() < 5 || cmd[1].empty())
		return (user.setBufferSend(ERR_NEEDMOREPARAMS(ERR, cmd[0])), 1);
	user.setUsername(cmd[1]);
	user.setRealName(cmd[4]);
	user.setRegistered();
	serv.registrationDone(fd);
	return (0);
}
