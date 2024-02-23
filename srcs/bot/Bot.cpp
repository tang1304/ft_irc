/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:11:10 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/23 15:06:44 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "Bot.hpp"

Bot::Bot(const int &port, const std::string &password): _port(port), _password(password){
	_socket = socket(AF_INET, SOCK_STREAM, 0);
std::cout << "Socket: " << _socket << std::endl;
	if (_socket < 0)
		throw (std::runtime_error("Error: Bot socket creation failed"));
	int	opt = 0;
	opt = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (opt < 0){
		close(_socket);
		throw (std::runtime_error("Error: setsokopt() failed"));
	}
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_port = htons(_port);
	_servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(_socket, (struct sockaddr *)&_servAddr, sizeof(_servAddr)) == -1) {
		close(_socket);
		throw (std::runtime_error("Error: Bot connection failed"));
	}
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
	while (signalStatus == 0){
		char	buffer[BUFFER_SIZE + 1];
		int		bytesRead = 0;

		memset(&buffer, 0, BUFFER_SIZE);
		bytesRead = recv(_socket, buffer, sizeof(buffer), 0);
		if (bytesRead < 1)
			throw (std::runtime_error("Error: Bot recv() failed"));
		else{
			std::string	buf(buffer);
std::cout << "Buffer: " << buf << std::endl;
			setBufferRead(buf, 1);
			if (buf.find("PRIVMSG") != std::string::npos)
				parseInput(buf);
			// if ((buf.empty() || buf == "\r\n") && getBufferRead().empty())
			// 	return ;
			// size_t pos = getBufferRead().find("\r\n");
			// if (pos != std::string::npos){
			// 	buf = getBufferRead();
			// 	parseInput(buf);
			// 	setBufferRead("", 0);
			// }
		}
	}
}

void	Bot:: parseInput(std::string &input){
	size_t		pos = 0;
	size_t		colon = 0;
	std::string	sub = "";
	vecStr		args;

	pos = input.find("PRIVMSG");
	colon = input.find_first_of(':', pos);
	sub = input.substr(colon);
std::cout << sub << std::endl;
	args = split(sub, " ");
}

// void	Bot:: parseInput(std::string &input){
// 	std::string msg;
// 	vecStr		command;
// 	vecVecStr	vecCommand;

// 	command = splitCmds(input, "\r\n");
// 	vecCommand = splitCmd(command, " ");
// 	if (vecCommand.empty() || vecCommand.begin()->empty())
// 		return ;
// 	itVecVecStr	itvv = vecCommand.begin();
// 	for (; itvv != vecCommand.end(); itvv++)
// 	{
// 		if (*itvv->begin() != "PRIVMSG"){
// 			continue ;
// 		}
// 		else {
// 			send(_socket, "cool\r\n", 7, 0);
// 		}
// 		// itMapCmds	it = _commandsList.find(*itvv->begin());
// 		// if (it != _commandsList.end() && (*itvv->begin() != "PASS" && *itvv->begin() != "USER" && *itvv->begin() != "NICK")\
// 		// && !_clients[fd].getRegistered()){
// 		// msg = "you may register first";
// 		// 	_clients[fd].setBufferSend(ERROR(msg));
// 		// 	return ;
// 		// }
// 		// if (it != _commandsList.end()){
// 		// 	it->second(fd, *itvv, *this);
// 		// }
// 		// else if (*itvv->begin() != "CAP"){
// 		// 	_clients[fd].setBufferSend(ERR_UNKNOWNCOMMAND(_clients[fd].getName(), *itvv->begin()));
// 		// }
// 	}
// }

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

void	Bot::signalHandler(int signal)
{
	std::cout << std::endl << YELLOW << "[BOT] Signal received: " << signal << std::endl;
	if (signal == SIGINT){
		std::cout << "[BOT] Bye !" << DEFAULT << std::endl;
		signalStatus = SIGINT;
	}
	exit(EXIT_FAILURE);
	return ;
}

vecStr	Bot::split(std::string &input, const std::string &delimiter){
	vecStr		result;
	size_t		colonPos = 0;
	size_t		space = 0;
	size_t		pos = 0;
	size_t		prevPos = 0;
	std::string	tmp;
	std::string	colonStr;

	if ((colonPos = input.find(':')) != std::string::npos && colonPos != 0 && (space = input[colonPos - 1]) == ' '){
		colonStr = input.substr(colonPos + 1, input.find("\r\n") - colonPos);
		input.erase(colonPos - 1);
	}
	while ((pos = input.find(delimiter, prevPos)) != std::string::npos){
		tmp = input.substr(prevPos, pos - prevPos);
		if (!tmp.empty())
			result.push_back(tmp);
		prevPos = pos + 1;
	}
	tmp = input.substr(prevPos, input.find("\r\n") - prevPos);
	if (!tmp.empty())
		result.push_back(tmp);
	if (colonStr.size() > 0)
		result.push_back(colonStr);
	return (result);
}