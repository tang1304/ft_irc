/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 13:22:51 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/19 11:20:53 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int userCmd(int fd, vecStr &cmd, Server &serv)
{
	std::string 		msg;
	std::string 		ERR;
	std::stringstream	ss;

	if (serv.getClientMap()[fd].getName().empty())
	{
		ss << fd;
		ss >> ERR;
	}
	else
		ERR = serv.getClientMap()[fd].getName();
	if (serv.getClientMap()[fd].getRegistered())
		return (serv.getClientMap()[fd].setBufferSend(ERR_ALREADYREGISTERED(ERR)), 1);
	if (!serv.getClientMap()[fd].getPass())
	{
		msg = "must confirm password first";
		return (serv.getClientMap()[fd].setBufferSend(ERROR(msg)), 1);
	}
	// if (serv.getClientMap()[fd].getName().empty())
	// {
	// 	msg = "must set nickname first";
	// 	return (serv.getClientMap()[fd].setBufferSend(ERROR(msg)), 1);
	// }
	if (cmd.size() < 5 || cmd[1].empty())
		return (serv.getClientMap()[fd].setBufferSend(ERR_NEEDMOREPARAMS(ERR, cmd[0])), 1);
	serv.getClientMap()[fd].setUsername(cmd[1]);
	serv.getClientMap()[fd].setRealName(cmd[4]);
	if (serv.getClientMap()[fd].getName() != ""){
		serv.getClientMap()[fd].setRegistered();
		serv.registrationDone(fd);
	}
	return (0);
}
