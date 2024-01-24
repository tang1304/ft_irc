/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:45:59 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/24 16:46:30 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <sstream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include <unistd.h>
# include <csignal>

# define DEFAULT "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"

# define BUFFER_SIZE 1000

# define USAGE "Error\nThere must be 2 arguments : ./ircserv <port> <password>"
# define ERR_NEEDMOREPARAMS(client, cmd) (client + " " + cmd + " :Not enough parameters/r/n")
# define ERR_NEEDLESSPARAMS " :Too many parameters\n"
# define ERR_PASSWDMISMATCH " :Password incorrect\n"
# define ERR_ALREADYREGISTERED " :You may not reregister\n"
# define ERRUSER0 "Error.\nMust set password first\n"
# define INVCMD "Invalid command\n"

#endif