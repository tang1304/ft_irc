/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:01 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/01 12:26:04 by tgellon          ###   ########lyon.fr   */
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
		if (poll(this->_pollFds.data(), this->_pollFds.size(), -1) == -1 && !signalStatus)
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
	_clients[clientSocket]._clientFd = clientSocket;
	std::cout << GREEN << "New client succesfully connected on socket " << clientSocket << DEFAULT << std::endl;
}

void	Server::clientDisconnection(const int &fd){
	itVecPollfd	it = _pollFds.begin();

	// itVecChan	it;
	// itVecClient	cit;
	// itVecClient	opit;

	// for (it = getChanList().begin(); it != getChanList().end(); it++)
	// {
	// 	for (cit = it->getUsersJoin().begin(); cit != it->getUsersJoin().end(); cit++)
	// 	{
	// 		if (cit->getClientFd() == fd)
	// 			it->removeUser(*cit);
	// 	}
	// }


	std::cout << YELLOW << _clients[fd]._clientFd << " disconnected from the server" << DEFAULT << std::endl;
	close(fd);
	_clients.erase(fd);
	while (it->fd != fd)
		it++;
	_pollFds.erase(it);
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
// std::cout << GREEN << "buffer: " << buffer << "." << DEFAULT << std::endl;
		std::string	buf(buffer);
		if (buf.empty() || buf == "\r\n")
			return ;
		_clients[fd].setBufferRead(std::string(buf), 1);
		size_t pos = _clients[fd].getBufferRead().find("\r\n");
		if (pos != std::string::npos){
			parseInput((fd), buf);
			_clients[fd].setBufferRead("", 0);
		}
		send(_clients[fd]._clientFd, _clients[fd].getBufferSend().c_str(), _clients[fd].getBufferSend().length(), 0);
		_clients[fd].setBufferSend("");
	}
}

void	Server::parseInput(const int &fd, std::string &input){
	vecStr		command;
	vecVecStr	vecCommand;

	command = splitCmds(input, "\r\n");
	vecCommand = splitCmd(command, " ");
	if (command.empty())
		return ;
	itVecVecStr	itvv = vecCommand.begin();
	for (; itvv != vecCommand.end(); itvv++)
	{
std::cout << "COMMANDE " << std::endl;
itVecStr	i = itvv->begin();
for (; i < itvv->end(); i++){
	std::cout << BLUE << "[SERVER] cmd: " << *i << "." << DEFAULT << std::endl;
}
		itMapCmds	it = _commandsList.find(*itvv->begin());
		if (it != _commandsList.end() && (*itvv->begin() != "PASS" && *itvv->begin() != "USER" && *itvv->begin() != "NICK")\
			&& _clients[fd].getDisconnect()){
			_clients[fd].setBufferSend(ERR_NOTREGISTERED(_clients[fd].getNickName()));
			return ;
			}
		if (it != _commandsList.end()){
			it->second(fd, *itvv, *this);
		}
		else if (*itvv->begin() != "CAP"){
			_clients[fd].setBufferSend(*itvv->begin());
			_clients[fd].setBufferSend(ERR_UNKNOWNCOMMAND(_clients[fd].getNickName(), *itvv->begin()));
		}
	}
}

void	Server::registrationDone(int &fd){
	_clients[fd].setBufferSend(RPL_WELCOME(_clients[fd].getNickName(), _clients[fd].getUserName()));
	_clients[fd].setBufferSend(RPL_YOURHOST(_clients[fd].getNickName()));
	_clients[fd].setBufferSend(RPL_CREATED(_clients[fd].getNickName(), "2024"));
	_clients[fd].setBufferSend(RPL_MYINFO(_clients[fd].getNickName()));
	_clients[fd].setBufferSend(RPL_ISUPPORT(_clients[fd].getNickName(), "token"));
}
