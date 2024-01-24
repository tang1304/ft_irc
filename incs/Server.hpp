/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:20 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/24 17:14:08 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "Channel.hpp"
# include "Client.hpp"

class Channel;
class Client;

typedef std::vector<std::string>	vecstr;
typedef std::map<int, Client>		ClientMap;

extern int	signalStatus;

class Server{
private:

	int					_port;
	int					_socketFd;
	std::string			_password;
	std::vector<pollfd>	_pollFds; // To use poll(), keeps track of fds for multiple clients
	ClientMap			_clients;
	Server();

public:

	~Server();
	Server(const int &port, const std::string &password);
	Server(const Server &other);
	Server		&operator=(const Server &other);

	void		runningLoop();
	void		clientConnexion();
	void		clientDisconnection(int fd);
	void		clientHandle(int fd);
	void		parseInput(int fd, std::string input);
	static void	signalHandler(int signal); //static because of signal() that can' t accept member function

	std::string	getPassword() const;
	ClientMap	getClientMap() const;
};

void	pass_cmd(int fd, vecstr& cmd, Server& serv);
void	nick_cmd(int fd, vecstr& cmd, Server& serv);

#endif