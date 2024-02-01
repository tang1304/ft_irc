/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:41:49 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/01 10:05:57 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "irc.hpp"

static void	msgToAll(Channel &chan, Client &client){
	itVecClient	itClient = chan.getUsersJoin().begin();

	for (; itClient != chan.getUsersJoin().end(); itClient++){
		if (itClient->getFd() != client.getFd())
			itClient->setBufferSend(RPL_QUIT(itClient->getNickName()));
	}
}

int	quit_cmd(int fd, vecStr &cmd, Server &serv){
	(void)cmd;
	itVecChan	itChan = serv.getChanList().begin();

	serv.getClientMap()[fd].setBufferSend("Disconnected from the server \r\n");
	for (; itChan != serv.getChanList().end(); itChan++){
		itVecClient	itClient = itChan->getUsersJoin().begin();
		for (; itClient != itChan->getUsersJoin().end(); itClient++){
			if (itClient->getFd() == fd){
				msgToAll(*itChan, *itClient);
				itChan->getUsersJoin().erase(itClient);
				break ;
			}
		}
	}
	serv.clientDisconnection(fd);
	return (1);
}
