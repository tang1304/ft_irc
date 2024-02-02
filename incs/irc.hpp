/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:20:16 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/02 14:05:44 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include "config.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "rpl.hpp"

class Server;

int			quit_cmd(int fd, vecStr &cmd, Server &serv);
int			pass_cmd(int fd, vecStr &cmd, Server &serv);
int			nick_cmd(int fd, vecStr &cmd, Server &serv);
int 		user_cmd(int fd, vecStr &cmd, Server &serv);
int			join_cmd(int fd, vecStr &cmd, Server &serv);
int			privmsgCmd(int fd, vecStr &cmd, Server &serv);
int			checkArgs(const std::string &port, const std::string &password);
size_t		len(std::string s);
vecStr		splitCmds(std::string &input, const std::string &delimiter);
vecVecStr	splitCmd(vecStr &cmds, const std::string &delimiter);
vecPair		create_pair_cmd(vecStr &cmd);
void		sendToAll(Server &serv, const std::string &msg);
void		sendToClient(Client &user, const std::string &msg);
void		sendToChan(Channel &chan, const std::string &msg);

#endif