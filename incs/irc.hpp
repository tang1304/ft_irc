/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:20:16 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/02 13:41:14 by rrebois          ###   ########lyon.fr   */
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
int			invite_cmd(int fd, vecStr &cmd, Server &serv);
int			checkArgs(const std::string &port, const std::string &password);
size_t		len(std::string s);
vecStr		splitCmd(std::string &str, const std::string &delim);
vecPair		create_pair_cmd(vecStr &cmd);

#endif
