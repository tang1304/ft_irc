/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:14:23 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/09 11:45:07 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

static void modeLimitUser(char c, std::string target, Client &user, Channel &chan)
{
	unsigned int		i;
	std::stringstream	ss;

	ss << target;
	ss >> i;
	if ((target.empty() && c == '+') || i * 10 / 10 != i)
	{

		sendToClient(user, ERR_INVALIDMODEPARAM(user.getName(), chan.getName(), c+"l", target));
		return ;
	}
	chan.setLimitUserOnOff(c, i);
	//sendToChan(); a mettre dans setPrivate
}

static void	modeInviteOnly(char c, std::string target, Client &user, Channel &chan)
{
	if (!target.empty())
	{
		sendToClient(user, ERR_INVALIDMODEPARAM(user.getName(), chan.getName(), c+"i", target));
		return ;
	}
	chan.setPrivated(c);
//	sendToChan(); a mettre dans setPrivate
}

static int checkInvalidKey(std::string target)
{
	for (size_t i = 0; i < target.size(); i++)
	{
		if (!isalnum(target.c_str()[i]))
			return (1);
	}
	return (0);
}

static void	modeKey(char c, std::string target, Client &user, Channel &chan)
{
	if (c == '+' && (target.empty() || target.size() > PASSMAXLEN))
	{
		sendToClient(user, ERR_INVALIDMODEPARAM(user.getName(), chan.getName(), c+"k", target));
		return ;
	}
	if (c == '+' && checkInvalidKey(target))
	{
		sendToClient(user, ERR_INVALIDMODEPARAM(user.getName(), chan.getName(), c+"k", target));
		return ;
	}
	chan.setPassword(c, target);
//	sendToChan(); a mettre dans setPrivate
}

// static void	modeOperator(char c, std::string target, Client &user, Channel &chan)
// {
// 	itVecClient	itClient = findIt(target, chan.getUsersJoin());
// 	itVecClient	itChanop = findIt(target, chan.getChanop());

// 	if (target.empty())
// 	{
// 		sendToClient(user, ERR_INVALIDMODEPARAM(user.getName(), chan.getName(), c+"o", target));
// 		return ;
// 	}
// 	if (itClient == chan.getUsersJoin().end() && itChanop == chan.getChanop().end())
// 	{
// 		sendToClient(user, ERR_USERNOTINCHANNEL(user.getName(), target, chan.getName()));
// 		return ;
// 	}
// 	if (user == *itChanop && c == '-' && itChanop != chan.getChanop().end())
// 	{
// 		sendToClient(user, ERR_CANNOTAUTODEMOTE(user.getName(), chan.getName()));
// 		return ;
// 	}
// 	if (c == '+' && itClient != chan.getUsersJoin().end())
// 	{
// 		chan.promoteDemoteUsers(c, *itClient);
// 		sendToChan(chan, RPL_USERPROMOTED(user.getName(), target));
// 		return ;
// 	}
// 	else if (c == '+' && itClient == chan.getUsersJoin().end())
// 	{
// 		sendToClient(user, ERR_USERALREADYOP(user.getName(), target, chan.getName()));
// 		return ;
// 	}
// 	if (c == '-' && itChanop != chan.getChanop().end())
// 	{
// 		chan.promoteDemoteUsers(c, *itChanop);
// 		sendToChan(chan, RPL_USERDEMOTED(user.getName(), target));
// 		return ;
// 	}
// 	else if (c == '-' && itChanop == chan.getChanop().end())
// 	{
// 		sendToClient(user, ERR_USERALREADYBASICU(user.getName(), target, chan.getName()));
// 		return ;
// 	}
// }

// static void	modeBan(char c, std::string target, Client &user, Channel &chan)
// {
// 	itVecClient	itClient = findIt(target, chan.getUsersJoin());
// 	itVecClient	itChanop = findIt(target, chan.getChanop());
// 	itVecClient	itBan;

