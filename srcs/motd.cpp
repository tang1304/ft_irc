/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 08:38:22 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/07 09:28:36 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	motdCmd(int fd, vecStr& cmd, Server &serv){
	(void)cmd;
	Client	user = serv.getClient(fd);
	
	sendToClient(user, RPL_MOTDSTART(user.getName()));
	sendToClient(user, RPL_MOTD(user.getName()));
	sendToClient(user, RPL_ENDOFMOTD(user.getName()));
	return (0);
}