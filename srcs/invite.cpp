/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:22:53 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/15 10:11:45 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	inviteCmd(int fd, vecStr &cmd, Server &serv)
{
	Client		user = serv.getClient(fd);
	itVecChan	itChan;
	itVecClient	itClient;
	itVecClient	itChanop;
	itVecClient itClientList;

	if (cmd.size() < 3)
		return (serv.getClient(fd).setBufferSend(ERR_NEEDMOREPARAMS(user.getName(), cmd[0])), 1);
	itChan = findIt(cmd[2], serv.getChanList());
	if (itChan == serv.getChanList().end())
		return (serv.getClient(fd).setBufferSend(ERR_NOSUCHCHANNEL(user.getName(), cmd[2])), 1);
	if (!isItIn(user, itChan->getUsersJoin()) && !isItIn(user, itChan->getChanop()) && !itChan->getPrivated())
		return (serv.getClient(fd).setBufferSend(ERR_NOTONCHANNEL(user.getName(), cmd[2])), 1);
	if (isItIn(user, itChan->getUsersJoin()) && itChan->getPrivated()) // a tester
	 	return (serv.getClient(fd).setBufferSend(ERR_CHANOPRIVSNEEDED(user.getName(), cmd[2])), 1);
	if (itChan->getConnected() == itChan->getLimitUser() && itChan->getLimitUserOnOff())
		return (serv.getClient(fd).setBufferSend(ERR_CHANNELISFULL(user.getName(), cmd[2])), 1);
	itClient = findIt(cmd[1], itChan->getUsersJoin());
	itChanop = findIt(cmd[1], itChan->getChanop());
	if ((itClient != itChan->getUsersJoin().end() && itClient->getName() == cmd[1]) || \
		(itChanop != itChan->getChanop().end() && itChanop->getName() == cmd[1]))
		return (serv.getClient(fd).setBufferSend(ERR_USERONCHANNEL(user.getName(), cmd[1], cmd[2])), 1);
	for (itClientList = serv.getAllClients().begin(); itClientList != serv.getAllClients().end(); itClientList++)
		if (itClientList->getName() == cmd[1])
			break ;
	if (itClientList == serv.getAllClients().end())
		return (serv.getClient(fd).setBufferSend(ERR_NOEXISTINGUSER(user.getName(), cmd[1])), 1);
	sendToChan(*itChan, RPL_INVITING(user.getName(), cmd[1], itChan->getName()));
	itChan->addUser(*itClientList);
	return (0);
}
