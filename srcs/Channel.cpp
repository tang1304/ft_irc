/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:40:11 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/15 09:06:08 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"

Channel::Channel(std::string name, std::string key) :
		_name(name), _topic(""), _topicChanger(""), _password(key), _modes(""), \
		_privated(false), _changeTopic(false), _limitUserOnOff(true), _limitUser(USERPERCHAN), \
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
	if (mode.find('+')){
		_modes += mode[1];
	}
	else if (mode.find('-')){
		_modes.erase(_modes.find(mode[1]), 1);
	}
}

void	Channel::addUser(Client &user)
{
	_usersJoin.push_back(user);
	sendToChan(*this, RPL_CMD(user.getName(), user.getUserName(), "JOIN", this->_name));
	_connected++;
	send(user.getClientFd(), user.getBufferSend().c_str(), user.getBufferSend().length(), 0);
	user.setBufferSend("");
}

void	Channel::addChanop(Client &user)
{
	_chanop.push_back(user);
	removeUser(user);
	_connected++;
}

void	Channel::addBanned(Client &user, Client &target)
{
	itVecClient itClient;
	itVecClient itChanop;
	std::string msg;
	std::string comment;

	_banned.push_back(target);
	msg = "you have been banned from " + getName();
	comment = "Banned";
	sendToClient(target, INFO(msg));
	for (itClient = getUsersJoin().begin(); itClient != getUsersJoin().end(); ++itClient)
	{
		if (*itClient == target)
		{
			sendToChan(*this, RPL_CMD(user.getName(), user.getUserName(), "KICK"\
			,getName() + " " + itClient->getName() + " " + comment));
			removeUser(*itClient);
			return ;
		}
	}
	for (itChanop = getChanop().begin(); itChanop != getChanop().end(); ++itChanop)
	{
		if (*itChanop == target)
		{
			sendToChan(*this, RPL_CMD(user.getName(), user.getUserName(), "KICK"\
			,getName() + " " + itChanop->getName() + " " + comment));
			removeChanop(*itChanop);
			return ;
		}
	}
}

void	Channel::removeUser(Client &user)// checker si user pas end
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
	// add if !_connected -> delete channel de server + call destuctor?
}

void	Channel::removeChanop(Client &user) // checker si user pas end
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
	if (_chanop.empty() && _connected >= 1)
		promoteFirstUserToChanop(*_usersJoin.begin());
	// add if !_connected -> delete channel de server + call destuctor?
}

void	Channel::removeBan(Client &user, Client &target)
{
	std::string msgTargetUnbanned;
	std::string msg;

	msgTargetUnbanned = "you have been unbanned from channel " + getName();
	msg = user.getName() + " unbanned " + target.getName() + " from channel " + getName();
	for (itVecClient itBan = getBanned().begin(); itBan != getBanned().end(); ++itBan)
	{
		if (*itBan == target)
		{
			_banned.erase(itBan);
			sendToClient(target, INFO(msgTargetUnbanned));
			sendToChan(*this, INFO(msg));
//			sendToClient(target, RPL_COMMENTUNBANNED(target.getName(), _name));
//			sendToChan(*this, RPL_USERUNBANNED(user.getName(), target.getName(), getName()));
			break ;
		}
	}
}

void	Channel::promoteFirstUserToChanop(Client &user)
{
	_chanop.push_back(user);
	sendToChan(*this, RPL_MODE(user.getName(), user.getUserName(), this->getName(), "+", + "o", user.getName()));
	_usersJoin.erase(_usersJoin.begin());
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

vecClient	&Channel::getBanned()
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
// void	Channel::giveChanopStatus()
