/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:06:38 by rrebois           #+#    #+#             */
/*   Updated: 2024/01/24 16:29:57 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

void	nick_cmd(int fd, vecstr& cmd, Server& serv)
{
	std::string 		ERR;
	std::stringstream	ss;

	ss << fd;
	ss >> ERR;
	if (cmd.size() < 2)
	{
		serv.getClientMap()[fd].setBufferSend(ERR_NEEDMOREPARAMS(ERR, cmd[0]));
		return ;
	}
	if (cmd.size() > 3)
	{
		ERR += " " + cmd[0] + ERR_NEEDLESSPARAMS;
		send(fd, ERR.c_str(), len(ERR), 0);
		return ;
	}
	if (serv.getClientMap()[fd].getPass())
}