/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 13:22:51 by rrebois           #+#    #+#             */
/*   Updated: 2024/01/25 14:56:00 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int user_cmd(int fd, vecstr &cmd, Server &serv)
{
	std::string 		ERR;
	std::stringstream	ss;

	if (serv.getClientMap()[fd].getNickName().empty())
	{
		ss << fd;
		ss >> ERR;
	}
	else
		ERR = serv.getClientMap()[fd].getNickName();
	if (serv.getClientMap()[fd].getRegistered())
		return (serv.getClientMap()[fd].setBufferSend(ERR_ALREADYREGISTERED(ERR)), 1);
	if (!serv.getClientMap()[fd].getPass())
		return (serv.getClientMap()[fd].setBufferSend(ERR_PASSFIRST(ERR)), 1);
	if (serv.getClientMap()[fd].getNickName().empty())
		return (serv.getClientMap()[fd].setBufferSend(ERR_NICKFIRST(ERR)), 1);
	if (cmd.size() < 4 || cmd[1].empty())
		return (serv.getClientMap()[fd].setBufferSend(ERR_NEEDMOREPARAMS(ERR, cmd[0])), 1);
	serv.getClientMap()[fd].setUsername(cmd[1]);
	serv.getClientMap()[fd].setRealName(cmd[4]);
	serv.getClientMap()[fd].setRegistered();
	return (0);
}