/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:40:11 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/21 15:13:31 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"

Channel::Channel(std::string name, std::string key) :
		_name(name), _topic(""), _topicChanger(""), _password(key), _modes(""), \
		_privated(false), _changeTopic(false), _limitUserOnOff(true), _limitUser(USERPERCHAN), \
		_connected(0), _id(0), _timeTopicChange(std::time(NULL)) { }

Channel::~Channel() {}

void	Channel::setId(int i)
{
	_id = i;
}

int	Channel::setInvited(Client &user)
{
	itVecClient	target = findIt(user, _invited);
	if (target == _invited.end())
	{
		_invited.push_back(user);
		return (0);
	}
	return (1);
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

void	Channel::setChangeTopic(char c, Client &user)
{
	(void)user;
	if (c == '+')
	{
		std::string error = this->getName() + " topic can be modified only by chanop users";
		_changeTopic = true;
	}
	else
	{
		std::string error2 = this->getName() + " topic can be modified by any user";
		_changeTopic = false;
	}
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

void	Channel::setTopic(std::string &topic){
	if (topic == "")
		_topic.clear();
	else
	{
		_topic = topic;
		_timeTopicChange = std::time(NULL);
	}
}

void	Channel::setTopicChanger(const std::string &user){
		_topicChanger = user;
}

void	Channel::setModes(const std::string &mode){
	if (mode.find('+') != std::string::npos){
		_modes += mode[1];
	}
	else if (mode.find('-') != std::string::npos){
		_modes.erase(_modes.find(mode[1]), 1);
	}
}

void	Channel::addUser(Client &user)
{
	_usersJoin.push_back(user);
	// sendToChan(*this, RPL_CMD(user.getName(), user.getUserName(), "JOIN", this->_name));
	_connected++;
}

void	Channel::addChanop(Client &user)
{
	_chanop.push_back(user);
	_connected++;
	removeUser(user);
}

void	Channel::removeUser(Client &user)
{
	itVecClient	it;

	for (it = _usersJoin.begin(); it != _usersJoin.end(); it++)
	{
		if (user.getName() == it->getName())
		{
			_usersJoin.erase(it);
			_connected--;
			break ;
		}
	}
}

void	Channel::removeChanop(Client &user) 
{
	for (itVecClient it = _chanop.begin(); it != _chanop.end(); it++)
	{
		if (user.getName() == it->getName())
		{
			_chanop.erase(it);
			_connected--;
			break ;
		}
	}
	if (_chanop.empty() && _connected > 0)
	{
		sendToChan(*this, RPL_CMD(user.getName(), user.getUserName(), "MODE", this->getName() + " +o " + _usersJoin.begin()->getName()));
		addChanop(*_usersJoin.begin());
	}
}

void	Channel::removeInvited(Client &user)
{
	itVecClient target = findIt(user, _invited);
	_invited.erase(target);
}

//void	Channel::promoteFirstUserToChanop(Client &user)
//{
//	sendToChan(*this, RPL_MODE(user.getName(), user.getUserName(), this->getName(), "+", + "o", user.getName()));
//	_chanop.push_back(user);
//	_usersJoin.erase(_usersJoin.begin());
//}

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

vecClient	&Channel::getInvited()
{
	return (_invited);
}

const std::string	&Channel::getPassword() const
{
	return (_password);
}

const std::string	&Channel::getTopic() const
{
	return (_topic);
}

const std::string	&Channel::getTopicChanger() const
{
	return (_topicChanger);
}

const std::string	&Channel::getModes() const
{
	return (_modes);
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

const bool	&Channel::getChangeTopic() const
{
	return (_changeTopic);
}

const int	&Channel::getId() const
{
	return (_id);
}

const std::time_t	&Channel::getTimeTopicChange()
{
	return (_timeTopicChange);
}
