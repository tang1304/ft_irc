/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:49:59 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/23 15:50:06 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Client.hpp"

Client::Client(){
}

Client::~Client(){
}

Client::Client(const Client &other){
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

void	Client::setNickName(const std::string &nickName){
	_nickName = nickName;
}

void	Client::setRealName(const std::string &realName){
	_realName = realName;
}

void	Client::setBufferRead(const std::string &read){
	_bufferRead += read;
}