/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:21:25 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/30 14:19:26 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

# include "config.hpp"

# define RPL_QUIT(nick) (nick + " left the channel\r\n")

//	1
# define RPL_WELCOME( nickname, user_id) (":" + SERVERNAME + " 001 " + nickname + \
	" :Welcome to the" + SERVERNAME + " Network, " + user_id + "[!" + user_id + "@" + LOCALHOST + "]\r\n")

// 2
# define RPL_YOURHOST(client) (":" + SERVERNAME + " 002 " + client + " :Your host is " + SERVERNAME \
	+ ", running version 1.0\r\n")

// 3
# define RPL_CREATED(client, date) (":" + SERVERNAME + " 003 " + client + " :This server was created " + date + "\r\n")

//4
# define RPL_MYINFO(client) (":" + SERVERNAME + " 004 " + client + " " + SERVERNAME + " 1.0, -+iotlk\r\n")

//5
# define RPL_ISUPPORT(client, tokens) (":" + SERVERNAME + " 005 " + client + " " + tokens + "\r\n")

# define ERROR(error) ("ERROR: " + error + "\r\n")
# define ERR_NEEDMOREPARAMS(client, cmd) (client + " " + cmd + " :Not enough parameters\r\n")
# define ERR_NEEDLESSPARAMS " :Too many parameters\n"
# define ERR_UNKNOWNCOMMAND(client, command) (client + " " + command + " :Unknown command\r\n")
# define ERR_PASSWDMISMATCH(client) (client + " :Password incorrect\r\n")
# define ERR_ALREADYREGISTERED(client) (client + " :You may not reregister\r\n")
# define ERR_NONICKNAMEGIVEN(client) (client + " :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(client, nick) (client + " " + nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(client, nick) (client + " " + nick + " :Nickname is already in use\r\n")
# define ERRUSER0(client) (client + " :Must set password first\r\n")
# define ERR_NOTREGISTERED(client) (client + " :You have not registered\r\n")

# define ERR_PASSFIRST(client) (client + " :Must confirm password first\r\n")
# define ERR_NICKFIRST(client) (client + " :Must set nickname first\r\n")

#endif