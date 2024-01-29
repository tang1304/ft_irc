/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:15:06 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/29 15:38:23 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"
# include "Client.hpp"

class Client;

class Channel{
private:
	std::string				_topic;
	std::string				_password;
	std::vector	<Client>	_usersJoin;
	std::vector	<Client>	_chanop;
	bool					_privated; // invite only or not / false
	bool					_changeTopic; // false = anyone true = chanops only / false
	// bool					_passOnOff; // on?
	int						_connected; //number of users/chanops connected

public:
	void		setPrivateChan();
	void		setChangeTopic();
	vecClient	&getClientsVec();

	void	giveChanopStatus();
};

#endif
