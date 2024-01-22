/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:45:40 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/19 14:32:52 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

int	main(int argc, char **argv){
	if (argc != 3){
		std::cerr << RED << USAGE << DEFAULT << std::endl;
		return (1);
	}
	try{
		int	port = checkArgs(argv[1], argv[2]);
		Server serv(port, argv[2]);
		serv.runningLoop();
	}
	catch(std::exception &e){
		std::cerr << RED << e.what() << DEFAULT << std::endl;
	}
}
