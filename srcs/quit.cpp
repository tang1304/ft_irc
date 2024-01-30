/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:41:49 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/30 13:19:36 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "irc.hpp"

static void	msgToAll(Channel &chan, Client &client){
	itVecClient		itClient = chan.getClientsVec().begin();

	for (; itClient != chan.getClientsVec().end(); itClient++){
		if (itClient->getFd() != client.getFd())
			itClient->setBufferSend(RPL_QUIT(itClient->getNickName()), 1);
	}
}

int	quit_cmd(int fd, vecStr &cmd, Server &serv){
	(void)cmd;
	itVecChannel	itChan = serv.getChannelVec().begin();

	serv.getClientMap()[fd].setBufferSend("Disconnected from the server \r\n", 1);
	for (; itChan != serv.getChannelVec().end(); itChan++){
		itVecClient		itClient = itChan->getClientsVec().begin();
		for (; itClient != itChan->getClientsVec().end(); itClient++){
			if (itClient->getFd() == fd){
				msgToAll(*itChan, *itClient);
				itChan->getClientsVec().erase(itClient);
				break ;
			}
		}
	}
	serv.clientDisconnection(fd);
	return (1);
}