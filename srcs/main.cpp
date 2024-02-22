/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:45:40 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/21 12:42:40 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// join #abc by an abc client no message

#include "../incs/irc.hpp"

int	signalStatus = 0;

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
