/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:20:16 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/20 14:09:15 by tgellon          ###   ########lyon.fr   */
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

int			quitCmd(int fd, vecStr &cmd, Server &serv);
int			passCmd(int fd, vecStr &cmd, Server &serv);
int			nickCmd(int fd, vecStr &cmd, Server &serv);
int 		userCmd(int fd, vecStr &cmd, Server &serv);
int			joinCmd(int fd, vecStr &cmd, Server &serv);
int			kickCmd(int fd, vecStr &cmd, Server &serv);
int			privmsgCmd(int fd, vecStr &cmd, Server &serv);
int			inviteCmd(int fd, vecStr &cmd, Server &serv);
int			partCmd(int fd, vecStr &cmd, Server &serv);
int			motdCmd(int fd, vecStr &cmd, Server &serv);
int			pingCmd(int fd, vecStr &cmd, Server &serv);
int			modeCmd(int fd, vecStr &cmd, Server &serv);
int			whoCmd(int fd, vecStr &cmd, Server &serv);
int			topicCmd(int fd, vecStr &cmd, Server &serv);
int			whoCmd(int fd, vecStr &cmd, Server &serv);
int			listCmd(int fd, vecStr &cmd, Server &serv);
//int			botCmd(int fd, vecStr &cmd, Server &serv);
int			checkArgs(const std::string &port, const std::string &password);
size_t		len(std::string s);
vecStr		split(std::string &input, const std::string &delimiter);
vecStr		splitCmds(std::string &input, const std::string &delimiter);
vecVecStr	splitCmd(vecStr &cmds, const std::string &delimiter);
vecPair		create_pair_cmd(vecStr &cmd);
void		sendToAll(Server &serv, const std::string &msg);
void		sendToClient(Client &user, const std::string &msg);
void		sendToChan(Channel &chan, const std::string &msg);
void		sendToChanNotUser(Client &user, Channel &chan, const std::string &msg);

template <typename T, typename U>
bool	isItIn(T &content, U &container){
	typename U::iterator it = container.begin();

	for (; it != container.end(); it++){
		if (content == *it)
			return (true);
	}
	return (false);
}

template <typename U>
typename U::iterator	findIt(const std::string &name, U &container){
	typename U::iterator it = container.begin();

	for (; it != container.end(); it++){
		if (name == it->getName())
			break ;
	}
	return (it);
}

template <typename U>
typename U::iterator	findIt(Client &target, U &container){
	typename U::iterator it = container.begin();

	for (; it != container.end(); it++){
		if (target == *it)
			break ;
	}
	return (it);
}

#endif