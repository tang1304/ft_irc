/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:22:53 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/19 15:23:28 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int	inviteCmd(int fd, vecStr &cmd, Server &serv)
{
	Client		user = serv.getClient(fd);
	itVecChan	itChan;
	itVecClient	itClient;
	itVecClient	itChanop;
	itClientMap itClientList;
	std::string msg;

	if (cmd.size() < 3)
		return (serv.getClientMap()[fd].setBufferSend(ERR_NEEDMOREPARAMS(user.getName(), cmd[0])), 1);
	itChan = findIt(cmd[2], serv.getChanList());
	if (itChan == serv.getChanList().end())
		return (serv.getClientMap()[fd].setBufferSend(ERR_NOSUCHCHANNEL(user.getName(), cmd[2])), 1);
	if (!isItIn(user, itChan->getUsersJoin()) && !isItIn(user, itChan->getChanop()) && !itChan->getPrivated())
		return (serv.getClientMap()[fd].setBufferSend(ERR_NOTONCHANNEL(user.getName(), cmd[2])), 1);
	if (isItIn(user, itChan->getUsersJoin()) && itChan->getPrivated())
	 	return (serv.getClientMap()[fd].setBufferSend(ERR_CHANOPRIVSNEEDED(user.getName(), cmd[2])), 1);
	if (itChan->getConnected() == itChan->getLimitUser() && itChan->getLimitUserOnOff())
		return (serv.getClientMap()[fd].setBufferSend(ERR_CHANNELISFULL(user.getName(), cmd[2])), 1);
	itClient = findIt(cmd[1], itChan->getUsersJoin());
	itChanop = findIt(cmd[1], itChan->getChanop());
	if ((itClient != itChan->getUsersJoin().end() && itClient->getName() == cmd[1]) || \
		(itChanop != itChan->getChanop().end() && itChanop->getName() == cmd[1]))
		return (serv.getClientMap()[fd].setBufferSend(ERR_USERONCHANNEL(user.getName(), cmd[1], cmd[2])), 1);
	for (itClientList = serv.getClientMap().begin(); itClientList != serv.getClientMap().end(); itClientList++)
		if (itClientList->second.getName() == cmd[1])
			break ;
	if (itClientList == serv.getClientMap().end())
	{
		msg = "user " + cmd[1] + " does not exist on this server";
		return (serv.getClientMap()[fd].setBufferSend(ERROR(msg)), 1);
	}
	if (!itChan->setInvited(itClientList->second))
	{
		sendToChan(*itChan, RPL_INVITING(user.getName(), itChan->getName(), cmd[1]));
		msg = itClientList->second.getName() + " " + itChan->getName();
		sendToClient(itClientList->second, RPL_CMD(user.getName(), user.getUserName(),
												   cmd[0], msg));
	}
	else
	{
		msg = itClientList->second.getName() + " " + itChan->getName();
		sendToClient(user, RPL_CMD(user.getName(), user.getUserName(),
								   cmd[0], msg));
	}
	return (0);
}
