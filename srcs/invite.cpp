/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:22:53 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/02 12:22:53 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	invite_cmd(int fd, vecStr &cmd, Server &serv) // Any user on chan can invite any other user? users and chanop? Only members of the channel are allowed to invite other users.
{
	Client		user = serv.getClient(fd);
	itVecChan	itChan;
	itVecClient	itClient;
	itVecClient	itChanop;

	if (cmd.size() < 3)
		return (serv.getClientMap()[fd].setBufferSend(ERR_NEEDMOREPARAMS(user.getName(), cmd[0])), 1);
	for (itChan = serv.getChanList().begin(); itChan != serv.getChanList().end(); itChan++)
	{
		if (itChan->getName() == cmd[2])
			break ;
	}
	if (itChan == serv.getChanList().end())
		return (serv.getClientMap()[fd].setBufferSend(ERR_NOSUCHCHANNEL(user.getName(), cmd[2])), 1);
	for (itClient = itChan->getUsersJoin().begin(); itClient != itChan->getUsersJoin().end(); itClient++)
	{
		if (itClient->getName() == user.getName())
			break ;
	}
	for (itChanop = itChan->getUsersJoin().begin(); itChanop != itChan->getUsersJoin().end(); itChanop++)
	{
		if (itChanop->getName() == user.getName())
			break ;
	}
	// if ((itClient == getUsersJoin().end() && itChanop == getChanop().end()) && !itChan->getPrivated())
	// 	return (serv.getClientMap()[fd].setBufferSend(ERR_NOTONCHANNEL(user.getName(), cmd[2])), 1);
	// if (itClient != getUsersJoin().end() && itChan->getPrivated())
	// 	return (serv.getClientMap()[fd].setBufferSend(ERR_CHANOPRIVSNEEDED(user.getName(), cmd[2])), 1);
	for (itClient = itChan->getUsersJoin().begin(); itClient != itChan->getUsersJoin().end(); itClient++)
	{
		if (itClient->getName() == cmd[1])
			break ;
	}
	for (itChanop = itChan->getUsersJoin().begin(); itChanop != itChan->getUsersJoin().end(); itChanop++)
	{
		if (itChanop->getName() == cmd[1])
			break ;
	}
	if (itClient->getName() == cmd[1] || itChanop->getName() == cmd[1])
		return (serv.getClientMap()[fd].setBufferSend(ERR_USERONCHANNEL(user.getName(), cmd[1], cmd[2])), 1);
	return (0);
}