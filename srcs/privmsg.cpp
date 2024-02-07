/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:03:30 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/07 11:12:19 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	privmsgCmd(int fd, vecStr &cmd, Server &serv){
	if (cmd.size() < 2){
		sendToClient(serv.getClientMap()[fd], ERR_NORECIPIENT(serv.getClientMap()[fd].getName(), cmd[0]));
		return (0);
	}
	if (cmd.size() < 3 || (cmd.size() == 3 && cmd[2].empty())){
		sendToClient(serv.getClientMap()[fd], ERR_NOTEXTTOSEND(serv.getClientMap()[fd].getName()));
		return (0);
	}
	if (cmd.size() > 3 || (cmd.size() == 3 && cmd[2].find(':') == std::string::npos)){
		sendToClient(serv.getClientMap()[fd], ERR_TOOMANYTARGETS(serv.getClientMap()[fd].getName()));
		return (0);
	}
	return  (1);
}