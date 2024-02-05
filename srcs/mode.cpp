/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:14:23 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/05 17:23:16 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

static int	modeInviteOnly(char c, Client &user, Channel &chan)
{
	(void)user;

	chan.setPrivated(c);
	return (0);
}

int	mode_cmd(int fd, vecStr &cmd, Server &serv) // A verifier
{
	Client		user = serv.getClient(fd);
	itVecChan	itChan;
	modeCmds	modeList;
	itModeCmds	it;

	modeList['i'] = &modeInviteOnly;
//	_modeList['t'] = &modeTopic;
//	_modeList['k'] = &modeKey;
//	_modeList['o'] = &modeOperator;
//	_modeList['l'] = &modeLimitUser;
	for (itChan = serv.getChanList().begin(); itChan != serv.getChanList().end(); itChan++)
		if (itChan->getName() == cmd[1])
			break ;
	if (itChan == serv.getChanList().end())
		return (serv.getClientMap()[fd].setBufferSend(ERR_NOSUCHCHANNEL(user.getNickName(), cmd[1])), 1);
	if ((cmd.size() >= 3 && (cmd[2][0] != '+' && cmd[2][0] != '-')) || cmd.size() < 3)
		return (sendToClient(user, RPL_CHANNELMODEIS(user.getNickName(), cmd[1])), 1);
	if (!isItIn(user, itChan->getChanop()))
		return (sendToClient(user, ERR_CHANOPRIVSNEEDED(user.getNickName(), cmd[1])), 1);

	for (size_t i = 1; i < cmd[2].size(); i++)
	{
		it = modeList.find(cmd[2][i]);
		if (it != modeList.end())
			it->second(cmd[2][0], user, *itChan);
	}
	return (0);
}