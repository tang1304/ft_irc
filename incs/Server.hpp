/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:03:20 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/18 13:23:09 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

class Server{
private:

	int			_port;
	std::string	_password;
	int			_socketFd;
	Server();
	
public:

	~Server();
	Server(const int &port, const std::string &password);
	Server(const Server &other);
	Server &operator=(const Server &other);

	void	socketInit();
};

#endif