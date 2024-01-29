/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:15:06 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/29 16:11:08 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"
# include "Client.hpp"

class Client;

class Channel{
private:
	std::string				_name;
	std::string				_topic;
	std::string				_password;
	std::vector	<Client>	_usersJoin;
	std::vector	<Client>	_chanop;
	bool					_privated; // invite only or not / false
	bool					_changeTopic; // false = anyone true = chanops only / false
	// bool					_passOnOff; // on?
	int						_connected; //number of users/chanops connected
	int						_id;

public:
	void	setPrivateChan();
	void	setChangeTopic();
	// void	giveChanopStatus();
	std::string	&getName();

	Channel(std::string name);
	~Channel();
};

#endif
