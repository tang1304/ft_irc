/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:40:11 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/02 12:35:55 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"

Channel::Channel(std::string name, std::string key) :
		_name(name), _topic("No topic"), _password(key), _privated(false),
		_changeTopic(false), _limitUser(false), _connected(0) {}

Channel::~Channel() {}

void	Channel::setId(int i)
{
	_id = i;
}

void	Channel::setPrivated()
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

void	Channel::setLimitUser()
{
	if (_limitUser)
		_limitUser = false;
	else
		_limitUser = true;
}

void	Channel::addUser(Client &user)
{
	vecClient::iterator	it;

	for (it = _usersJoin.begin(); it != _usersJoin.end(); it++)
	{
		it->setBufferSend(RPL_USERJOIN(user.getNickName(), _name));
		send(it->_clientFd, it->getBufferSend().c_str(), it->getBufferSend().length(), 0);
		it->setBufferSend("");
	}
	for (it = _chanop.begin(); it != _chanop.end(); it++)
	{
		it->setBufferSend(RPL_USERJOIN(user.getNickName(), _name));
		send(it->_clientFd, it->getBufferSend().c_str(), it->getBufferSend().length(), 0);
		it->setBufferSend("");
	}
	_usersJoin.push_back(user);
	_connected++;
	user.setBufferSend(RPL_TOPIC(user.getNickName(), _name, _topic));
	send(user._clientFd, user.getBufferSend().c_str(), user.getBufferSend().length(), 0);
	user.setBufferSend("");
}

void	Channel::addChanop(Client &user)
{
	_chanop.push_back(user);
	_connected++;
}

void	Channel::addBanned(std::string nickName)
{
	_banned.push_back(nickName);
}

void	Channel::addInvited(std::string nickName)
{
	_invited.push_back(nickName);
}

void	Channel::removeUser(Client &user)
{
	int	index = 0;
	vecClient::iterator	it;

	for (it = _usersJoin.begin(); it != _usersJoin.end(); it++)
	{
		if (user.getNickName() == it->getNickName())
			break ;
		index++;
	}
	for (it = _usersJoin.begin(); it != _usersJoin.end(); it++)
	{
		if (user.getNickName() != it->getNickName()){
			it->setBufferSend(RPL_USERLEFT(user.getNickName(), _name));
			send(it->_clientFd, it->getBufferSend().c_str(), it->getBufferSend().length(), 0);
			it->setBufferSend("");
		}
	}
	for (it = _chanop.begin(); it != _chanop.end(); it++)
	{
		if (user.getNickName() != it->getNickName()){
			it->setBufferSend(RPL_USERLEFT(user.getNickName(), _name));
			send(it->_clientFd, it->getBufferSend().c_str(), it->getBufferSend().length(), 0);
			it->setBufferSend("");
		}
	}
	_usersJoin.erase(_usersJoin.begin() + index);
	_connected--;
	// add if !_connected -> delete channel de server + call destuctor?
}

void	Channel::removeChanop(Client &user)
{
	int					index = 0;
	vecClient::iterator	it;

	for (it = _chanop.begin(); it != _chanop.end(); it++)
	{
		if (user.getNickName() == it->getNickName())
			break ;
		index++;
	}
	_chanop.erase(_chanop.begin() + index);
	_connected--;
	// for (it = _usersJoin.begin(); it != _usersJoin.end(); it++)
	// {
	// 	if (username != it->getNickName()){
	// 		it->setBufferSend(RPL_USERLEFT(username, _name));
	// 		send(it->_clientFd, it->getBufferSend().c_str(), it->getBufferSend().length(), 0);
	// 		it->setBufferSend("");
	// 	}
	// }
	// for (it = _chanop.begin(); it != _chanop.end(); it++)
	// {
	// 	if (username != it->getNickName()){
	// 		it->setBufferSend(RPL_USERLEFT(username, _name));
	// 		send(it->_clientFd, it->getBufferSend().c_str(), it->getBufferSend().length(), 0);
	// 		it->setBufferSend("");
	// 	}
	// }
	if (!_chanop.size() && _connected >= 1)
		promoteUserToChanop(*_usersJoin.begin());
	// add if !_connected -> delete channel de server + call destuctor?
}

void	Channel::promoteUserToChanop(Client &user)
{
	_chanop.push_back(user);
	_usersJoin.erase(_usersJoin.begin());
}

void	Channel::removeBan(Client &user)
{
	int	index = 0;

	for (vecStr::iterator it = _banned.begin(); it != _banned.end(); it++)
	{
		if (user.getNickName() == *it)
			break ;
		index++;
	}
	_banned.erase(_banned.begin() + index);
}

const std::string	&Channel::getName() const
{
	return (_name);
}

const int	&Channel::getConnected() const
{
	return (_connected);
}

vecClient	&Channel::getUsersJoin()
{
	return (_usersJoin);
}

vecClient	&Channel::getChanop()
{
	return (_chanop);
}

const std::string	&Channel::getPassword() const
{
	return (_password);
}

const vecStr	&Channel::getBanned() const
{
	return (_banned);
}

const vecStr	&Channel::getInvited() const
{
	return (_invited);
}

const bool	&Channel::getLimitUser() const
{
	return (_limitUser);
}

const bool	&Channel::getPrivated() const
{
	return (_privated);
}
// void	Channel::giveChanopStatus()

const int	&Channel::getId() const
{
	return (_id);
}