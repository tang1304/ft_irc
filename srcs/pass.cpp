/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:49:57 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/08 10:41:49 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	passCmd(int fd, vecStr &cmd, Server &serv)
{
	std::string 		ERR;
	std::stringstream	userFd;

	if (serv.getClientMap()[fd].getName().empty())
	{
		userFd << fd;
		userFd >> ERR;
	}
	else
		ERR = serv.getClientMap()[fd].getName();
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
