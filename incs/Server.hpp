/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:20 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/30 13:58:20 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "Channel.hpp"
# include "Client.hpp"

class Channel;
class Client;

typedef std::vector<std::string>		vecstr;
typedef std::vector<Channel>			vecChannel;
typedef std::vector<Channel>::iterator	itVecChannel;
typedef std::vector<Client>				vecClient;
typedef std::vector<Client>::iterator	itVecClient;
typedef std::map<int, Client>			ClientMap;

extern int	signalStatus;

class Server{
private:

	ClientMap				_clients;
	vecChannel				_channels;
	int						_port;
	std::string				_password;
	int						_socketFd;
	std::vector<pollfd>		_pollFds; // To use poll(), keeps track of fds for multiple clients
	mapCmds					_commandsList;

	Server();

public:

	~Server();
	Server(const int &port, const std::string &password);

	std::string	getPassword() const;
	ClientMap	&getClientMap();
	vecChannel	&getChannelVec();
	void		cmdInit();
	static void	signalHandler(int signal); //static because of signal() that can' t accept member function
	void		runningLoop();
	void		clientConnexion();
	void		clientDisconnection(const int &fd);
	void		clientHandle(const int &fd);
	void		parseInput(const int &fd, std::string &input);
	void		registrationDone(int &fd);
};

int	pass_cmd(int fd, vecStr &cmd, Server &serv);
int	nick_cmd(int fd, vecstr &cmd, Server &serv);
int	user_cmd(int fd, vecstr &cmd, Server &serv);
int	cap_cmd(int fd, vecstr &cmd, Server &serv);
int	quit_cmd(int fd, vecstr &cmd, Server &serv);

#endif