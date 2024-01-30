/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:45:59 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/30 11:17:45 by rrebois          ###   ########lyon.fr   */
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
# include <cstring>

# define DEFAULT "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"

# define BUFFER_SIZE 1000
# define NICKLEN 9
# define USERLEN 12
# define CHANMAXUSER 10
# define USERCHANLIMIT 2

# define USAGE "Error\nThere must be 2 arguments : ./ircserv <port> <password>"

class Server;
class Client;
class Channel;

typedef std::vector<Channel>								vecChan;
typedef std::vector<Client>									vecCli;
typedef std::vector<std::string>							vecStr;
typedef std::vector<std::string>::iterator					itVecStr;
typedef std::map<int, Client>								clientMap;
typedef std::vector<std::pair<std::string, std::string> >	vecPair;
typedef void (*fctPointer)(int, vecStr, Server);
typedef std::map<std::string, fctPointer>					mapCmds;
typedef std::map<std::string, fctPointer>::iterator			itMapCmds;

#endif
