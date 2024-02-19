/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:14:23 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/19 11:41:03 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

static void modeLimitUser(char c, std::string target, Client &user, Channel &chan)
{
	unsigned int		i;
	std::stringstream	ss;
	std::string 		msg;

	ss << target;
	ss >> i;
	if ((target.empty() && c == '+') || i * 10 / 10 != i)
	{

		sendToClient(user, ERR_INVALIDMODEPARAM(user.getName(), chan.getName(), c+"l", target));
		return ;
	}
	chan.setLimitUserOnOff(c, i);
	if (c == '+')
		msg = chan.getName() + " user limit set to " + target + " (" + c + "l)";
	else
		msg = chan.getName() + " user limit removed (" + c + "l)";
	sendToChan(chan, INFO(msg));
}

static void	modeInviteOnly(char c, std::string target, Client &user, Channel &chan)
{
	std::string 		msg;

	if (!target.empty())
	{
		sendToClient(user, ERR_INVALIDMODEPARAM(user.getName(), chan.getName(), c+"i", target));
		return ;
	}
	chan.setPrivated(c);
	if (c == '+')
		msg = "channel " + chan.getName() + " has become private";
	else
		msg = "channel " + chan.getName() + " is free to join";
	sendToChan(chan, INFO(msg));
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
	std::string	msg;

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
	if (c == '+')
		sendToChan(chan, RPL_CMD(user.getName(), user.getUserName(), "MODE", chan.getName() + " +k " + target));
	else
		sendToChan(chan, RPL_CMD(user.getName(), user.getUserName(), "MODE", chan.getName() + " -k "));
}

static void	modeOperator(char c, std::string target, Client &user, Channel &chan)
{
	itVecClient	itClient = findIt(target, chan.getUsersJoin());
	itVecClient	itChanop = findIt(target, chan.getChanop());
	std::string msg;

	if (target.empty())
	{
		sendToClient(user, ERR_INVALIDMODEPARAM(user.getName(), chan.getName(), c+"o", target));
		return ;
	}
	if (itClient == chan.getUsersJoin().end() && itChanop == chan.getChanop().end())
	{
		sendToClient(user, ERR_USERNOTINCHANNEL(user.getName(), target, chan.getName()));
		return ;
	}
	if (itChanop != chan.getChanop().end() && user == *itChanop && c == '-')
	{
		msg = "cannot demote yourself";
		sendToClient(user, ERROR(msg));
		return ;
	}
	if (c == '+' && itClient != chan.getUsersJoin().end())
	{
		chan.addChanop(*itClient);
		sendToChan(chan, RPL_MODE(user.getName(), user.getUserName(), chan.getName(), "+", + "o", target));
		return ;
	}
	else if (c == '+' && itClient == chan.getUsersJoin().end())
	{
		msg = "user " + target + " is already a chanop in the channel " + chan.getName()
			  + ". Cannot be promoted further";
		sendToClient(user, ERROR(msg));
		return ;
	}
	if (c == '-' && itChanop != chan.getChanop().end())
	{
		chan.removeChanop(*itChanop);
		sendToChan(chan, RPL_MODE(user.getName(), user.getUserName(), chan.getName(), "-", + "o", target));
		return ;
	}
	else if (c == '-' && itChanop == chan.getChanop().end())
	{
		msg = "user " + target + " is already a basic user in the channel " + chan.getName()
				+ ". Cannot be demoted further";
		sendToClient(user, ERROR(msg));
		return ;
	}
}

static void	modeBan(char c, std::string target, Client &user, Channel &chan)
{
	itVecClient	itClient = findIt(target, chan.getUsersJoin());
	itVecClient	itChanop = findIt(target, chan.getChanop());
	itVecClient	itBan;
	std::string msg;

	msg = user.getName() + " banned " + target + " from " + chan.getName();
	if (target.empty())
	{
		for (itBan = chan.getBanned().begin(); itBan != chan.getBanned().end(); itBan++)
			sendToClient(user, RPL_BANLIST(user.getName(), chan.getName(), itBan->getName()));
		sendToClient(user, RPL_ENDOFBANLIST(user.getName(), chan.getName()));
		return ;
	}
	if (itClient == chan.getUsersJoin().end() && itChanop == chan.getChanop().end() && c == '+')
	{
		sendToClient(user, ERR_USERNOTINCHANNEL(user.getName(), target, chan.getName()));
		return ;
	}
	if (((itChanop != chan.getChanop().end() && user == *itChanop) ||
		(itClient != chan.getUsersJoin().end() && user == *itClient)) && c == '+')
	{
		sendToClient(user, ERROR(std::string("Cannot ban yourself")));
		return ;
	}
	if (itClient != chan.getUsersJoin().end() && c == '+')
	{
		chan.addBanned(user, *itClient);
		sendToChan(chan, INFO(msg));
	}
	else if (itClient == chan.getUsersJoin().end() && c == '+')
	{
		chan.addBanned(user, *itChanop);
		sendToChan(chan, INFO(msg));
	}
	if (c == '-')
	{
		itBan = findIt(target, chan.getBanned());
		if (itBan == chan.getBanned().end())
		{
			msg = "user " + target + " not banned in the channel " + chan.getName();

			sendToClient(user, ERROR(msg));
			return ;
		}
		chan.removeBan(user, *itBan);
	}
}

static void	modeTopic(char c, std::string target, Client &user, Channel &chan)
{
	(void)target;
	std::string msg;

	chan.setChangeTopic(c, user);
	if (c == '+')
		msg = chan.getName() + " topic can only be changed by chanops (+t)";
	else
		msg = chan.getName() + " topic can be changed by any user (-t)";
	sendToChan(chan, INFO(msg));
}

int	modeCmd(int fd, vecStr &cmd, Server &serv)
{
	Client			user = serv.getClient(fd);
	itVecChan		itChan;
	modeCmds		modeList;
	itModeCmds		itCmd;
	vecModesPair	modesPair;
	size_t			j = 3;
	char 			modestring;
	std::string 	msg;

	modeList['b'] = &modeBan;
	modeList['i'] = &modeInviteOnly;
	modeList['k'] = &modeKey;
	modeList['l'] = &modeLimitUser;
	modeList['o'] = &modeOperator;
	modeList['t'] = &modeTopic;
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
		if (it->first == '+' || it->first == '-')
			modestring = it->first;
		else
		{
			itCmd = modeList.find(it->first);
			if (itCmd != modeList.end())
					itCmd->second(modestring, it->second, user, *itChan);
			else
			{
				msg = "unknown MODE flag (";
				msg += modestring;
				msg += it->first;
				msg += ")";
				sendToClient(user, ERROR(msg));
			}
		}
	}
	return (0);
}