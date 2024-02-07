/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 15:09:03 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/07 09:30:06 by tgellon          ###   ########lyon.fr   */
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
		it->removeChanop(user);
	}
	else{
		it->removeUser(user);
	}
	return (0);
}