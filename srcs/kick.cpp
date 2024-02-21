/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:18:54 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/21 14:25:36 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	kickCmd(int fd, vecStr &cmd, Server &serv)
{
	Client		user = serv.getClient(fd);

	itVecChan	itChan;
	itVecClient itClient;
	itVecClient	itChanop;
	vecStr 		namesSplits;
	std::string	comment;
	size_t 		j = 3;
	int 		i = 0;

	if (cmd.size() == 1)
		return (sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), "")), 1);
	else if (cmd.size() == 2)
		return (sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), cmd[1])), 1);
	itChan = findIt(cmd[1], serv.getChanList());
	if (itChan == serv.getChanList().end())
		return (sendToClient(user, ERR_NOSUCHCHANNEL(user.getName(), cmd[1])), 1);
	itClient = findIt(user.getName(), itChan->getUsersJoin());
	if (itClient != itChan->getUsersJoin().end())
		return (sendToClient(user, ERR_CHANOPRIVSNEEDED(user.getName(), cmd[1])), 1);
	itChanop = findIt(user.getName(), itChan->getChanop());
	if (itChanop == itChan->getChanop().end())
		return (sendToClient(user, ERR_NOTONCHANNEL(user.getName(), itChan->getName())), 1);
	namesSplits = split(cmd[2], ",");
	if (cmd.size() >= 4)
	{
		for (; j < cmd.size(); j++)
			comment += cmd[j];
		if (!comment.empty() && comment[0] == ':')
			comment.erase(comment.begin());
	}
	else
		comment = STDKICKMSG;
	for (itVecStr it = namesSplits.begin(); it != namesSplits.end(); it++)
	{
		itClient = findIt(*it, itChan->getUsersJoin());
		itChanop = findIt(*it, itChan->getChanop());
		if (itClient == itChan->getUsersJoin().end() && itChanop == itChan->getChanop().end())
		{
			sendToClient(user, ERR_USERNOTINCHANNEL(user.getName(), *it, itChan->getName()));
			namesSplits.erase(it);
			it--;
		}
		else if (itClient != itChan->getUsersJoin().end())
		{
			itChan->removeUser(*itClient);
			if (itChan->getConnected() == 0)
			{
				serv.removeChan(itChan->getId());
				return (0);
			}
			sendToChan(*itChan, RPL_CMD(user.getName(), user.getUserName(), cmd[0]\
			,itChan->getName() + " " + itClient->getName() + " " + comment));
		}
		else if (itChanop != itChan->getChanop().end())
		{
			if (itChanop->getName() == user.getName()){
				sendToClient(user, ERROR(std::string("Cannot kick yourself")));
			}
			else
			{
				sendToChan(*itChan, RPL_CMD(user.getName(), user.getUserName(), cmd[0]\
				, itChan->getName() + " " + itChanop->getName() + " " + comment));
				itChan->removeChanop(*itChanop);
				if (itChan->getConnected() == 0)
				{
					serv.removeChan(itChan->getId());
					return (0);
				}
			}
		}
		i++;
		if (i == MAXKICKUSERLIMIT)
			break ;
	}
	return (0);
}
