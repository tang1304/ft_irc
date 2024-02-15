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
	Client				user = serv.getClient(fd);
	std::string 		ERR;
	std::stringstream	userFd;

	if (user.getName().empty())
	{
		userFd << fd;
		userFd >> ERR;
	}
	else
		ERR = user.getName();
	if (user.getRegistered())
		return (user.setBufferSend(ERR_ALREADYREGISTERED(ERR)), 1);
	if (cmd.size() < 2)
		return (user.setBufferSend(ERR_NEEDMOREPARAMS(ERR, cmd[0])), 1);
	if (cmd[1] != serv.getPassword())
	{
		user.setDisconnect();
		return (user.setBufferSend(ERR_PASSWDMISMATCH(ERR)), 1);
	}
	user.setPass();
	return (0);
}
