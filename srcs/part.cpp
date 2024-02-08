/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 15:09:03 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/08 09:44:50 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "irc.hpp"

int partCmd(int fd, vecStr &cmd, Server &serv){
	Client		user = serv.getClientMap()[fd];

	if (cmd.size() < 2){
		sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), cmd[0]));
		return (1);
	}
	std::string	chan = cmd[1];
	itVecChan it = findIt(cmd[1], serv.getChanList());
	if (it == serv.getChanList().end()){
		sendToClient(user, ERR_NOSUCHCHANNEL(user.getName(), chan));
		return (1);
	}
	bool	chanop = isItIn(user, it->getChanop());
	if (isItIn(user, it->getUsersJoin()) == false && chanop == false){
		sendToClient(user, ERR_NOTONCHANNEL(user.getName(), chan));
		return (1);
	}
	if (chanop == true){
		sendToChan(*it, RPL_CMD(user.getName(), user.getUserName(), cmd[0], chan));
		it->removeChanop(user);
	}
	else{
		sendToChan(*it, RPL_CMD(user.getName(), user.getUserName(), cmd[0], chan));
		it->removeUser(user);
	}
	if (it->getConnected() == 0)
		serv.removeChan(it->getId());
	return (0);
}