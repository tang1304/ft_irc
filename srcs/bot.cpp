/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:31:57 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/19 16:14:49 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	botCmd(int fd, vecStr &cmd, Server &serv)
{
	Client	user = serv.getClient(fd);
	std::string	args;

	std::cout << "bot call" << std::endl;
	args = "";
	sendToClient(user, RPL_CMD(user.getName(), user.getUserName(),
										 cmd[0], args));
	return (0);
}
//http://api.openweathermap.org/data/2.5/forecast?id=524901&appid={API key}