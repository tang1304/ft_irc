/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:49:59 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/08 10:36:37 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Client.hpp"

Client::Client() : _bufferRead(""), _bufferSend(""), _chanCount(0), _disconnect(false), _pass(false), _registered(false) { }

Client::~Client(){
}

Client::Client(const Client &other) : _chanCount(0), _disconnect(false), _pass(false), _registered(false) {
	*this = other;
}

bool	Client::operator==(Client &rhs){
	if (this->getName() == rhs.getName())
		return (true);
	return (false);
}

const std ::string	&Client::getName() const{
	return (_name);
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

const int	&Client::getFd() const
{
	return (_clientFd);
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

const int	&Client::getClientFd() const
{
	return (_clientFd);
}

void	Client::setName(const std::string &name){
	_name = name;
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
		_bufferRead.clear();
}

void	Client::setBufferSend(const std::string& msg)
{
	if (msg.empty())
		_bufferSend.clear();
	else
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

void	Client::setClientFd(int &fd)
{
	_clientFd = fd;
}
