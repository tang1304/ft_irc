/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:41:49 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/07 13:29:43 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "irc.hpp"

int	quit_cmd(int fd, vecStr &cmd, Server &serv){
	Client		user = serv.getClient(fd);
	std::string	userName = user.getName();
	itVecClient	itClient;
	std::string	reason;

	if (cmd.size() > 1)
		reason = cmd[1];
	else
		reason = "";
	sendToClient(user, "Disconnected from the server \r\n");
	for (itVecChan itChan = serv.getChanList().begin(); itChan != serv.getChanList().end(); itChan++){
		if ((itClient = findIt(userName, itChan->getUsersJoin())) != itChan->getUsersJoin().end()){
			// itChan->removeUser(*itClient);
			sendToChan(*itChan, RPL_QUIT(itClient->getName(), itChan->getName()));
		}
		else if ((itClient = findIt(userName, itChan->getChanop())) != itChan->getChanop().end()){
			// itChan->removeChanop(*itClient);
			sendToChan(*itChan, RPL_QUIT(itClient->getName(), itChan->getName()));
		}
	}
	serv.clientDisconnection(fd);
	return (1);
}
