/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 09:13:54 by rrebois           #+#    #+#             */
/*   Updated: 2024/01/29 09:13:54 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

static int	check_chan_first_char(vecPair pair)
{
	bool	valid;

	valid = false;
	for (vecPair::iterator	it = pair.begin(); it != pair.end(); it++)
	{
		if (it->first[0] == '#' || it->first[0] == '&')
			valid = true;
	}
	if (valid == true)
		return (1);
	return (0);
}

static int	user_join_chan(vecPair::iterator &it, Server &serv, Client &user)
{
	vecChan::iterator	itc;
	vecStr::const_iterator	itCli;
	vecStr::const_iterator	itvit;

	for (itc = serv.getChanList().begin(); itc != serv.getChanList().end(); itc++)
	{
		if (it->first == itc->getName())
			break ;
	}
	if (it->second != itc->getPassword())
		return (user.setBufferSend(ERR_BADCHANNELKEY(user.getNickName(), it->first)), 0);
	for (itCli = itc->getBanned().begin(); itCli != itc->getBanned().end(); itCli++)
	{
		if (user.getNickName() == *itCli)
			return (user.setBufferSend(ERR_BANNEDFROMCHAN(user.getNickName(), it->first)), 0);
	}
	if (itc->getConnected() == CHANMAXUSER && itc->getLimitUser())
		return (user.setBufferSend(ERR_CHANNELISFULL(user.getNickName(), it->first)), 0);
	if (itc->getPrivated())
	{
		for (itvit = itc->getInvited().begin(); itvit != itc->getInvited().end(); itvit++)
		{
			if (*itvit == user.getNickName())
				itc->addUser(user); // + replies
		}
		return (user.setBufferSend(ERR_INVITEONLYCHAN(user.getNickName(), it->first)), 0);
	}
	itc->addUser(user); // + replies
	return (1);
}

static int check_chan_name(std::string name)
{
	for (size_t i = 0; i < name.size(); i++)
	{
		if (name.c_str()[i] == 7 || name.c_str()[i] == ' ' || \
			(name.c_str()[0] != '#' && name.c_str()[0] != '&'))
			return (0);
	}
	return (1);
}

static int	user_create_chan(vecPair::iterator &it, Server &serv, Client &user)
{
	if (!check_chan_name(it->first))
		return (user.setBufferSend(ERR_BADCHANMASK(it->first)), 0); // send here?
	if (user.getChanCount() == USERCHANLIMIT)
		return (user.setBufferSend(ERR_TOOMANYCHANNELS(user.getNickName(), it->first)), 0);
	serv.addChan(it->first, it->second, user);
	return (1);
}

int	join_cmd(int fd, vecStr &cmd, Server &serv)
{// regarder si user registered or not!!!
	Client	user;
	vecPair	chanPass;
	bool	exists;

	chanPass = create_pair_cmd(cmd);
	user = serv.getClient(fd);

// TEST
	// for (vecPair::iterator it = chanPass.begin(); it != chanPass.end(); it++)
	// {
	// 	std::cout << "Chan: " << it->first << " ";
	// 	if (it->second.empty())
	// 		std::cout << std::endl;
	// 	else
	// 		std::cout << "mdp: " << it->second << std::endl;
	// }
// END TEST
	if (!check_chan_first_char(chanPass) && cmd.size() < 3)
		return (serv.getClientMap()[fd].setBufferSend(ERR_NEEDMOREPARAMS(user.getNickName(), cmd[1])), 1);
	for (vecPair::iterator it = chanPass.begin(); it != chanPass.end(); it++)
	{
		exists = false;
		for (vecChan::const_iterator itc = serv.getChanList().begin(); itc != serv.getChanList().end(); itc++)
		{
			if (it->first == itc->getName())
				exists = true;
		}
		if (exists)
			user_join_chan(it, serv, user);
		else
			user_create_chan(it, serv, user);
	}

	// TEST
for (vecChan::const_iterator itc = serv.getChanList().begin(); itc != serv.getChanList().end(); itc++)
{
	std::cout << "Chan " << itc->getName() << " created." << std::endl;
	if (!itc->getPassword().empty())
		std::cout << "Chan password " << itc->getPassword() << "." << std::endl;
	else
		std::cout << "No password set for this channel." << std::endl;
	std::cout << "Users connected " << itc->getConnected() << "." << std::endl;
	for (vecCli::const_iterator ut = itc->getUsersJoin().begin(); ut != itc->getUsersJoin().end(); ut++)
		std::cout << "user " << ut->getNickName() << " connected." << std::endl;
	for (vecCli::const_iterator ut = itc->getChanop().begin(); ut != itc->getChanop().end(); ut++)
		std::cout << "Chanop " << ut->getNickName() << " connected." << std::endl;
}
// END TEST

	// for (vecPair::iterator it = chanPass.begin(); it != chanPass.end(); it++)
	return (0);
}
