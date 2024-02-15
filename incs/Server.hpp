/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:20 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/15 09:58:17 by rrebois          ###   ########.fr       */
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

	int					_port;
	int					_socketFd;
	std::string			_password;
	vecClient 			_clients;
	std::vector<pollfd>	_pollFds; // To use poll(), keeps track of fds for multiple clients
	mapCmds				_commandsList;
	vecChan				_chanList;

	Server();

public:

	~Server();
	Server(const int &port, const std::string &password);

	const std::string	&getPassword() const;
	Client				&getClient(int fd); // en template??
	vecClient 			&getAllClients();
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
};

#endif
