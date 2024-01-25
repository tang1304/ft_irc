/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:20 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/25 09:03:33 by tgellon          ###   ########lyon.fr   */
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

	ClientMap				_clients;
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
	ClientMap	getClientMap() const;
	void		cmdInit();
	static void	signalHandler(int signal); //static because of signal() that can' t accept member function
	void		runningLoop();
	void		clientConnexion();
	void		clientDisconnection(const int &fd);
	void		clientHandle(const int &fd);
	void		parseInput(const int &fd, const std::string &input);
	void		msgToClient(const int &fd, const std::string &msg);
};

void	pass_cmd(int fd, vecstr& cmd, Server& serv);
void	nick_cmd(int fd, vecstr& cmd, Server& serv);

#endif