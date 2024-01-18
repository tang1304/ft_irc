/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:49:59 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/18 12:53:56 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(){
}

Client::~Client(){
}

Client::Client(const Client &other){
	*this = other;
}

Client	&Client::operator=(const Client &other){
	if (this != &other){
		this->_nickName = other._nickName;
		this->_realName = other._realName;
		this->_bufferRead = other._bufferRead;
		this->_bufferSend = other._bufferSend;
	}
	return (*this);
}
