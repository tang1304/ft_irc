/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:45:59 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/05 08:46:48 by tgellon          ###   ########lyon.fr   */
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
# include <errno.h>
# include <csignal>
# include <cstring>

# define DEFAULT "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define PURPLE "\033[35m"
# define CYAN "\033[36m"

# define SERVERNAME std::string("Ft_Irc")
# define LOCALHOST std::string("127.0.0.1")
# define BUFFER_SIZE 1000
# define NICKLEN 9
# define USERLEN 12
# define CHANMAXUSER 10
# define USERCHANLIMIT 5

# define USAGE "Error\nThere must be 2 arguments : ./ircserv <port> <password>"

class Server;

class Client;
class Channel;

typedef std::vector<pollfd>::iterator								itVecPollfd;
typedef std::vector<Channel>										vecChan;
typedef std::vector<Channel>::iterator								itVecChan;
typedef std::vector<Client>											vecClient;
typedef std::vector<Client>::iterator								itVecClient;
typedef std::vector<std::string>									vecStr;
typedef std::vector<std::string>::iterator							itVecStr;
typedef std::vector<vecStr>											vecVecStr;
typedef std::vector<vecStr>::iterator								itVecVecStr;
typedef std::map<int, Client>										clientMap;
typedef std::map<int, Client>::iterator								itClientMap;
typedef std::vector<std::pair<std::string, std::string> >			vecPair;
typedef std::vector<std::pair<std::string, std::string> >::iterator	itVecPair;
typedef int (*fctPointer)(int, vecStr &, Server &);
typedef std::map<std::string, fctPointer>							mapCmds;
typedef std::map<std::string, fctPointer>::iterator					itMapCmds;


#endif
