/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:14:10 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/25 14:27:11 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Client{
private:


public:

	std::string	_nickName;
	std::string _userName;
	std::string	_realName;
	std::string	_bufferRead;
	std::string	_bufferSend;
	int			_clientFd;
	bool		_disconnect;
	bool		_pass;
	bool		_registered;

	Client();
	~Client();
	Client(const Client &other);

	const std::string	&getNickName() const;
	const std::string	&getUserName() const;
	const std::string	&getRealName() const;
	const std::string	&getBufferRead() const;
	const std::string	&getBufferSend() const;
	bool				getRegistered() const;
	bool				getDisconnect() const;
	bool				getPass() const;

	void				setNickName(const std::string &nickName);
	void				setUsername(const std::string &userName);
	void				setRealName(const std::string &realName);
	void				setBufferSend(const std::string& msg);
	void				setBufferRead(const std::string &read, int i);
	void				setDisconnect();
	void				setPass();
	void				setRegistered();
};

#endif