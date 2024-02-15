/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:01 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/15 10:02:12 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"
#include "irc.hpp"

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
	std::cout << BLUE << "Server launched\nWaiting for connections..." << DEFAULT << std::endl;
}

void	Server::cmdInit(){
	_commandsList["PASS"] = &passCmd;
	_commandsList["USER"] = &userCmd;
	_commandsList["NICK"] = &nickCmd;
	_commandsList["QUIT"] = &quitCmd;
	_commandsList["MOTD"] = &motdCmd;
	_commandsList["PING"] = &pingCmd;
	_commandsList["PRIVMSG"] = &privmsgCmd;
	_commandsList["JOIN"] = &joinCmd;
	_commandsList["PART"] = &partCmd;
	_commandsList["KICK"] = &kickCmd;
	_commandsList["INVITE"] = &inviteCmd;
	_commandsList["TOPIC"] = &topicCmd;
	_commandsList["WHO"] = &whoCmd;
	_commandsList["MODE"] = &modeCmd;
	_commandsList["LIST"] = &listCmd;
}

const std::string	&Server::getPassword() const
{
	return (_password);
}

vecClient &Server::getAllClients()
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
	sendToChan(newChan, RPL_CMD(user.getName(), user.getUserName(), "JOIN" ,chan));
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
	std::cout << std::endl << YELLOW << "[Server] Signal received: " << signal << std::endl;
	if (signal == SIGINT){
		std::cout << "[Server] Shutting down the server. Bye !" << DEFAULT << std::endl;
		signalStatus = SIGINT;
	}
	return ;
}

