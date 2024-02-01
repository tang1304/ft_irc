/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:20:16 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/01 11:25:40 by tgellon          ###   ########lyon.fr   */
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

int			cap_cmd(int fd, vecStr &cmd, Server &serv);
int			quit_cmd(int fd, vecStr &cmd, Server &serv);
int			pass_cmd(int fd, vecStr &cmd, Server &serv);
int			nick_cmd(int fd, vecStr &cmd, Server &serv);
int 		user_cmd(int fd, vecStr &cmd, Server &serv);
int			join_cmd(int fd, vecStr &cmd, Server &serv);
int			checkArgs(const std::string &port, const std::string &password);
size_t		len(std::string s);
vecStr		splitCmds(std::string &input, const std::string &delimiter);
vecVecStr	splitCmd(vecStr &cmds, const std::string &delimiter);
vecPair		create_pair_cmd(vecStr &cmd);

#endif
