/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:41:49 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/29 16:36:15 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "irc.hpp"

int	quit_cmd(int &fd, vecStr &cmd, Server &serv){
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
}

void	msgToAll(Channel &chan, Client &client){
	itVecClient		itClient = chan.getClientsVec().begin();

	for (; itClient != chan.getClientsVec().end(); itClient++){
		if (itClient->getFd() != client.getFd())
			itClient->setBufferSend(RPL_QUIT(itClient->getNickName()), 1);
	}
}