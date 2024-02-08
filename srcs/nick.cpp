/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:06:38 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/08 10:41:39 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

static int check_valid_nick(std::string nick)
{
	if (nick.size() > NICKLEN || nick.empty())
		return (0);
	for (size_t i = 0; i < nick.size(); i++)
	{
		if (isdigit(nick[i]) && i == 0)
			return (0);
		if (nick[i] < 48 || (nick[i] > 57 && nick[i] < 65) ||
			(nick[i] > 93 && nick[i] < 97) || nick[i] >125)
			return (0);
	}
	return (1);
}

static int check_password_used(std::string nick, Server &serv)
{
	clientMap	cpy;
	itClientMap	it;

	cpy = serv.getClientMap();
	for (it = cpy.begin(); it != cpy.end(); it++)
		if (nick == it->second.getName())
			return (0);
	return (1);
}

int	nick_cmd(int fd, vecStr& cmd, Server &serv)
{
	std::string 		ERR;
	std::stringstream	ss;

	if (serv.getClientMap()[fd].getName().empty())
	{
		ss << fd;
		ss >> ERR;
	}
	else
		ERR = serv.getClientMap()[fd].getName();
	if (!serv.getClientMap()[fd].getPass())
		return (serv.getClientMap()[fd].setBufferSend(ERR_PASSFIRST(ERR)), 1);
	if (cmd.size() < 2)
		return (serv.getClientMap()[fd].setBufferSend(ERR_NONICKNAMEGIVEN(ERR)), 1);
	if (!check_valid_nick(cmd[1]))
		return (serv.getClientMap()[fd].setBufferSend(ERR_ERRONEUSNICKNAME(ERR, cmd[1])), 1);
	if (!check_password_used(cmd[1], serv))
		return (serv.getClientMap()[fd].setBufferSend(ERR_NICKNAMEINUSE(ERR, cmd[1])), 1);
	serv.getClientMap()[fd].setName(cmd[1]);
return (0);
}
