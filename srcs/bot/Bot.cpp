/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:11:10 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/29 14:22:36 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "Bot.hpp"

Bot::Bot(const int &port, const std::string &password): _port(port), _password(password){
	_socket = socket(AF_INET, SOCK_STREAM, 0);

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

void	Bot::setBufferSend(const std::string& msg)
{
	if (msg.empty())
		_bufferSend.clear();
	else
		_bufferSend += msg;
}

const std::string	&Bot::getBufferRead() const{
	return (_bufferRead);
}

const std::string	&Bot::getBufferSend() const{
	return (_bufferSend);
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

void	Bot::connection(){
	sendToClient("PASS " + _password + "\r\nNICK Bot\r\nUSER bot 0 * bot\r\n");
}

void	Bot::runningLoop(){
	signal(SIGINT, Bot::signalHandler);
	while (signalStatus == 0){
		char	buffer[BUFFER_SIZE + 1];
		int		bytesRead = 0;

		std::memset(&buffer, 0, BUFFER_SIZE);
		bytesRead = recv(_socket, buffer, BUFFER_SIZE, MSG_DONTWAIT);
		buffer[BUFFER_SIZE] = 0;
		std::string buf(buffer);
		setBufferRead(buf, 1);
		if (buf.find("PRIVMSG") != std::string::npos)
			parseInput(buf);
		setBufferRead("", 0);
		setBufferSend("");
	}
	close(_socket);
	return ;
}

void	Bot:: parseInput(std::string &input){
	size_t		pos = 0;
	size_t		colon = 0;
	std::string	sub = "";
	vecStr		args;

	pos = input.find("PRIVMSG");
	colon = input.find_first_of(':', pos);
	sub = input.substr(colon + 1);
	args = split(sub, " ");
	if (args.size() != 1){
		sendToClient("[BOT] I only need a number\r\n");
		return ;
	}
	apiCall(*args.begin());
}

int	Bot::apiCall(std::string &cmd)
{
	std::string	nbr = cmd;
	if (nbr.find_first_not_of("0123456789", 0) != std::string::npos){
		sendToClient("[BOT]" + nbr + " not a valid argument\r\n");
		return (1);
	}
	std::string url = "curl --request GET --url 'https://numbersapi.p.rapidapi.com/" + nbr + \
		"/trivia?notfound=floor' --header 'X-RapidAPI-Host: numbersapi.p.rapidapi.com' --header \
		'X-RapidAPI-Key: 8811cec522mshe204decfbb38e0ep14fcdbjsne677ef174dcb'";
	FILE*	pipe = popen(url.c_str(), "r");
	if (pipe == NULL){
		std::cerr << RED << "Error on popen()" << DEFAULT << std::endl;
		return (1);
	}
	std::string result = "[BOT] ";
	char buffer[1000];
	while (fgets(buffer, 1000, pipe) != NULL) {
		if (ferror(pipe)) {
			std::cerr << RED << "Error on fgets()" << DEFAULT << std::endl;
			pclose(pipe);
			return (1);
		}
		result += buffer;
	}
	result += "\r\n";
	pclose(pipe);
	sendToClient(result);
	return (0);
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

void	Bot::sendToClient(const std::string &msg){
	setBufferSend(msg);
	send(_socket, getBufferSend().c_str(), getBufferSend().length(), MSG_NOSIGNAL);
	setBufferSend("");
}
