/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:14:23 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/08 10:17:22 by rrebois          ###   ########.fr       */
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

static void	modeOperator(char c, std::string target, Client &user, Channel &chan)
{
	itVecClient	itClient = findIt(target, chan.getUsersJoin());
	itVecClient	itChanop = findIt(target, chan.getChanop());

	if (itClient == chan.getUsersJoin().end() && itChanop == chan.getChanop().end())
	{
		sendToClient(user, ERR_USERNOTINCHANNEL(user.getName(), target, chan.getName()));
		return ;
	}
	if (c == '+' && itClient != chan.getUsersJoin().end())
	{
		chan.promoteDemoteUsers(c, user, *itClient);
		return ;
	}
	else if (c == '+' && itClient == chan.getUsersJoin().end())
	{
		sendToClient(user, ERR_USERALREADYOP(user.getName(), target, chan.getName()));
		return ;
	}
	if (c == '-' && itChanop != chan.getChanop().end())
		chan.promoteDemoteUsers(c, user, *itChanop);
	if (c == '-' && itClient == chan.getChanop().end())
	{
		sendToClient(user, ERR_USERALREADYBASICU(user.getName(), target, chan.getName()));
		return ;
	}

}

//static void	modeBan(char c, std::string target, Client &user, Channel &chan)
//{
////	sendToChan(); a mettre dans setPrivate
//}
//user, server, chan, pair modestring
int	mode_cmd(int fd, vecStr &cmd, Server &serv) // A verifier
{
	Client			user = serv.getClient(fd);
	itVecChan		itChan;
	modeCmds		modeList;
	itModeCmds		itCmd;
	vecModesPair	modesPair;
	size_t			j = 3;
	char 			modestring;

//	modeList['b'] = &modeBan;
	modeList['i'] = &modeInviteOnly;
	modeList['k'] = &modeKey;
	modeList['l'] = &modeLimitUser;
	modeList['o'] = &modeOperator;
//	_modeList['t'] = &modeTopic;
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
			if (itCmd != modeList.end())
					itCmd->second(modestring, it->second, user, *itChan);
			else
				sendToClient(user, ERR_CMODEUNKNOWNFLAG(cmd[1]));
		}
	}
	return (0);
}