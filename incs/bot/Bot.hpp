/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:05:27 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/28 14:02:18 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <sstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdlib.h>
# include <csignal>
# include <cstring>
# include <string.h>
# include <vector>
# include <poll.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>


# define DEFAULT "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"

# define USAGE "Error\nThere must be 2 arguments : ./bot <port> <password>"
# define BUFFER_SIZE 1024

typedef std::vector<std::string>			vecStr;
typedef std::vector<std::string>::iterator	itVecStr;

extern int	signalStatus;

class Bot{
private:
	int					_socket;
	int					_port;
	std::string			_password;
	std::string			_bufferRead;
	std::string			_bufferSend;
	struct sockaddr_in	_servAddr;

public:
	Bot(const int &port, const std::string &password);
	~Bot();

	void				setBufferRead(const std::string &read, int i);
	void				setBufferSend(const std::string& msg);
	const std::string	&getBufferRead() const;
	const std::string	&getBufferSend() const;
	void				connection();
	void				runningLoop();
	void				parseInput(std::string &input);
	int					apiCall(std::string &cmd);
	vecStr				split(std::string &input, const std::string &delimiter);
	static void			signalHandler(int signal);
	void				sendToClient(const std::string &msg);
};

#endif