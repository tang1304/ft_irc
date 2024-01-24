/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:14:10 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/24 16:37:12 by rrebois          ###   ########.fr       */
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
	bool		_pass;
	bool		_registered;

	Client();
	~Client();
	Client(const Client &other);

	std::string	getNickName() const;
	std::string	getRealName() const;
	std::string	getBufferRead() const;
	std::string	getBufferSend() const;
	bool		getRegistered() const;
	bool		getPass() const;

	void		setNickName(const std::string &nickName);
	void		setRealName(const std::string &realName);
	void		setBufferSend(const std::string& msg);
	void		setBufferRead(const std::string &read, int i);
	void		setPass();
	void		setRegistered();
};

#endif