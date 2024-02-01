/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:41:49 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/01 16:16:56 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "irc.hpp"

int	quit_cmd(int fd, vecStr &cmd, Server &serv){
	(void)cmd;

	serv.getClientMap()[fd].setBufferSend("Disconnected from the server \r\n");
	serv.clientDisconnection(fd);
	return (1);
}
