/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:40:11 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/07 17:44:44 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"

Channel::Channel(std::string name, std::string key) :
		_name(name), _topic("No topic"), _password(key), _privated(false),
		_changeTopic(false), _limitUserOnOff(true), _limitUser(USERPERCHAN),
		_connected(0) { }

Channel::~Channel() {}

void	Channel::setId(int i)
{
	_id = i;
}

void	Channel::setPassword(char c, std::string &key)
{
	if (c == '+')
		_password = key;
	if (c == '-')
		_password.clear();
}

void	Channel::setPrivated(char c)
{
	if (c == '+')
		_privated = true;
	else if (c == '-')
		_privated = false;
}

void	Channel::setChangeTopic()
{
	if (_changeTopic)
		_changeTopic = false;
	else
		_changeTopic = true;
}

void	Channel::setLimitUserOnOff(char c, unsigned int i)
{
	if (c == '+')
	{
		_limitUser = i;
		_limitUserOnOff = true;
	}
	else if (c == '-')
		_limitUserOnOff = false;
}

void	Channel::addUser(Client &user)
{
	sendToChan(*this, RPL_USERJOIN(user.getName(), _name));
	_usersJoin.push_back(user);
	_connected++;
	user.setBufferSend(RPL_TOPIC(user.getName(), _name, _topic));
	send(user.getClientFd(), user.getBufferSend().c_str(), user.getBufferSend().length(), 0);
	user.setBufferSend("");
}

void	Channel::addChanop(Client &user)
{
	_chanop.push_back(user);
	_connected++;
}

void	Channel::addBanned(std::string &nickName) //a modifier
{
	_banned.push_back(nickName);
}

void	Channel::removeUser(Client &user)// checker si user pas end
{
	int	index = 0;
	itVecClient	it;

	for (it = _usersJoin.begin(); it != _usersJoin.end(); it++)
	{
		if (user.getName() == it->getName())
			break ;
		index++;
	}
	for (it = _usersJoin.begin(); it != _usersJoin.end(); it++)
	{
		if (user.getName() != it->getName()){
			it->setBufferSend(RPL_USERLEFT(user.getName(), _name));
			send(it->getClientFd(), it->getBufferSend().c_str(), it->getBufferSend().length(), 0);
			it->setBufferSend("");
		}
	}
	for (it = _chanop.begin(); it != _chanop.end(); it++)
	{
		if (user.getName() != it->getName()){
			it->setBufferSend(RPL_USERLEFT(user.getName(), _name));
			send(it->getClientFd(), it->getBufferSend().c_str(), it->getBufferSend().length(), 0);
			it->setBufferSend("");
		}
	}
	_usersJoin.erase(_usersJoin.begin() + index);
	_connected--;
	// add if !_connected -> delete channel de server + call destuctor?
}

void	Channel::removeChanop(Client &user) // checker si user pas end
{
	int	index = 0;

	for (itVecClient it = _chanop.begin(); it != _chanop.end(); it++)
	{
		if (user.getName() == it->getName())
			break ;
		index++;
	}
	_chanop.erase(_chanop.begin() + index);
	_connected--;
	if (!_chanop.size() && _connected >= 1)
		promoteFirstUserToChanop(*_usersJoin.begin());
	// add if !_connected -> delete channel de server + call destuctor?
}

void	Channel::promoteFirstUserToChanop(Client &user)
{
	_chanop.push_back(user);
	_usersJoin.erase(_usersJoin.begin());
}

void	Channel::promoteDemoteUsers(char c, Client &user, Client &target)
{
	if (c == '+' && isItIn(target, getChanop()))
	{
		sendToClient(user, ERR_USERALREADYOP(user.getName(), target.getName(), getName()));
		return ;
	}
	else if (c == '+' && !isItIn(target, getChanop()))
	{
		int index = 0;

		_chanop.push_back(target);
		for (itVecClient it = _usersJoin.begin(); it != _usersJoin.end(); it++)
		{
			if (user.getName() == it->getName())
				break ;
			index++;
		}
		_usersJoin.erase(_usersJoin.begin() + index);
		sendToChan(*this, RPL_USERPROMOTED(user.getName(), target.getName()));
	}
	if (c == '-' && isItIn(target, getUsersJoin()))
	{
		sendToClient(user, ERR_USERALREADYBASICU(user.getName(), target.getName(), getName()));
		return ;
	}
	else if (c == '-' && !isItIn(target, getUsersJoin()))
	{
		_usersJoin.push_back(target);
		int	index = 0;

		for (itVecClient it = _chanop.begin(); it != _chanop.end(); it++)
		{
			if (user.getName() == it->getName())
				break ;
			index++;
		}
		_chanop.erase(_chanop.begin() + index);
		sendToChan(*this, RPL_USERDEMOTED(user.getName(), target.getName()));
	}
}

void	Channel::removeBan(Client &user)
{
	int	index = 0;

	for (itVecStr it = _banned.begin(); it != _banned.end(); it++)
	{
		if (user.getName() == *it)
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

const int	&Channel::getLimitUser() const
{
	return (_limitUser);
}

const bool	&Channel::getLimitUserOnOff() const
{
	return (_limitUserOnOff);
}

const bool	&Channel::getPrivated() const
{
	return (_privated);
}

const int	&Channel::getId() const
{
	return (_id);
}

// void	Channel::giveChanopStatus()
