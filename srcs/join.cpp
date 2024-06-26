/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 09:13:54 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/22 17:20:16 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static int	check_chan_first_char(vecPair pair)
{
	bool	valid;

	valid = false;
	for (itVecPair it = pair.begin(); it != pair.end(); it++)
	{
		if (it->first[0] == '#' || it->first[0] == '&')
			valid = true;
	}
	return (valid);
}

static void	user_join_chan(itVecPair &it, Server &serv, Client &user)
{
	itVecChan	itc;
	itVecClient	itInvited;
	itVecClient	itClient;
	itVecClient	itChanop;
	std::string msg;

	itc = findIt(it->first, serv.getChanList());
	itInvited = findIt(user, itc->getInvited());
	if (it->second != itc->getPassword() && itInvited == itc->getInvited().end() &&
		itc->getModes().find("k") != std::string::npos)
	{
		sendToClient(user, ERR_BADCHANNELKEY(user.getName(), it->first));
		return ;
	}
	if (itc->getConnected() == itc->getLimitUser() && itc->getLimitUserOnOff())
	{
		sendToClient(user, ERR_CHANNELISFULL(user.getName(), it->first));
		return ;
	}
	if (itc->getPrivated() && itInvited == itc->getInvited().end())
	{
		sendToClient(user, ERR_INVITEONLYCHAN(user.getName(), it->first));
		return ;
	}
	itClient = findIt(user.getName(), itc->getUsersJoin());
	itChanop = findIt(user.getName(), itc->getChanop());
	if (itClient == itc->getUsersJoin().end() && itChanop == itc->getChanop().end())
  	{
		std::string			allUsers;
	 	std::string			timestamp;
	  	std::stringstream	ss;

	 	ss << itc->getTimeTopicChange();
	  	ss >> timestamp;
		itc->addUser(user);
		sendToChan(*itc, RPL_CMD(user.getName(), user.getUserName(), "JOIN", itc->getName()));

		if (itc->getTopic().size() > 0)
		{
			sendToClient(user, RPL_TOPIC(user.getName(), itc->getName(), itc->getTopic()));
			sendToClient(user, RPL_TOPICWHOTIME(user.getName(), itc->getName(), itc->getTopicChanger(),
							timestamp, std::asctime(std::localtime(&itc->getTimeTopicChange()))));
		}
		for (itVecClient it = itc->getUsersJoin().begin(); it != itc->getUsersJoin().end(); it++)
			allUsers += " " + it->getName();
		for (itVecClient it = itc->getChanop().begin(); it != itc->getChanop().end(); it++)
			allUsers += " @" + it->getName();
		sendToClient(user, RPL_NAMREPLY(user.getName(), itc->getName(), allUsers));
		sendToClient(user, RPL_ENDOFNAMES(user.getName(), itc->getName()));
		if (itInvited != itc->getInvited().end())
			itc->removeInvited(*itInvited);
	}
	else
	{
		msg = "you are already in the channel " + itc->getName();
		sendToClient(user, ERROR(msg));
	}
}

static int check_chan_name(std::string name)
{
	if (name.size() == 1)
		return (0);
	for (size_t i = 0; i < name.size(); i++)
	{
		if (name.c_str()[0] != '#' && name.c_str()[0] != '&')
			return (0);
		if (name.c_str()[i] == 7 || name.c_str()[i] == ' ')
			return (0);
	}
	return (1);
}

static void	user_create_chan(itVecPair &it, Server &serv, Client &user)
{
	if (!check_chan_name(it->first))
	{
		sendToClient(user, ERR_BADCHANMASK(it->first));
		return ;
	}
	if (user.getChanCount() == CHANUSERCANJOIN)
	{
		sendToClient(user, ERR_TOOMANYCHANNELS(user.getName(), it->first));
		return ;
	}
	serv.addChan(it->first, it->second, user);
	Channel	chan = serv.getChanList().back();
	std::string	userName = "@" + user.getName();
	sendToClient(user, RPL_NAMREPLY(user.getName(), chan.getName(), userName));
	sendToClient(user, RPL_ENDOFNAMES(user.getName(), chan.getName()));
}

int	joinCmd(int fd, vecStr &cmd, Server &serv)
{
	Client	user;
	vecPair	chanPass;
	bool	exists;

	chanPass = create_pair_cmd(cmd);
	user = serv.getClient(fd);
	if (!check_chan_first_char(chanPass) && cmd.size() < 3)
	{
		sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), cmd[1]));
		return (1);
	}
	for (itVecPair it = chanPass.begin(); it != chanPass.end(); it++)
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
	return (0);
}