// 	if (target.empty())
// 	{
// 		sendToClient(user, ERR_INVALIDMODEPARAM(user.getName(), chan.getName(), c+"b", target));
// 		return ;
// 	}
// 	if (itClient == chan.getUsersJoin().end() && itChanop == chan.getChanop().end() && c == '+')
// 	{
// 		sendToClient(user, ERR_USERNOTINCHANNEL(user.getName(), target, chan.getName()));
// 		return ;
// 	}
// 	if (((user == *itChanop && itChanop != chan.getChanop().end()) ||
// 		(user == *itClient && itClient != chan.getUsersJoin().end())) && c == '+')
// 	{
// 		sendToClient(user, ERR_CANNOTAUTOBAN(user.getName(), chan.getName()));
// 		return ;
// 	}
// 	if (itClient != chan.getUsersJoin().end() && c == '+')
// 		chan.addBanned(user, *itClient);
// 	else if (itClient == chan.getUsersJoin().end() && c == '+')
// 		chan.addBanned(user, *itChanop);
// 	if (c == '-')
// 	{
// 		itBan = findIt(target, chan.getBanned());
// 		if (itBan == chan.getBanned().end())
// 		{
// 			sendToClient(user, RPL_USERNOTBANNED(target, chan.getName()));
// 			return ;
// 		}
// 		chan.removeBan(user, *itBan);
// 	}
// //	sendToChan(); a mettre dans setPrivate
// }

// static void	modeTopic(char c, std::string target, Client &user, Channel &chan)
// {
// 	(void)target;

// 	chan.setChangeTopic(c, user);
// }

int	modeCmd(int fd, vecStr &cmd, Server &serv) // A verifier
{
	Client			user = serv.getClient(fd);
	itVecChan		itChan;
	modeCmds		modeList;
	itModeCmds		itCmd;
	vecModesPair	modesPair;
	size_t			j = 3;
	char 			modestring;

	// modeList['b'] = &modeBan;
	modeList['i'] = &modeInviteOnly;
	modeList['k'] = &modeKey;
	modeList['l'] = &modeLimitUser;
	// modeList['o'] = &modeOperator;
	// modeList['t'] = &modeTopic;
	if (cmd.size() < 2)
		return (sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), cmd[0])), 1);
	itChan = findIt(cmd[1], serv.getChanList());
	if (itChan == serv.getChanList().end())
		return (sendToClient(user, ERR_NOSUCHCHANNEL(user.getName(), cmd[1])), 1);
	if (cmd.size() == 2)
		return (sendToClient(user, RPL_CHANNELMODEIS(user.getName(), cmd[1], itChan->getModes())), 1);
	if (cmd.size() >= 3 && (cmd[2][0] != '+' && cmd[2][0] != '-'))
		return (sendToClient(user, RPL_CHANNELMODEIS(user.getName(), cmd[1], itChan->getModes())), 1);
	if (!isItIn(user, itChan->getChanop()))
		return (sendToClient(user, ERR_CHANOPRIVSNEEDED(user.getName(), cmd[1])), 1);
	for (size_t i = 0; i < cmd[2].size(); i++)
	{
		if (cmd[2][i] == '+' || cmd[2][i] == '-')
			modesPair.push_back(std::make_pair(cmd[2][i], ""));
		else
		{
			if (j < cmd.size())
			{
				modesPair.push_back(std::make_pair(cmd[2][i], cmd[j]));
				j++;
			}
			else
				modesPair.push_back(std::make_pair(cmd[2][i], ""));
		}
	}
	for(itVecModesPair it = modesPair.begin(); it != modesPair.end(); it++)
	{
std::cout << it->first << " " << it->second << std::endl;
		if (it->first == '+' || it->first == '-')
			modestring = it->first;
		else
		{
			itCmd = modeList.find(it->first);
			if (itCmd != modeList.end())
					itCmd->second(modestring, it->second, user, *itChan);
			else
				sendToClient(user, ERR_CMODEUNKNOWNFLAG(cmd[1]));
		}
	}




	// TEST
	for (itVecChan itc = serv.getChanList().begin(); itc != serv.getChanList().end(); itc++)
	{
		std::cout << "Chan " << itc->getName() << " created." << std::endl;
		if (!itc->getPassword().empty())
			std::cout << "Chan password " << itc->getPassword() << "." << std::endl;
		else
			std::cout << "No password set for this channel." << std::endl;
		std::cout << "Users connected " << itc->getConnected() << "." << std::endl;
		for (itVecClient ut = itc->getUsersJoin().begin(); ut != itc->getUsersJoin().end(); ut++)
			std::cout << "user " << ut->getName() << " connected." << std::endl;
		for (itVecClient ut = itc->getChanop().begin(); ut != itc->getChanop().end(); ut++)
			std::cout << "Chanop " << ut->getName() << " connected." << std::endl;
	}
// END TEST
	return (0);
}