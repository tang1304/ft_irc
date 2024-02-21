/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:11:10 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/21 11:44:49 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "Bot.hpp"

Bot::Bot(const int &port, const std::string &password): _port(port), _password(password){
	struct sockaddr_in	servAddr;

	int	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		throw (std::runtime_error("Error: Bot socket creation failed"));
	int	opt = 0;
	opt = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	if (opt < 0){
		close(sock);
		throw (std::runtime_error("Error: setsokopt() failed"));
	}
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_port);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
		close(sock);
		throw (std::runtime_error("Error: Bot connection failed"));
	}
	std::cout << BLUE << "Connected to server" << DEFAULT << std::endl;
}

Bot::~Bot(){}