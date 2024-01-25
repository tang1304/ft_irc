/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:49:57 by rrebois           #+#    #+#             */
/*   Updated: 2024/01/25 10:50:48 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

void	pass_cmd(int fd, vecstr& cmd, Server& serv)
{
	std::string 		ERR;
	std::stringstream	userFd;
	int 				i = cmd.size();

std::cout << i << std::endl;

	userFd << fd;
	userFd >> ERR;
	if (i >= 3)
		i = 3;
	i--;
std::cout << i << std::endl;
	switch (i)
	{
		case (0):
		{
			serv.getClientMap()[fd].setBufferSend(ERR_NEEDMOREPARAMS(ERR, cmd[0]));
//			std::cout << "test: " << ERR_NEEDMOREPARAMS(ERR, cmd[0]) << std::endl;
			std::cout << "buf send: " << serv.getClientMap()[fd].getBufferSend() << std::endl;
			return ;
		}
//		case (2):
//		{
//			ERR += " " + cmd[0] + ERR_NEEDLESSPARAMS;
//			send(fd, ERR.c_str(), len(ERR), 0);
//			return ;
//		}
//		default:
//		{
//			if (cmd[1] != serv.getPassword())
//			{
//				ERR += (ERR_PASSWDMISMATCH);
//				send(fd, ERR.c_str(), len(ERR), 0);
//				serv.clientDisconnection(fd);
//				return ;
//			}
//			else if (serv.getClientMap()[fd].getRegistered())
//			{
//				ERR.empty();
//				ERR = serv.getClientMap()[fd].getNickName() + ERR_ALREADYREGISTERED;
//				send(fd, ERR.c_str(), len(ERR), 0);
//				return ;
//			}
//			else
//				serv.getClientMap()[fd].setPass();
//			return ;
//		}
	}
}