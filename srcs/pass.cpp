/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:49:57 by rrebois           #+#    #+#             */
/*   Updated: 2024/01/25 11:31:30 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	pass_cmd(int fd, vecstr& cmd, Server &serv)
{
	std::string 		ERR;
	std::stringstream	userFd;

	if (serv.getClientMap()[fd].getNickName().empty())
	{
		userFd << fd;
		userFd >> ERR;
	}
	else
		ERR = serv.getClientMap()[fd].getNickName();
	if (serv.getClientMap()[fd].getRegistered())
		return (serv.getClientMap()[fd].setBufferSend(ERR_ALREADYREGISTERED(ERR)), 1);
	if (cmd.size() < 2)
		return (serv.getClientMap()[fd].setBufferSend(ERR_NEEDMOREPARAMS(ERR, cmd[0])), 1);
	if (cmd[1] != serv.getPassword())
	{
		serv.getClientMap()[fd].setDisconnect();
		return (serv.getClientMap()[fd].setBufferSend(ERR_PASSWDMISMATCH(ERR)), 1);
	}
	serv.getClientMap()[fd].setPass();
	return (0);
}