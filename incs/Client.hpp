/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:14:10 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/07 11:08:22 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Client{
private:
	std::string	_name;
	std::string _userName;
	std::string	_realName;
	std::string	_bufferRead;
	std::string	_bufferSend;
	int			_clientFd;
	int			_chanCount;
	bool		_disconnect;
	bool		_pass;
	bool		_registered;

public:

	Client();
	~Client();
	Client(const Client &other);

	bool	operator==(Client &rhs) const;

	const std::string	&getName() const;
	const std::string	&getUserName() const;
	const std::string	&getRealName() const;
	const std::string	&getBufferRead() const;
	const std::string	&getBufferSend() const;
	const int			&getFd() const;
	bool				getRegistered() const;
	bool				getDisconnect() const;
	bool				getPass() const;
	const int			&getChanCount() const;
	const int			&getClientFd() const;

	void				setName(const std::string &nickName);
	void				setUsername(const std::string &userName);
	void				setRealName(const std::string &realName);
	void				setBufferSend(const std::string& msg);
	void				setBufferRead(const std::string &read, int i);
	void				setDisconnect();
	void				setPass();
	void				setRegistered();
	void				setChanCount(int i);
	void				setClientFd(int &fd);
};

#endif
