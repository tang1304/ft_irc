/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:01 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/18 14:01:12 by tgellon          ###   ########lyon.fr   */
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
	;
}