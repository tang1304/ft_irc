/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:06:38 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/15 10:06:44 by rrebois          ###   ########.fr       */
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
	for (itVecClient it = serv.getAllClients().begin(); it != serv.getAllClients().end(); it++)
		if (nick == it->getName())
			return (0);
	return (1);
}

int	nickCmd(int fd, vecStr& cmd, Server &serv)
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
	if (!user.getPass())
		return (user.setBufferSend(ERR_PASSFIRST(ERR)), 1);
	if (cmd.size() < 2)
		return (user.setBufferSend(ERR_NONICKNAMEGIVEN(ERR)), 1);
	if (!check_valid_nick(cmd[1]))
		return (user.setBufferSend(ERR_ERRONEUSNICKNAME(ERR, cmd[1])), 1);
	if (!check_password_used(cmd[1], serv))
		return (user.setBufferSend(ERR_NICKNAMEINUSE(ERR, cmd[1])), 1);
	user.setName(cmd[1]);
return (0);
}
