/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:49:59 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/24 16:54:13 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Client.hpp"

Client::Client() : _pass(false), _registered(false) { }

Client::~Client(){
}

Client::Client(const Client &other) : _pass(false), _registered(false) {
	*this = other;
}

std ::string	Client::getNickName() const{
	return (_nickName);
}

std::string	Client::getRealName() const{
	return (_realName);
}

std::string	Client::getBufferRead() const{
	return (_bufferRead);
}

std::string	Client::getBufferSend() const
{
	return (_bufferSend);
}

void	Client::setNickName(const std::string &nickName){
	_nickName = nickName;
}

void	Client::setRealName(const std::string &realName){
	_realName = realName;
}

void	Client::setPass()
{
	_pass = true;
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

void	Client::setBufferSend(const std::string& msg)
{std::cout << "test set: " << msg << std::endl;
	_bufferSend += msg;
}

bool	Client::getPass() const
{
	return (_pass);
}

bool	Client::getRegistered() const
{
	return (_registered);
}