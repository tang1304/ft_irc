/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:02:21 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/21 12:21:58 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "Bot.hpp"

static int	checkArgs(const std::string &port, const std::string &password){
	int	portValue = 0;

	if (password.empty())
		throw (std::invalid_argument("Error: the password can't be empty"));
	if (port.find_first_not_of("0123456789") != std::string::npos || port.empty() )
		throw (std::invalid_argument("Error: Wrong port format, must be a number"));
	std::istringstream	issPort(port);
	issPort >> portValue;
	if (issPort.fail() || portValue < 1024 || portValue > 65535)
		throw (std::invalid_argument("Error: Port number must be between 1024 and 65535"));
	return (portValue);
}

int	main(int argc, char **argv){
	if (argc != 3){
		std::cerr << RED << USAGE << DEFAULT << std::endl;
		return (1);
	}
	try{
		int	port = checkArgs(argv[1], argv[2]);
		Bot	bot(port, argv[2]);
		std::string buff = "PASS abc\r\nNICK Bot\r\nUSER bot 0 * bot\r\n";
		send(3, buff.c_str(), sizeof(buff), 0);
		while (1)
			;
	}
	catch(std::exception &e){
		std::cerr << RED << e.what() << DEFAULT << std::endl;
	}
}