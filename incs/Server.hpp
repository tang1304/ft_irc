/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:20 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/22 13:14:04 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "Channel.hpp"
# include "Client.hpp"

class Channel;
class Client;

class Server{
private:

	int						_port;
	std::string				_password;
	int						_socketFd;
	std::vector<pollfd>		_pollFds; // To use poll(), keeps track of fds for multiple clients
	std::map<int, Client>	_clients;
	Server();

public:

	~Server();
	Server(const int &port, const std::string &password);
	Server(const Server &other);
	Server	&operator=(const Server &other);

	void	runningLoop();
	void	clientConnexion();
	void	clientDisconnection(int fd);
	void	clientHandle(int fd);
};

#endif