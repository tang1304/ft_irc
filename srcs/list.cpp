/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:31:31 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/15 10:13:09 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int			listCmd(int fd, vecStr &cmd, Server &serv)
{
	Client				user = serv.getClient(fd);
	itVecChan			itChan;
	vecStr 				channels;
	std::stringstream	ss;
	std::string 		count;
	std::string 		topic;

	if (cmd.size() == 1)
	{
		for (itChan = serv.getChanList().begin(); itChan != serv.getChanList().end(); itChan++)
		{
			ss << itChan->getConnected();
			ss >> count;
			if (itChan->getTopic().empty())
				topic = "No topic is set";
			else
				topic = itChan->getTopic();
			sendToClient(user, RPL_LIST(user.getName(), itChan->getName(), \
                                        count, topic));
		}
		sendToClient(user, RPL_LISTEND(user.getName()));
		return (0);
	}
	channels = split(cmd[1], ",");
	for (itVecStr it = channels.begin(); it != channels.end(); it++)
	{
		itChan = findIt(*it, serv.getChanList());
		if (itChan != serv.getChanList().end())
		{
			ss << itChan->getConnected();
			ss >> count;
			if (itChan->getTopic().empty())
				topic = "No topic is set";
			else
				topic = itChan->getTopic();
			sendToClient(user, RPL_LIST(user.getName(), itChan->getName(), \
										count, topic));
		}
	}
	sendToClient(user, RPL_LISTEND(user.getName()));
	return (0);
}