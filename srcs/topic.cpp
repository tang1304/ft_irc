/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:28:56 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/15 11:26:35 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

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
		if (topic.find_first_of(':') != 0)
			topic.insert(0, ":");
		if (it->getChangeTopic() == true && chanop == false){
			sendToClient(user, ERR_CHANOPRIVSNEEDED(user.getName(), chan));
			return (1);
		}
		it->setTopic(topic);
		it->setTopicChanger(user.getName());
	}
	else{
		std::string			timestamp;
		std::stringstream	ss;

		ss << it->getTimeTopicChange();
		ss >> timestamp;
		if (it->getTopic().size() > 0){
			sendToClient(user, RPL_TOPIC(user.getName(), it->getName(), it->getTopic()));
			sendToClient(user, RPL_TOPICWHOTIME(user.getName(), it->getName(), it->getTopicChanger(),
									timestamp, std::asctime(std::localtime(&it->getTimeTopicChange()))));
		}
		else
			sendToClient(user, RPL_NOTOPIC(user.getName(), it->getName()));
	}
	return (0);
}