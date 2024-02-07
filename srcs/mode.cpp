/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:14:23 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/07 14:30:12 by rrebois          ###   ########.fr       */
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
		sendToClient(user, ERR_INVALIDMODEARG(chan.getName(), c+"l"));
		return ;
	}
	chan.setLimitUserOnOff(c, i);
	//sendToChan(); a mettre dans setPrivate
}

static void	modeInviteOnly(char c, std::string target, Client &user, Channel &chan)
{
	if (!target.empty())
	{
		sendToClient(user, ERR_INVALIDMODEARG(chan.getName(), c+"i"));
		return ;
	}
	chan.setPrivated(c);
//	sendToChan(); a mettre dans setPrivate
}

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
//	modeList['k'] = &modeKey;
//	_modeList['o'] = &modeOperator;
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
			if (itCmd != modeList.end())
				itCmd->second(modestring, it->second, user, *itChan);
			else
				sendToClient(user, ERR_CMODEUNKNOWNFLAG(cmd[1]));
		}
	}
	return (0);
}