/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:03:30 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/15 10:08:42 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	privmsgCmd(int fd, vecStr &cmd, Server &serv){
	Client	user = serv.getClient(fd);

	if (cmd.size() == 1 || cmd.size() > 3 || cmd[1].find(':') == 0){
		sendToClient(user, ERR_NORECIPIENT(user.getName(), cmd[0]));
		return (1);
	}
	if (cmd.size() == 2 || (cmd.size() == 3 && (cmd[2].empty() || (cmd[2].find(':') == 0 && cmd[2].size() == 1)))){
		sendToClient(user, ERR_NOTEXTTOSEND(user.getName()));
		return (1);
	}
	if ((cmd.size() == 3 && cmd[2].find(':') == std::string::npos)){
		sendToClient(user, ERR_TOOMANYTARGETS(user.getName()));
		return (1);
	}

	std::string	target = cmd[1];
	std::string	msg = cmd[2];//.substr(1, std::string::npos);
	if (target.find_first_of("#&") == 0){
		for (itVecChan it = serv.getChanList().begin(); it != serv.getChanList().end(); it++){
			if (it->getName() == target){
				if (isItIn(user, it->getChanop()) || isItIn(user, it->getUsersJoin())){
					sendToChanNotUser(user, *it, RPL_CMD(user.getName(), user.getUserName(), "PRIVMSG " + target, msg));
					return (0);
				}
				else{
					sendToClient(user, ERR_CANNOTSENDTOCHAN(user.getName(), *it->getName()));
					return (1) ;
				}
			}
		}
		sendToClient(user, ERR_NOSUCHNICK(user.getName(), target));
		return (1);
	}
	else {
		for (itVecClient it = serv.getAllClients().begin(); it != serv.getAllClients().end(); it++){
			if (it->getName() == target){
				sendToClient(*it, RPL_CMD(user.getName(), user.getUserName(), "PRIVMSG " + target, msg));
				return (0);
			}
		}
		sendToClient(user, ERR_NOSUCHNICK(user.getName(), target));
	}

	return  (1);
}