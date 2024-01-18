/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:01 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/18 16:36:25 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::~Server(){
}

Server::Server(const int &port, const std::string &password): _port(port), _password(password){
	socketInit();
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

void	Server::socketInit(){
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