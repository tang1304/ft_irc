/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 16:15:06 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/07 17:12:22 by rrebois          ###   ########.fr       */
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
	std::string	_password;
	vecClient	_usersJoin;
	vecClient	_chanop; // Channel moderators are identified by the channel member prefix ('@' for standard channel operators, '%' for halfops) next to their nickname whenever it is associated with a channel (e.g. replies to the NAMES, WHO, and WHOIS commands).
	vecStr		_banned; // peut etre a mettre dans client
	bool		_privated; // invite only or not / false
	bool		_changeTopic; // false = anyone true = chanops only / false
	bool		_limitUserOnOff;
	int			_limitUser;
//	bool		_passOnOff; // on?
	int			_connected; //number of users/chanops connected
	int			_id;

public:
	void				setId(int i);
	void				setPassword(char c, std::string &key);
	void				setPrivated(char c);
	void				setChangeTopic();
	void				setLimitUserOnOff(char c, unsigned int i);
	void				addUser(Client &user);
	void				addChanop(Client &user);
	void				addBanned(std::string &nickName);// a modif, add client
	void				removeUser(Client &user);
	void				removeChanop(Client &user);
	void				removeBan(Client &user);
	void				promoteFirstUserToChanop(Client &user);
	void				promoteDemoteUsers(char c, Client &user, Client &target);

	const int			&getConnected() const;
	const std::string	&getPassword() const;
	const std::string	&getName() const;
	vecClient			&getUsersJoin();
	vecClient			&getChanop();
	const vecStr		&getBanned() const; // a modif, add client
	const int			&getLimitUser() const;
	const bool			&getLimitUserOnOff() const;
	const bool			&getPrivated() const;
	const int			&getId() const;
//	void				getModeFunc(std::string mode) const;
	// void	giveChanopStatus();

	Channel(std::string name, std::string key);
	~Channel();
};

#endif
