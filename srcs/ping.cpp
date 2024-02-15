/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:06:34 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/13 14:13:20 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	pingCmd(int fd, vecStr &cmd, Server &serv){
	Client	user = serv.getClient(fd);

	if (cmd.size() < 2){
		sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), cmd[0]));
		return (1);
	}
	std::string	token = cmd[1];
	sendToClient(user, RPL_CMD(user.getName(), user.getUserName(), "PONG", token));
	return (0);
}