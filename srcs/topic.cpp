/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:28:56 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/09 09:52:07 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int	topicCmd(int fd, vecStr &cmd, Server &serv){
	Client		user = serv.getClientMap()[fd];

	if (cmd.size() < 2){
		sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), cmd[0]));
		return (1);
	}
	std::string	chan = cmd[1];
	itVecChan it = findIt(chan, serv.getChanList());
	if (it == serv.getChanList().end()){
		sendToClient(user, ERR_NOSUCHCHANNEL(user.getName(), chan));
		return (1);
	}
	bool	chanop = isItIn(user, it->getChanop());
	if (isItIn(user, it->getUsersJoin()) == false && chanop == false){
		sendToClient(user, ERR_NOTONCHANNEL(user.getName(), chan));
		return (1);
	}
	if (cmd.size() > 2){
		std::string	topic = cmd[2];
		if (it->getChangeTopic() == true && chanop == false){
			sendToClient(user, ERR_CHANOPRIVSNEEDED(user.getName(), chan));
			return (1);
		}
		it->setTopic(topic);
		it->setTopicChanger(user.getName());
		//Needs to change the time of last change
		sendToChan(*it, RPL_TOPIC(user.getName(), it->getName(), topic));
	}
	else{
		if (it->getTopic().size() > 0){
			sendToClient(user, RPL_TOPIC(user.getName(), it->getName(), it->getTopic()));
			// sendToClient(user, RPL_TOPICWHOTIME(user.getName(), it->getName(), it->getTopicChanger(), time));
		}
		else
			sendToClient(user, RPL_NOTOPIC(user.getName(), it->getName()));
	}
	return (0);
}