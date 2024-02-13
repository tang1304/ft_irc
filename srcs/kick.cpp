/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:18:54 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/13 09:13:52 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	kickCmd(int fd, vecStr &cmd, Server &serv) // les coms sont avec :comment here so reason is co;;ent here for all or not??
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
			sendToClient(*itClient, RPL_CMD(itClient->getName(), itClient->getUserName(), "KICK"\
			,itChan->getName() + " " + itClient->getName() + " " + comment));
			itChan->removeUser(*itClient);
			sendToChanNotUser(user, *itChan, RPL_USERKICKED(user.getName(), *it, itChan->getName(), comment));
		}
		else if (itChanop != itChan->getChanop().end())
		{
			sendToClient(*itChanop, RPL_COMMENTKICKED(itChanop->getName(), itChan->getName(), comment));
			itChan->removeChanop(*itChanop);
			sendToChanNotUser(user, *itChan, RPL_USERKICKED(user.getName(), *it, itChan->getName(), comment));
		}
		i++;
		if (i == MAXKICKUSERLIMIT)
			break ;
	}
	return (0);
}