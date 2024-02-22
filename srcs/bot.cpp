/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:31:57 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/22 13:39:45 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	botCmd(int fd, vecStr &cmd, Server &serv)
{
	Client	user = serv.getClient(fd);

	if (cmd.size() < 2){
		sendToClient(user, ERR_NEEDMOREPARAMS(user.getName(), cmd[0]));
		return (1);
	}
	std::string	nbr = cmd[1];
	if (nbr.find_first_not_of("0123456789", 0) != std::string::npos){
		sendToClient(user, ERROR(nbr + " not a valid argument"));
		return (1);
	}
	std::string url = "curl --request GET --url 'https://numbersapi.p.rapidapi.com/" + nbr + "/trivia?notfound=floor' --header 'X-RapidAPI-Host: numbersapi.p.rapidapi.com' --header 'X-RapidAPI-Key: 8811cec522mshe204decfbb38e0ep14fcdbjsne677ef174dcb'";
	FILE*	pipe = popen(url.c_str(), "r");
	if (pipe == NULL){
		sendToClient(user, ERROR(nbr + " not a valid argument"));
		return (1);
	}
	std::string result = "";
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
	sendToClient(user, result);
	return (0);
}