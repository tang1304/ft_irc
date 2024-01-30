/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:49:59 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/30 11:36:38 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Client.hpp"

Client::Client() : _chanCount(0), _disconnect(false), _pass(false), _registered(false) { }

Client::~Client(){
}

Client::Client(const Client &other) : _chanCount(0), _disconnect(false), _pass(false), _registered(false) {
	*this = other;
}

const std ::string	&Client::getNickName() const{
	return (_nickName);
}

const std ::string	&Client::getUserName() const{
	return (_userName);
}

const std::string	&Client::getRealName() const{
	return (_realName);
}

const std::string	&Client::getBufferRead() const{
	return (_bufferRead);
}

const std::string	&Client::getBufferSend() const
{
	return (_bufferSend);
}

bool	Client::getDisconnect() const
{
	return (_disconnect);
}

bool	Client::getPass() const
{
	return (_pass);
}

bool	Client::getRegistered() const
{
	return (_registered);
}

const int	&Client::getChanCount() const
{
	return (_chanCount);
}

void	Client::setNickName(const std::string &nickName){
	_nickName = nickName;
}

void	Client::setUsername(const std::string &userName){
	if (userName.size() > USERLEN)
		_userName= userName.substr(0, USERLEN);
	else
		_userName = userName;
}

void	Client::setRealName(const std::string &realName){
	_realName = realName;
}

void	Client::setPass()
{
	_pass = true;
}

void	Client::setDisconnect()
{
	_disconnect = true;
}

void	Client::setRegistered()
{
	_registered = true;
}

void	Client::setBufferRead(const std::string &read, int i){
	if (i)
		_bufferRead += read;
	else
		_bufferRead = read;
}

void	Client::setBufferSend(const std::string &msg)
{
	_bufferSend += msg;
}

void	Client::setChanCount(int i)
{
	if (i)
		_chanCount++;
	else
		_chanCount--;
std::cout << "User chan count in incre " << _chanCount << std::endl;
}
