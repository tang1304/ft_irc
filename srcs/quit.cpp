/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:41:49 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/21 14:27:34 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../incs/irc.hpp"

int	quitCmd(int fd, vecStr &cmd, Server &serv){
	Client		user = serv.getClient(fd);
	std::string	userName = user.getName();
	itVecClient	itClient;
	std::string	reason;

	if (cmd.size() > 1)
		reason = cmd[1].substr(1, std::string::npos);
	else
		reason = "No reason given";
	for (itVecChan itChan = serv.getChanList().begin(); itChan != serv.getChanList().end(); itChan++){
		if ((itClient = findIt(userName, itChan->getUsersJoin())) != itChan->getUsersJoin().end()){
			itChan->removeUser(*itClient);
			if (itChan->getConnected() == 0)
			{
				serv.removeChan(itChan->getId());
				return (0);
			}
			sendToChan(*itChan, RPL_CMD(itClient->getName(), itClient->getUserName(), "QUIT", reason));
		}
		else if ((itClient = findIt(userName, itChan->getChanop())) != itChan->getChanop().end()){
			itChan->removeChanop(*itClient);
			if (itChan->getConnected() == 0)
			{
				serv.removeChan(itChan->getId());
				return (0);
			}
			sendToChan(*itChan, RPL_CMD(itClient->getName(), itClient->getUserName(), "QUIT", reason));
		}
	}
	sendToClient(user, ERROR(std::string("Disconnected from server")));
	serv.clientDisconnection(fd);
	return (1);
}
