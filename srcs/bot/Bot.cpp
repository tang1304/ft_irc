/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:11:10 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/21 14:50:25 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "Bot.hpp"

Bot::Bot(const int &port, const std::string &password): _port(port), _password(password){
	struct sockaddr_in	servAddr;

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		throw (std::runtime_error("Error: Bot socket creation failed"));
	int	opt = 0;
	opt = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	if (opt < 0){
		close(_socket);
		throw (std::runtime_error("Error: setsokopt() failed"));
	}
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_port);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(_socket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
		close(_socket);
		throw (std::runtime_error("Error: Bot connection failed"));
	}
	std::cout << BLUE << "Connected to server" << DEFAULT << std::endl;
}

Bot::~Bot(){}

void	Bot::setBufferRead(const std::string &read, int i){
	if (i)
		_bufferRead += read;
	else
		_bufferRead.clear();
}

const std::string	&Bot::getBufferRead() const{
	return (_bufferRead);
}

void	Bot::runningLoop(){
	signal(SIGINT, Bot::signalHandler);
	while (signalStatus == 0)
	{
		char	buffer[BUFFER_SIZE];
		int		bytesRead = 0;

		memset(&buffer, 0, BUFFER_SIZE);
		bytesRead = recv(_socket, buffer, BUFFER_SIZE, 0);
		if (bytesRead < 1)
			throw (std::runtime_error("Error: Bot recv() failed"));
		else{
			std::string	buf(buffer);
			setBufferRead(buf, 1);
			if ((buf.empty() || buf == "\r\n") && getBufferRead().empty())
				return ;
			size_t pos = getBufferRead().find("\r\n");
			if (pos != std::string::npos){
				buf = getBufferRead();
				parseInput(buf);
				setBufferRead("", 0);
			}
		}
	}
}

void	Bot:: parseInput(std::string &input){
	std::string msg;
	vecStr		command;
	vecVecStr	vecCommand;

	command = splitCmds(input, "\r\n");
	vecCommand = splitCmd(command, " ");
	if (vecCommand.empty() || vecCommand.begin()->empty())
		return ;
	itVecVecStr	itvv = vecCommand.begin();
	for (; itvv != vecCommand.end(); itvv++)
	{
		if (*itvv->begin() != "PRIVMSG"){
			std::string	err = "Sorry, I don't understand your request\r\n";
			send(3, err.c_str(), sizeof(err), 0);
		}
		else {
			send(3, "cool\r\n", 7, 0);
		}
		// itMapCmds	it = _commandsList.find(*itvv->begin());
		// if (it != _commandsList.end() && (*itvv->begin() != "PASS" && *itvv->begin() != "USER" && *itvv->begin() != "NICK")\
		// && !_clients[fd].getRegistered()){
		// msg = "you may register first";
		// 	_clients[fd].setBufferSend(ERROR(msg));
		// 	return ;
		// }
		// if (it != _commandsList.end()){
		// 	it->second(fd, *itvv, *this);
		// }
		// else if (*itvv->begin() != "CAP"){
		// 	_clients[fd].setBufferSend(ERR_UNKNOWNCOMMAND(_clients[fd].getName(), *itvv->begin()));
		// }
	}
}

vecStr	Bot::splitCmds(std::string &input, const std::string &delimiter){
	vecStr		result;
	size_t		pos = 0;
	size_t		prevPos = 0;
	std::string	tmp;

	while ((pos = input.find(delimiter, prevPos)) != std::string::npos && pos != input.size() - 2){
		tmp = input.substr(prevPos, pos - prevPos);
		if (!tmp.empty())
			result.push_back(tmp);
		prevPos = pos + 2;
		tmp.clear();
	}
	tmp = input.substr(prevPos, input.size() - prevPos - 2);
	if (!tmp.empty())
		result.push_back(tmp);
	return (result);
}

vecVecStr	Bot::splitCmd(vecStr &cmds, const std::string &delimiter){
	vecVecStr	result;
	vecStr		cmd;
	std::string	tmp;
	std::string	colonStr;
	size_t		colonPos = 0;
	size_t		pos = 0;
	size_t		prevPos = 0;

	itVecStr	it = cmds.begin();
	for (; it != cmds.end(); it++)
	{
		colonPos = 0;
		pos = 0;
		prevPos = 0;
		cmd.clear();
		tmp.clear();
		colonStr.clear();
		if ((colonPos = it->find(':')) != std::string::npos && (it->c_str()[colonPos - 1] == ' ')){
			colonStr = it->substr(colonPos , it->find("\r\n") - colonPos);
			it->erase(colonPos - 1);
		}
		while ((pos = it->find(delimiter, prevPos)) != std::string::npos){
			tmp = it->substr(prevPos, pos - prevPos);
			if (!tmp.empty())
				cmd.push_back(tmp);
			prevPos = pos + 1;
		}
		tmp = it->substr(prevPos, it->find("\r\n") - prevPos);
		if (!tmp.empty())
			cmd.push_back(tmp);
		if (colonStr.size() > 0)
			cmd.push_back(colonStr);
		result.push_back(cmd);
	}
	return (result);
}

void	Bot::signalHandler(int signal)
{
	std::cout << std::endl << YELLOW << "[BOT] Signal received: " << signal << std::endl;
	if (signal == SIGINT){
		std::cout << "[BOT] Bye !" << DEFAULT << std::endl;
		signalStatus = SIGINT;
	}
	return ;
}