/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:40:11 by rrebois           #+#    #+#             */
/*   Updated: 2024/01/29 15:40:11 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"

Channel::Channel(std::string name, std::string key) :
		_name(name), _password(key), _privated(false), _changeTopic(false),
		_connected(0) {}

Channel::~Channel() {}

const std::string	&Channel::getName() const
{
	return (_name);
}

const int	&Channel::getConnected() const
{
	return (_connected);
}

const vecCli	&Channel::getUsersJoin() const
{
	return (_usersJoin);
}

const vecCli	&Channel::getChanop() const
{
	return (_chanop);
}

const std::string	&Channel::getPassword() const
{
	return (_password);
}

void	Channel::setPrivateChan()
{
	if (!_privated)
		_privated = true;
	else
		_privated = false;
}

void	Channel::setChangeTopic()
{
	if (_changeTopic)
		_changeTopic = false;
	else
		_changeTopic = true;
}

void	Channel::setId(int i)
{
	_id = i;
}

void	Channel::addUser(Client &user)
{
	_usersJoin.push_back(user);
	_connected++;
}

void	Channel::addChanop(Client &user)
{
	_chanop.push_back(user);
	_connected++;
}

void	Channel::removeUser(Client &user)
{
	int	index = 0;

	for (vecCli::iterator it = _usersJoin.begin(); it != _usersJoin.end(); it++)
	{
		if (user.getNickName() == it->getNickName())
			break ;
		index++;
	}
	_usersJoin.erase(_usersJoin.begin() + index);
	_connected--;
	// add if !_connected -> delete channel de server + call destuctor?
}

void	Channel::removeChanop(Client &user)
{
	int	index = 0;

	for (vecCli::iterator it = _chanop.begin(); it != _chanop.end(); it++)
	{
		if (user.getNickName() == it->getNickName())
			break ;
		index++;
	}
	_chanop.erase(_chanop.begin() + index);
	_connected--;
	// add if !_connected -> delete channel de server + call destuctor?
}

// void	Channel::giveChanopStatus()
