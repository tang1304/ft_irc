/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:15:06 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/30 11:11:49 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"
# include "Client.hpp"

class Client;

class Channel{
private:
	std::string	_name;
	std::string	_topic;
	std::string	_password;
	vecCli		_usersJoin;
	vecCli		_chanop;
	bool		_privated; // invite only or not / false
	bool		_changeTopic; // false = anyone true = chanops only / false
	// bool		_passOnOff; // on?
	int			_connected; //number of users/chanops connected
	int			_id;

public:
	void				setId(int i);
	void				setPrivateChan();
	void				setChangeTopic();

	void				addUser(Client &user);
	void				addChanop(Client &user);
	void				removeUser(Client &user);
	void				removeChanop(Client &user);

	const int			&getConnected() const;
	const std::string	&getPassword() const;
	const vecCli		&getUsersJoin() const;
	const vecCli		&getChanop() const;
	const std::string	&getName() const;

	// void	giveChanopStatus();

	Channel(std::string name, std::string key);
	~Channel();
};

#endif
