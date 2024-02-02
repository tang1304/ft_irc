/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:01 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/02 10:56:55 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

Server::~Server() { }

Server::Server(const int &port, const std::string &password): _port(port), _password(password){
	// socket initiation;
	struct sockaddr_in	servAddr;

	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd < 0)
		throw (std::runtime_error("Error: Socket creation failed"));
	int	opt = 0;
	opt = setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	if (opt < 0)
		throw (std::runtime_error("Error: setsokopt() failed"));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(_port);
	if (bind(_socketFd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
		throw (std::runtime_error("Error: bind() failed"));
	if (listen(_socketFd, SOMAXCONN) < 0)
		throw (std::runtime_error("Error: listen() failed"));
	struct pollfd tmp;
	tmp.fd = _socketFd;
	tmp.events = POLLIN;
	tmp.revents = 0;
	_pollFds.push_back(tmp);
	cmdInit();
}

void	Server::cmdInit(){
	_commandsList["PASS"] = &pass_cmd;
	_commandsList["USER"] = &user_cmd;
	_commandsList["NICK"] = &nick_cmd;
	// _commandsList["CAP"] = &cap_cmd;
	_commandsList["QUIT"] = &quit_cmd;
	// _commandsList["PING"] = &ping;
	// _commandsList["PRIVMSG"] = &privmsg;
	_commandsList["JOIN"] = &join_cmd;
	// _commandsList["PART"] = &part;
	// _commandsList["TOPIC"] = &topic;
	// _commandsList["KICK"] = &kick;
	// _commandsList["INVITE"] = &invite;
	// _commandsList["LIST"] = &list;
}

const std::string	&Server::getPassword() const
{
	return (_password);
}

clientMap &Server::getClientMap()
{
	return (_clients);
}

Client	&Server::getClient(int fd)
{
	return (_clients[fd]);
}

vecChan	&Server::getChanList()
{
	return (_chanList);
}

void	Server::addChan(std::string chan, std::string key, Client &user)
{
	int	j = _chanList.size();
	Channel	newChan(chan, key);

	newChan.setId(j);
	newChan.addChanop(user);
	user.setChanCount(1);
	_chanList.push_back(newChan);
}

void	Server::removeChan(int id)
{
	int	i = 0;
	_chanList.erase(_chanList.begin() + id);

	for (itVecChan it = _chanList.begin(); it != _chanList.end(); it++)
	{
		it->setId(i);
		i++;
	}
}

void	Server::signalHandler(int signal)
{
	std::cout << std::endl << YELLOW << "Signal received: " << signal << DEFAULT << std::endl;
	if (signal == SIGINT){
		std::cout << "Shutting down the server. Bye !" << std::endl;
		signalStatus = SIGINT;
	}
	return ;
}

void	Server::runningLoop(){
	signal(SIGINT, Server::signalHandler);
	while (signalStatus == 0)
	{
		if (poll(_pollFds.data(), _pollFds.size(), -1) == -1 && !signalStatus)
			throw (std::runtime_error("Error: poll() failed"));
		for (size_t i = 0; i < _pollFds.size(); ++i){
			if (_pollFds[i].revents & POLLIN){ //there is data to read
				if (_pollFds[i].fd == _pollFds[0].fd){ // socket fd -> means a new connection
					clientConnexion();
				}
				else{ // means you're on an existing client -> handle client data
					clientHandle(_pollFds[i].fd);
				}
			}
		}
	}
}

void	Server::clientConnexion(){
	struct sockaddr_in	clientAddr;
	socklen_t			clientAddrLen = sizeof(clientAddr);

	int	clientSocket = accept(_socketFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
	if (clientSocket == -1)
		throw (std::runtime_error("Error: accept() failed"));
	struct pollfd tmp;
	tmp.fd = clientSocket;
	tmp.events = POLLIN;
	tmp.revents = 0;
	_pollFds.push_back(tmp);
	_clients[clientSocket] = Client();
	_clients[clientSocket].setClientFd(clientSocket);
	std::cout << GREEN << "New client succesfully connected on socket " << clientSocket << DEFAULT << std::endl;
}

void	Server::clientDisconnection(const int &fd){
	itVecPollfd	it = _pollFds.begin();
	itVecChan	itChan;
	itVecClient	itClient;
	itVecClient	itChanop;

	for (itChan = getChanList().begin(); itChan != getChanList().end(); itChan++)
	{
		for (itClient = itChan->getUsersJoin().begin(); itClient != itChan->getUsersJoin().end(); itClient++)
		{
			if (itClient->getClientFd() == fd)
			{
				itChan->removeUser(*itClient);
				itClient--;
			}
		}
		for (itChanop = itChan->getChanop().begin(); itChanop != itChan->getChanop().end(); itChanop++)
		{
			if (itChanop->getClientFd() == fd)
			{
				itChan->removeChanop(*itChanop);
				itChanop--;
			}
		}
		if (itChan->getConnected() == 0)
		{
			removeChan(itChan->getId());
			itChan--;
		}
	}

	std::cout << YELLOW << _clients[fd].getClientFd() << " disconnected from the server" << DEFAULT << std::endl;
	close(fd);
	_clients.erase(fd);
	while (it->fd != fd)
		it++;
	_pollFds.erase(it);

	// TEST
for (itVecChan itc = getChanList().begin(); itc != getChanList().end(); itc++)
{
	std::cout << "Chan " << itc->getName() << " created." << std::endl;
	if (!itc->getPassword().empty())
		std::cout << "Chan password " << itc->getPassword() << "." << std::endl;
	else
		std::cout << "No password set for this channel." << std::endl;
	std::cout << "Number of users + chanops connected: " << itc->getConnected() << "." << std::endl;
	for (itVecClient ut = itc->getUsersJoin().begin(); ut != itc->getUsersJoin().end(); ut++)
		std::cout << "user " << ut->getNickName() << " connected." << std::endl;
	for (itVecClient ut = itc->getChanop().begin(); ut != itc->getChanop().end(); ut++)
		std::cout << "Chanop " << ut->getNickName() << " connected." << std::endl;
}
	//END TEST
}

void	Server::clientHandle(const int &fd){
	char	buffer[BUFFER_SIZE];
	int		bytesRead = 0;

	memset(&buffer, 0, BUFFER_SIZE);
	bytesRead = recv(fd, buffer, sizeof(buffer), 0);
	if (bytesRead == -1){
		std::cerr << RED << "Error: recv() failed: " << strerror(errno) << DEFAULT << std::endl;
		clientDisconnection(fd);
	}
	else if (bytesRead == 0)
		clientDisconnection(fd);
	else{
std::cout << BLUE << "buffer: " << buffer << "." << DEFAULT << std::endl;
		std::string	buf(buffer);
		if (buf.empty() || buf == "\r\n")
			return ;
		_clients[fd].setBufferRead(std::string(buf), 1);
		size_t pos = _clients[fd].getBufferRead().find("\r\n");
		if (pos != std::string::npos){
			parseInput((fd), buf);
			_clients[fd].setBufferRead("", 0);
		}
		send(_clients[fd].getClientFd(), _clients[fd].getBufferSend().c_str(), _clients[fd].getBufferSend().length(), 0);
		_clients[fd].setBufferSend("");
		if (_clients[fd].getDisconnect())
			clientDisconnection(fd);
	}
}

void	Server::parseInput(const int &fd, std::string &input){
	vecStr	command;

std::cout << GREEN << "COMMANDE " << DEFAULT << std::endl;
	command = splitCmd(input, " ");
	if (command.empty())
		return ;
	itMapCmds	it = _commandsList.find(command[0]);
for (size_t i = 0; i < command.size(); i++)
std::cout << YELLOW << "[SERVER] cmd: " << i << " " << command[i] << "." << DEFAULT << std::endl;
	if (it != _commandsList.end() && (command[0] != "PASS" && command[0] != "USER" && command[0] != "NICK" \
		&& command[0] != "QUIT") && !_clients[fd].getDisconnect()){
		_clients[fd].setBufferSend(ERR_NOTREGISTERED(_clients[fd].getNickName()));
		return ;
		}
	if (it != _commandsList.end()){
		it->second(fd, command, *this);
	}
	else if (command[0] != "CAP"){
		_clients[fd].setBufferSend(ERR_UNKNOWNCOMMAND(_clients[fd].getNickName(), command[0]));
	}
}

void	Server::registrationDone(int &fd){
	_clients[fd].setBufferSend(RPL_WELCOME(_clients[fd].getNickName(), _clients[fd].getUserName()));
	_clients[fd].setBufferSend(RPL_YOURHOST(_clients[fd].getNickName()));
	_clients[fd].setBufferSend(RPL_CREATED(_clients[fd].getNickName(), "2024"));
	_clients[fd].setBufferSend(RPL_MYINFO(_clients[fd].getNickName()));
	_clients[fd].setBufferSend(RPL_ISUPPORT(_clients[fd].getNickName(), "token"));
}

// TEST
// void	Server::parseInput(const int &fd, std::string &input)
// {
// 	vecStr command;
// 	(void) input;

// if (fd == 4)
// {
// 	command.push_back("JOIN");
// 	command.push_back("#abc,#def,&ghi,j kl");
// 	command.push_back("abc");
// _clients[fd].setRegistered();
// _clients[fd].setPass();
// _clients[fd].setNickName("TOTO");
// }
// else
// {
// 	command.push_back("JOIN");
// 	command.push_back("#abc");//,#def,&jkl,j kl");
// 	command.push_back("abc,def");
// _clients[fd].setRegistered();
// _clients[fd].setPass();
// _clients[fd].setNickName("TITI");
// }
// 	if (command[0] == "JOIN" || command[1] == "JOIN")
// 		join_cmd(fd, command, *this);
// 	std::cout << _clients[fd].getNickName() << std::endl;
// }

//END TEST