void	Server::runningLoop(){
	signal(SIGINT, Server::signalHandler);
	while (signalStatus == 0)
	{
		if (poll(this->_pollFds.data(), this->_pollFds.size(), -1) == -1 && !signalStatus)
			throw (std::runtime_error("[Server] Error: poll() failed"));
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
	close(_socketFd);
}

void	Server::clientConnexion(){
	struct sockaddr_in	clientAddr;
	socklen_t			clientAddrLen = sizeof(clientAddr);

	int	clientSocket = accept(_socketFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
	if (clientSocket == -1)
		throw (std::runtime_error("[Server] Error: accept() failed"));
	struct pollfd tmp;
	tmp.fd = clientSocket;
	tmp.events = POLLIN;
	tmp.revents = 0;
	_pollFds.push_back(tmp);
	_clients[clientSocket] = Client();
	_clients[clientSocket].setClientFd(clientSocket);
	std::cout << GREEN << "[Server] New client succesfully connected on socket #" << clientSocket \
		<< DEFAULT << std::endl;
}

void	Server::clientDisconnection(const int &fd){
	itVecPollfd	it = _pollFds.begin();
	itVecChan	itChan;
	itVecClient	itClient;
	std::string	user = getClient(fd).getName();

	for (itChan = getChanList().begin(); itChan != getChanList().end(); itChan++)
	{
		if ((itClient = findIt(user, itChan->getUsersJoin())) != itChan->getUsersJoin().end()){
			sendToChan(*itChan, RPL_CMD(itClient->getName(), itClient->getUserName(), "QUIT", ""));
			itChan->removeUser(*itClient);
		}
		else if ((itClient = findIt(user, itChan->getChanop())) != itChan->getChanop().end() && \
		itClient->getName() != user){
			sendToChan(*itChan, RPL_CMD(itClient->getName(), itClient->getUserName(), "QUIT", ""));
			itChan->removeChanop(*itClient);
		}
		if (itChan->getConnected() == 0){
			removeChan(itChan->getId());
			itChan--;
		}
	}
	std::cout << YELLOW << "[Server] Client #" << _clients[fd].getClientFd() << \
		" disconnected from the server" << DEFAULT << std::endl;
	close(fd);
	for (itClient = getAllClients().begin(); itClient != getAllClients().end(); itClient++)
	{
		if (itClient->getFd() == fd)
			_clients.erase(itClient);
	}
	while (it->fd != fd)
		it++;
	_pollFds.erase(it);
}

void	Server::clientHandle(const int &fd){
	char	buffer[BUFFER_SIZE];
	static	std::string	buf;
	int		bytesRead = 0;
	Client	user = getClient(fd);

	memset(&buffer, 0, BUFFER_SIZE);
	bytesRead = recv(fd, buffer, BUFFER_SIZE, 0);
	if (bytesRead == -1){
		std::cerr << RED << "[Server] Error: recv() failed: " << strerror(errno) << DEFAULT << std::endl;
		clientDisconnection(fd);
	}
	else if (bytesRead == 0)
		clientDisconnection(fd);
	else{
		std::cout << std::endl << PURPLE << "[Client] Received data from client before #" << fd << ": " << buffer << DEFAULT;
// std::cout << GREEN << "buffer: " << buffer << ". Size: " << BUFFER_SIZE << DEFAULT << std::endl;
		buf += buffer;
//		_clients[fd].setBufferRead(std::string(buf), 1);
		if (buf.empty() || buf == "\r\n")
		{
			buf.clear();
			return ;
		}
//		size_t pos = _clients[fd].getBufferRead().find("\r\n");
		size_t pos = buf.find("\r\n");
		if (pos == std::string::npos)
			return ;
		if (pos != std::string::npos){
			_clients[fd].setBufferRead(std::string(buf), 1);
			std::cout << std::endl << PURPLE << "[Client] Received data from client #" << fd << ": " << buf << DEFAULT;
			buf = _clients[fd].getBufferRead();
			parseInput((fd), buf);
			_clients[fd].setBufferRead("", 0);
			buf.clear();
		}
		send(_clients[fd].getClientFd(), _clients[fd].getBufferSend().c_str(), _clients[fd].getBufferSend().length(), 0);
		_clients[fd].setBufferSend("");
		if (_clients[fd].getDisconnect())
			clientDisconnection(fd);
	}
}

void	Server:: parseInput(const int &fd, std::string &input){
	vecStr		command;
	vecVecStr	vecCommand;
	command = splitCmds(input, "\r\n");

	vecCommand = splitCmd(command, " ");
	if (vecCommand.empty() || vecCommand.begin()->empty())
		return ;
	itVecVecStr	itvv = vecCommand.begin();
	for (; itvv != vecCommand.end(); itvv++)
	{
// itVecStr	i = itvv->begin();
// for (; i < itvv->end(); i++){
// 	std::cout << BLUE << "[SERV] cmd: " << *i << "." << DEFAULT << std::endl;
// }
		itMapCmds	it = _commandsList.find(*itvv->begin());
		if (it != _commandsList.end() && (*itvv->begin() != "PASS" && *itvv->begin() != "USER" && *itvv->begin() != "NICK")\
		&& !_clients[fd].getRegistered()){
			_clients[fd].setBufferSend(ERR_NOTREGISTERED(_clients[fd].getName()));
			return ;
		}
		if (it != _commandsList.end()){
			it->second(fd, *itvv, *this);
		}
		else if (*itvv->begin() != "CAP"){
			_clients[fd].setBufferSend(ERR_UNKNOWNCOMMAND(_clients[fd].getName(), *itvv->begin()));
		}
	}
}

void	Server::registrationDone(int &fd){
	_clients[fd].setBufferSend(RPL_WELCOME(_clients[fd].getName(), _clients[fd].getUserName()));
	_clients[fd].setBufferSend(RPL_YOURHOST(_clients[fd].getName()));
	_clients[fd].setBufferSend(RPL_CREATED(_clients[fd].getName(), "2024"));
	_clients[fd].setBufferSend(RPL_MYINFO(_clients[fd].getName()));
	_clients[fd].setBufferSend(RPL_ISUPPORT(_clients[fd].getName(), "token"));
	_clients[fd].setBufferSend(RPL_MOTDSTART(_clients[fd].getName()));
	_clients[fd].setBufferSend(RPL_MOTD(_clients[fd].getName()));
	_clients[fd].setBufferSend(RPL_ENDOFMOTD(_clients[fd].getName()));
}