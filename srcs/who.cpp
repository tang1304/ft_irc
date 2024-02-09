/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 12:00:25 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/09 16:14:06 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static void	noMask(Client &user, Server &serv){
	itVecChan	itChan;
	for (itChan = serv.getChanList().begin(); itChan != serv.getChanList().end(); itChan++){
		if (isItIn(user, itChan->getUsersJoin()) == false && \
		isItIn(user, itChan->getChanop()) == false){
			itVecClient	itClient;
			for (itClient = itChan->getChanop().begin(); itClient != itChan->getChanop().end(); itClient++){
				sendToClient(*itClient, RPL_WHOREPLY(user.getName(), itChan->getName()\
				, itClient->getUserName(), itClient->getName(), "H@", itClient->getRealName()));
			}
			for (itClient = itChan->getUsersJoin().begin(); itClient != itChan->getUsersJoin().end(); itClient++){
				sendToClient(*itClient, RPL_WHOREPLY(user.getName(), itChan->getName()\
				, itClient->getUserName(), itClient->getName(), "H", itClient->getRealName()));
			}
		}
		else
			continue ;
	}
	sendToClient(user, RPL_ENDOFWHO(user.getName(), ""));
}

int	whoCmd(int fd, vecStr &cmd, Server &serv){
	Client		user = serv.getClientMap()[fd];

	if (cmd.size() == 1){
		noMask(user, serv);
		return (0);
	}
	std::string	mask = cmd[1];
	itVecChan	itChan = findIt(mask, serv.getChanList());
	itClientMap itClient = serv.findClient(mask);
	if (itChan != serv.getChanList().end()){
		itVecClient	it;
		for (it = itChan->getChanop().begin(); it != itChan->getChanop().end(); it++){
			sendToClient(*it, RPL_WHOREPLY(user.getName(), itChan->getName()\
			, it->getUserName(), it->getName(), "H@", it->getRealName()));
		}
		for (it = itChan->getUsersJoin().begin(); it != itChan->getUsersJoin().end(); it++){
			sendToClient(*it, RPL_WHOREPLY(user.getName(), itChan->getName()\
			, it->getUserName(), it->getName(), "H", it->getRealName()));
		}
		sendToClient(user, RPL_ENDOFWHO(user.getName(), mask));
	}
	else if (itClient != serv.getClientMap().end()){
		sendToClient(user, RPL_WHOREPLY(user.getName(), mask, itClient->second.getUserName(), itClient->second.getName(),\
		"H" , itClient->second.getRealName()));
	}
	else
		sendToClient(user, ERR_NOSUCHCHANNEL(user.getName(), mask));
	return (0);
}