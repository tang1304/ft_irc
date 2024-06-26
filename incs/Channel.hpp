/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:15:06 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/19 11:34:24 by rrebois          ###   ########.fr       */
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
	std::string	_topic; // add timestamp of when it has been created and update it when changed
	std::string	_topicChanger;
	std::string	_password;
	std::string	_modes;
	vecClient 	_invited;
	vecClient	_usersJoin;
	vecClient	_chanop; // Channel moderators are identified by the channel member prefix ('@' for standard channel operators) next to their nickname whenever it is associated with a channel.
	bool		_privated; // invite only or not / false
	bool		_changeTopic; // false = anyone true = chanops only / false
	bool		_limitUserOnOff;
	int			_limitUser;
	int			_connected; //number of users/chanops connected
	int			_id;
	std::time_t	_timeTopicChange;

public:
	int 				setInvited(Client &user);
	void				setId(int i);
	void				setPassword(char c, std::string &key);
	void				setPrivated(char c);
	void				setChangeTopic(char c, Client &user);
	void				setLimitUserOnOff(char c, unsigned int i);
	void				setTopic(std::string &topic);
	void				setTopicChanger(const std::string &user);
	void				setModes(const std::string &mode);
	void				addUser(Client &user);
	void				addChanop(Client &user);
	void				removeUser(Client &user);
	void				removeChanop(Client &user);
	void				removeInvited(Client &user);
//	void				promoteFirstUserToChanop(Client &user);

	const int			&getConnected() const;
	const std::string	&getPassword() const;
	const std::string	&getName() const;
	const std::string	&getTopic() const;
	const std::string	&getTopicChanger() const;
	const std::string	&getModes() const;
	vecClient			&getUsersJoin();
	vecClient			&getChanop();
	vecClient 			&getInvited();
	const int			&getLimitUser() const;
	const bool			&getLimitUserOnOff() const;
	const bool			&getPrivated() const;
	const bool			&getChangeTopic() const;
	const int			&getId() const;
	const std::time_t	&getTimeTopicChange();

	Channel(std::string name, std::string key);
	~Channel();
};

#endif
