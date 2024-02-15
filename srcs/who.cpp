/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 12:00:25 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/15 10:13:20 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

static void	noMask(Client &user, Server &serv){
	itVecClient	itClientServ;

	for (itClientServ = serv.getAllClients().begin(); itClientServ != serv.getAllClients().end(); itClientServ++){
		if (itClientServ->getName() == user.getName())
			continue ;
		if (serv.getChanList().empty()){
			sendToClient(user, RPL_WHOREPLY(user.getName(), itClientServ->getName()\
				, itClientServ->getUserName(), itClientServ->getName(), "H", itClientServ->getRealName()));
			continue ;
		}
		bool	sent = 0;
		itVecChan itChan;
		for (itChan = serv.getChanList().begin(); itChan != serv.getChanList().end(); itChan++){
			if (isItIn(user, itChan->getUsersJoin()) == false && isItIn(user, itChan->getChanop()) == false && sent == 0){
				sendToClient(user, RPL_WHOREPLY(user.getName(), itChan->getName()\
				, itClientServ->getUserName(), itClientServ->getName(), "H", itClientServ->getRealName()));
				sent = 1;
			}
			else
				continue ;
		}
		if (sent == 0)
			sendToClient(user, RPL_WHOREPLY(user.getName(), itClientServ->getName()\
			, itClientServ->getUserName(), itClientServ->getName(), "H", itClientServ->getRealName()));
	}
	sendToClient(user, RPL_ENDOFWHO(user.getName(), ""));
}

int	whoCmd(int fd, vecStr &cmd, Server &serv){
	Client		user = serv.getAllClients()[fd];

	if (cmd.size() == 1){
		noMask(user, serv);
		return (0);
	}
	std::string	mask = cmd[1];
	itVecChan	itChan = findIt(mask, serv.getChanList());
	itVecClient itClient = findIt() serv.findClient(mask);
	if (itChan != serv.getChanList().end()){
		itVecClient	it;
		for (it = itChan->getChanop().begin(); it != itChan->getChanop().end(); it++){
			sendToClient(user, RPL_WHOREPLY(user.getName(), itChan->getName()\
			, it->getUserName(), it->getName(), "H@", it->getRealName()));
		}
		for (it = itChan->getUsersJoin().begin(); it != itChan->getUsersJoin().end(); it++){
			sendToClient(user, RPL_WHOREPLY(user.getName(), itChan->getName()\
			, it->getUserName(), it->getName(), "H", it->getRealName()));
		}
		sendToClient(user, RPL_ENDOFWHO(user.getName(), mask));
	}
	else if (itClient != serv.getAllClients().end()){
		sendToClient(user, RPL_WHOREPLY(user.getName(), mask, itClient->second.getUserName(), \
		itClient->second.getName(), "H" , itClient->second.getRealName()));
		sendToClient(user, RPL_ENDOFWHO(user.getName(), mask));
	}
	else
		sendToClient(user, ERR_NOSUCHCHANNEL(user.getName(), mask));
	return (0);
}