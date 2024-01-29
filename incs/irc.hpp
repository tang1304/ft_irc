/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:20:16 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/29 12:13:58 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include "config.hpp"
# include "Server.hpp"
# include "rpl.hpp"

class Server;

int			pass_cmd(int fd, vecStr &cmd, Server &serv);
int			nick_cmd(int fd, vecStr &cmd, Server &serv);
int 		user_cmd(int fd, vecStr &cmd, Server &serv);
int			join_cmd(int fd, vecStr &cmd, Server &serv);
int			checkArgs(const std::string &port, const std::string &password);
size_t		len(std::string s);
vecPair		create_pair_cmd(vecStr &cmd);
vecStr		splitCmd(const std::string &str, const std::string &delim);

#endif
