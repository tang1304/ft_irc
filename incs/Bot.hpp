/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:05:27 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/21 16:10:36 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <sstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdlib.h>
# include <csignal>
# include <string.h>
# include <vector>
# include <poll.h>

# define DEFAULT "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"

# define USAGE "Error\nThere must be 2 arguments : ./bot <port> <password>"
# define BUFFER_SIZE 1000

typedef std::vector<std::string>			vecStr;
typedef std::vector<std::string>::iterator	itVecStr;
typedef std::vector<vecStr>					vecVecStr;
typedef std::vector<vecStr>::iterator		itVecVecStr;

extern int	signalStatus;

class Bot{
private:
	int			_socket;
	int			_port;
	std::string	_password;
	std::string	_bufferRead;
	std::string	_bufferSend;
	std::vector<pollfd>	_pollFds;

public:
	Bot(const int &port, const std::string &password);
	~Bot();

	void				setBufferRead(const std::string &read, int i);
	const std::string	&getBufferRead() const;
	void				runningLoop();
	void				parseInput(std::string &input);
	vecStr				splitCmds(std::string &input, const std::string &delimiter);
	vecVecStr			splitCmd(vecStr &cmds, const std::string &delimiter);
	static void			signalHandler(int signal);
};

#endif