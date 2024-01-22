/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:14:10 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/22 14:15:34 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Client{
private:


public:

	std::string	_nickName;
	std::string	_realName;
	std::string	_bufferRead;
	std::string	_bufferSend;
	int			_clientFd;
	Client();
	~Client();
	Client(const Client &other);

	std::string	getNickName() const;
	std::string	getRealName() const;

	void		setNickName(std::string &nickName);
	void		setRealName(std::string &realName);
};

#endif