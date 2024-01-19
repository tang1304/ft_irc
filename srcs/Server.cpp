/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:01 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/19 15:38:59 by tgellon          ###   ########lyon.fr   */
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
		throw std::runtime_error("Error: Socket creation failed");
	int	opt = setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	if (opt < 0)
		throw std::runtime_error("Error: setsokopt() failed");
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(_port);
	if (bind(_socketFd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
		throw std::runtime_error("Error: bind() failed");
	if (listen(_socketFd, SOMAXCONN) < 0)
		throw std::runtime_error("Error: listen() failed");
	_pollFds.push_back({_socketFd, POLLIN, 0});
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
		while (it != _pollFds.end()){
			if (it->revents & POLLIN){ //there is data to read
				if (it->fd == _pollFds[0].fd){ // socket fd -> means a new connection
					clientConnexion();
					continue ;
				}
				else{ // means you're on an existing client -> handle client data
					clientHandle();
				}
			}
		}
	}
}