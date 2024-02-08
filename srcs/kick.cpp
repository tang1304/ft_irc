/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 16:18:54 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/08 18:16:29 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	kickCmd(int fd, vecStr &cmd, Server &serv) // les coms sont avec :comment here so reason is co;;ent here for all or not??
{//segf if KICK #abc t,y "Horrible person" RPLS seem ok but parsong od comment needs more check.
	Client		user = serv.getClient(fd);

	itVecChan	itChan;
	itVecClient itClient;
	itVecClient	itChanop;
	vecStr 		namesSplits;
	vecPair 	kickUser;
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
	namesSplits = split(cmd[2], ",");
	for (itVecStr it = namesSplits.begin(); it != namesSplits.end(); it++)
	{
		if (j < cmd.size())
		{
			kickUser.push_back(std::make_pair(*it, cmd[j]));
			j++;
			i++;
		}
		else
		{
			kickUser.push_back(std::make_pair(*it, STDKICKMSG));
			i++;
		}
		if (i == 3)
			break ;
	}
	for (itVecPair it = kickUser.begin(); it != kickUser.end(); it++)
	{
		itClient = findIt(it->first, itChan->getUsersJoin());
		itChanop = findIt(it->first, itChan->getChanop());
		if (itClient == itChan->getUsersJoin().end() && itChanop == itChan->getChanop().end())
		{
			sendToClient(user, ERR_USERNOTINCHANNEL(user.getName(), it->first, itChan->getName()));
			kickUser.erase(it);
			it--;
		}
	}
	itChanop = findIt(user.getName(), itChan->getChanop());
	if (itChanop == itChan->getChanop().end())
		return (sendToClient(user, ERR_NOTONCHANNEL(user.getName(), itChan->getName())), 1);
	for (itVecPair it = kickUser.begin(); it != kickUser.end(); it++)
	{
		std::cout << it->first << ": " << it->second << std::endl;
		itClient = findIt(it->first, itChan->getUsersJoin());
		itChanop = findIt(it->first, itChan->getChanop());
		if (itClient != itChan->getUsersJoin().end())
		{
			sendToClient(*itClient, RPL_COMMENTKICKED(itClient->getName(), itChan->getName(), it->second));
			itChan->removeUser(*itClient);
		}
		else
		{
			sendToClient(*itChanop, RPL_COMMENTKICKED(itChanop->getName(), itChan->getName(), it->second));
			itChan->removeChanop(*itChanop);
//			sendToChan(*itChan, RPL_USERKICKED(user.getName(), itChanop->getName(), itChan->getName(), it->second));
		}
		sendToChan(*itChan, RPL_USERKICKED(user.getName(), it->first, itChan->getName(), it->second));
	}
	return (0);
}