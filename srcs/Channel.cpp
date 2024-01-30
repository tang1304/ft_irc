/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:40:11 by rrebois           #+#    #+#             */
/*   Updated: 2024/01/30 09:09:56 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name), _privated(false), _changeTopic(false) {}

Channel::~Channel() {}

std::string	&Channel::getName()
{
	return (_name);
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

vecClient	&Channel::getClientsVec(){
	return (_usersJoin);
}
