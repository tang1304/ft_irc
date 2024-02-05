/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:22:53 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/05 11:46:56 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	invite_cmd(int fd, vecStr &cmd, Server &serv)
{
	Client		user = serv.getClient(fd);
	itVecChan	itChan;
	itVecClient	itClient;
	itVecClient	itChanop;
	itClientMap itClientList;

	if (cmd.size() < 3)
		return (serv.getClientMap()[fd].setBufferSend(ERR_NEEDMOREPARAMS(user.getNickName(), cmd[0])), 1);
	for (itChan = serv.getChanList().begin(); itChan != serv.getChanList().end(); itChan++)
	{
		if (itChan->getName() == cmd[2])
			break ;
	}
	if (itChan == serv.getChanList().end())
		return (serv.getClientMap()[fd].setBufferSend(ERR_NOSUCHCHANNEL(user.getNickName(), cmd[2])), 1);
	for (itClient = itChan->getUsersJoin().begin(); itClient != itChan->getUsersJoin().end(); itClient++)
	{
		if (itClient->getNickName() == user.getNickName())
			break ;
	}
	for (itChanop = itChan->getChanop().begin(); itChanop != itChan->getChanop().end(); itChanop++)
	{
		if (itChanop->getNickName() == user.getNickName())
			break ;
	}
	 if ((itClient == itChan->getUsersJoin().end() && itChanop == itChan->getChanop().end()) && !itChan->getPrivated())
	 	return (serv.getClientMap()[fd].setBufferSend(ERR_NOTONCHANNEL(user.getNickName(), cmd[2])), 1);
	 if (itClient != itChan->getUsersJoin().end() && itChan->getPrivated()) // a tester
	 	return (serv.getClientMap()[fd].setBufferSend(ERR_CHANOPRIVSNEEDED(user.getNickName(), cmd[2])), 1);
	 for (itClient = itChan->getUsersJoin().begin(); itClient != itChan->getUsersJoin().end(); itClient++)
	{
		if (itClient->getNickName() == cmd[1])
			break ;
	}
	for (itChanop = itChan->getChanop().begin(); itChanop != itChan->getChanop().end(); itChanop++)
	{
		if (itChanop->getNickName() == cmd[1])
			break ;
	}
	if ((itClient != itChan->getUsersJoin().end() && itClient->getNickName() == cmd[1]) || \
		(itChanop != itChan->getChanop().end() && itChanop->getNickName() == cmd[1]))
		return (serv.getClientMap()[fd].setBufferSend(ERR_USERONCHANNEL(user.getNickName(), cmd[1], cmd[2])), 1);
	for (itClientList = serv.getClientMap().begin(); itClientList != serv.getClientMap().end(); itClientList++)
		if (itClientList->second.getNickName() == cmd[1])
			break ;
	if (itClientList == serv.getClientMap().end())
		return (serv.getClientMap()[fd].setBufferSend(ERR_NOEXISTINGUSER(user.getNickName(), cmd[1])), 1);
	sendToChan(*itChan, RPL_INVITING(user.getNickName(), cmd[1], itChan->getName()));
	itChan->addUser(itClientList->second);
	return (0);
}
