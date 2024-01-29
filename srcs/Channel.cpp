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

// void	Channel::giveChanopStatus()
