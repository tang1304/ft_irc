/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:01 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/22 14:09:20 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::~Server(){
}

Server::Server(const int &port, const std::string &password): _port(port), _password(password){
	// socket initiation;
	struct sockaddr_in	servAddr;
	
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd < 0)
		throw (std::runtime_error("Error: Socket creation failed"));
	int	opt = setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
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
}

Server::Server(const Server &other){
	*this = other;
}

Server	&Server::operator=(const Server &other){
	if (this != &other){
		_port = other._port;
		_password = other._password;
		_socketFd = other._socketFd;
	}
	return (*this);
}

void	Server::runningLoop(){
	while (1){
		if (poll(this->_pollFds.data(), this->_pollFds.size(), -1) == -1)
			throw (std::runtime_error("Error: poll() failed"));
		std::vector<pollfd>::iterator	it = _pollFds.begin();
std::cout << "here" << std::endl;
		while (it != _pollFds.end()){
			if (it->revents && POLLIN){ //there is data to read
				if (it->fd == _pollFds[0].fd){ // or it->fd == _socketFd ? // socket fd -> means a new connection
					clientConnexion();
					continue ;
				}
				else{ // means you're on an existing client -> handle client data
					clientHandle(it->fd);
				}
			}
			it++;
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
	std::cout << GREEN << "New client succesfully connected" << DEFAULT << std::endl;
}

void	Server::clientDisconnection(int fd){
	std::cout << YELLOW << _clients[fd]._clientFd << " disconnected from the server" << DEFAULT << std::endl;
	close(fd);
	_clients.erase(fd);
}

void	Server::clientHandle(int fd){
	char	buffer[BUFFER_SIZE];
	int		bytesRead = 0;

	if ((bytesRead = recv(fd, buffer, BUFFER_SIZE, 0)) < 0)
		clientDisconnection(fd);
	else{
		send(_clients[fd]._clientFd, buffer, bytesRead, 0);
	}
}