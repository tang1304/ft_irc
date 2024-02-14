/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 15:09:03 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/14 13:49:24 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../incs/irc.hpp"

int partCmd(int fd, vecStr &cmd, Server &serv){
	Client		user = serv.getClientMap()[fd];
//Modif to leave multiple channels !
//Modif to add reason !
	if (cmd.size() < 2){
		sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), cmd[0]));
		return (1);
	}
	if (cmd.size() > 2){
		std::string	reason;
		for (itVecStr it = cmd.begin() + 2; it != cmd.end(); it++){
			reason += *it;
		}
	}
	vecStr	channels = split(cmd[1], ",");
	for (itVecStr itChan = channels.begin(); itChan != channels.end(); itChan++){
		itVecChan it = findIt(*itChan, serv.getChanList());
		if (it == serv.getChanList().end()){
			sendToClient(user, ERR_NOSUCHCHANNEL(user.getName(), *itChan));
			return (1);
		}
		bool	chanop = isItIn(user, it->getChanop());
		if (isItIn(user, it->getUsersJoin()) == false && chanop == false){
			sendToClient(user, ERR_NOTONCHANNEL(user.getName(), *itChan));
			return (1);
		}
		if (chanop == true){
			sendToChan(*it, RPL_CMD(user.getName(), user.getUserName(), cmd[0], *itChan));
			it->removeChanop(user);
		}
		else{
			sendToChan(*it, RPL_CMD(user.getName(), user.getUserName(), cmd[0], *itChan));
			it->removeUser(user);
		}
		if (it->getConnected() == 0)
			serv.removeChan(it->getId());
	}
	return (0);
}