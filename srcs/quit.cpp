/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:41:49 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/12 15:56:38 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../incs/irc.hpp"

int	quitCmd(int fd, vecStr &cmd, Server &serv){
	Client		user = serv.getClient(fd);
	std::string	userName = user.getName();
	itVecClient	itClient;
	std::string	reason;

	if (cmd.size() > 1)
		reason = cmd[1];
	else
		reason = ":No reason given";
	for (itVecChan itChan = serv.getChanList().begin(); itChan != serv.getChanList().end(); itChan++){
		if ((itClient = findIt(userName, itChan->getUsersJoin())) != itChan->getUsersJoin().end()){
			sendToChan(*itChan, RPL_CMD(itClient->getName(), itClient->getUserName(), "QUIT", reason));
			itChan->removeUser(*itClient);
		}
		else if ((itClient = findIt(userName, itChan->getChanop())) != itChan->getChanop().end()){
			sendToChan(*itChan, RPL_CMD(itClient->getName(), itClient->getUserName(), "QUIT", reason));
			itChan->removeChanop(*itClient);
		}
	}
	sendToClient(user, ERROR(std::string("Disconnected from server")));
	serv.clientDisconnection(fd);
	return (1);
}
