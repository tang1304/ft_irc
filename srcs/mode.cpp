/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:14:23 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/07 17:12:34 by rrebois          ###   ########.fr       */
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

//static void	modeOperator(char c, std::string target, Client &user, Channel &chan)
//{//le chan existe et user is chanop in that chan
//	itVecClient	itClient = findIt(target, chan.getUsersJoin());
//	itVecClient	itChanop = findIt(target, chan.getChanop());
//	itVecClient
//
//	if (itClient == chan.getUsersJoin().end() && itChanop == chan.getChanop().end())
//		sendToClient(user, ERR_NOTONCHANNEL(target, chan.getName()));
//	if ((c == '+' && itChanop != chan.getChanop().end()) || (c == '-' && itClient != chan.getUsersJoin().end()))
//		return ;
//	if (c == '+')
//	{
//		addChanop()
//	}
////	sendToChan(); a mettre dans setPrivate
//}

int	mode_cmd(int fd, vecStr &cmd, Server &serv) // A verifier
{
	Client			user = serv.getClient(fd);
	itVecChan		itChan;
	modeCmds		modeList;
	itModeCmds		itCmd;
	vecModesPair	modesPair;
	size_t			j = 3;
	char 			modestring;

	modeList['i'] = &modeInviteOnly;
//	_modeList['t'] = &modeTopic;
	modeList['k'] = &modeKey;
//	modeList['o'] = &modeOperator;
	modeList['l'] = &modeLimitUser;
	//modeList['b'] = &modeBan;
	if (cmd.size() < 3)
	{
		if (cmd.size() > 2)
			return (sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), cmd[1])), 1);
		else
			return (sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), "")), 1);
	}
	itChan = findIt(cmd[1], serv.getChanList());
	if (itChan == serv.getChanList().end())
		return (sendToClient(user, ERR_NOSUCHCHANNEL(user.getName(), cmd[1])), 1);
	if (cmd.size() >= 3 && (cmd[2][0] != '+' && cmd[2][0] != '-'))
		return (sendToClient(user, RPL_CHANNELMODEIS(user.getName(), cmd[1])), 1);
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
			if (itCmd != modeList.end() || it->first == 'o')
			{
				if (it->first == 'o')
				{
					itClientMap	itCM;
					for (itCM = serv.getClientMap().begin(); itCM != serv.getClientMap().end(); itCM++)
					{
						if (it->second == itCM->second.getName())
							break ;
					}
					if (itCM == serv.getClientMap().end())
						sendToClient(user, ERR_NOEXISTINGUSER(user.getName(), it->second));
					else
					{
						itVecClient	itClient = findIt(it->second, itChan->getUsersJoin());
						itVecClient	itChanop = findIt(it->second, itChan->getChanop());

						if (itClient == itChan->getUsersJoin().end() && itChanop == itChan->getChanop().end())
							sendToClient(user, ERR_NOTONCHANNEL(it->second, itChan->getName()));
						itChan->promoteDemoteUsers(modestring, user, itCM->second);
//						if (modestring == '+' && itClient != itChan->getUsersJoin().end())
//						{
//							itChan->addChanop(itCM->second);
//							itChan->removeUser(itCM->second);
//						}
//						else if (modestring == '-' && itChanop != itChan->getChanop().end())
//						{
//							itChan->addUser(itCM->second);
//							itChan->removeChanop(itCM->second);
//						}
					}
				}
				else
					itCmd->second(modestring, it->second, user, *itChan);
			}
			else
				sendToClient(user, ERR_CMODEUNKNOWNFLAG(cmd[1]));
		}
	}
	return (0);
}