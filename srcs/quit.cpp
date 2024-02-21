/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:41:49 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/21 15:16:40 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../incs/irc.hpp"

int	quitCmd(int fd, vecStr &cmd, Server &serv){
	Client		user = serv.getClient(fd);
	std::string	userName = user.getName();
	itVecClient	itClient;
	std::string	reason;

	if (cmd.size() > 1)
		reason = cmd[1].substr(1, std::string::npos);
	else
		reason = "No reason given";
	sendToClient(user, ERROR(std::string("Disconnected from server")));
	serv.clientDisconnection(fd);
	return (1);
}
