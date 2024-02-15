/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:20 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/15 10:11:29 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "Channel.hpp"
# include "Client.hpp"

class Channel;
class Client;

extern int	signalStatus;

class Server{
private:

	clientMap			_clients;
	int					_port;
	std::string			_password;
	int					_socketFd;
	std::vector<pollfd>	_pollFds; // To use poll(), keeps track of fds for multiple clients
	mapCmds				_commandsList;
	vecChan				_chanList;

	Server();

public:

	~Server();
	Server(const int &port, const std::string &password);

	const std::string	&getPassword() const;
	clientMap			&getClientMap();
	Client				&getClient(int fd);
	vecChan				&getChanList();
	void				cmdInit();
	static void			signalHandler(int signal); //static because of signal() that can' t accept member function
	void				runningLoop();
	void				clientConnexion();
	void				clientDisconnection(const int &fd);
	void				clientHandle(const int &fd);
	void				parseInput(const int &fd, std::string &input);
	void				addChan(std::string chan, std::string key, Client &user);
	void				removeChan(int id);
	void				registrationDone(int &fd);
	itClientMap			findClient(std::string name);
};

#endif
