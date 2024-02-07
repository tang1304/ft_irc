/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:21:25 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/07 13:38:25 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

# include "config.hpp"

# define RPL_QUIT(nick) (nick + " left the channel\r\n")

//	1
# define RPL_WELCOME( nickname, user_id) (":" + SERVERNAME + " 001 " + nickname + \
	" :Welcome to the " + SERVERNAME + " Network, " + user_id + "[!" + user_id + "@" + LOCALHOST + "]\r\n")

// 2
# define RPL_YOURHOST(client) (":" + SERVERNAME + " 002 " + client + " :Your host is " + SERVERNAME \
	+ ", running version 1.0\r\n")

// 3
# define RPL_CREATED(client, date) (":" + SERVERNAME + " 003 " + client + " :This server was created " + date + "\r\n")

//4
# define RPL_MYINFO(client) (":" + SERVERNAME + " 004 " + client + " " + SERVERNAME + " 1.0, -+iotlk\r\n")

//5
# define RPL_ISUPPORT(client, tokens) (":" + SERVERNAME + " 005 " + client + " " + tokens + "\r\n")

//324 -> needs to be completed
# define RPL_CHANNELMODEIS(client, channel) (":" + SERVERNAME + " 324 " + client + " " + channel + "\r\n")

// 332
# define RPL_TOPIC(client, channel, topic) (":" + SERVERNAME + " 332 " + client + " " + channel + " :" + topic + "\r\n")

//341
# define RPL_INVITING(client, nick, channel) (":" + SERVERNAME + " 341 " + client + " invited " + nick + " to the channel " + channel + "\r\n")

//403
# define ERR_NOSUCHCHANNEL(client, channel) (":" + SERVERNAME + " 403 " + client + " " + channel + " :No such channel\r\n")

//405
# define ERR_TOOMANYCHANNELS(client, channel) (":" + SERVERNAME + " 405 " + client + " " + channel + " :You have joined too many channels\r\n")

//407
# define ERR_TOOMANYTARGETS(client) (":" + SERVERNAME + " 407 " + client + " :Duplicate recipients. No message delivered\r\n")

//411
# define ERR_NORECIPIENT(client, command) (":" + SERVERNAME + " 411 " + client + " :No recipient given (" + command + ")\r\n")

//412
# define ERR_NOTEXTTOSEND(client) (":" + SERVERNAME + " 412 " + client + " :No text to send\r\n")

//421
# define ERR_UNKNOWNCOMMAND(client, command) (":" + SERVERNAME + " 421 " + client + " " + command + " :Unknown command\r\n")

//431
# define ERR_NONICKNAMEGIVEN(client) (":" + SERVERNAME + " 431 " + client + " :No nickname given\r\n")

//432
# define ERR_ERRONEUSNICKNAME(client, nick) (":" + SERVERNAME + " 432 " + client + " " + nick + " :Erroneus nickname\r\n")

//433
# define ERR_NICKNAMEINUSE(client, nick) (":" + SERVERNAME + " 433 " + client + " " + nick + " :Nickname is already in use\r\n")

//442
# define ERR_NOTONCHANNEL(client, channel) (":" + SERVERNAME + " 442 " + client + " " + channel + " :You're not on that channel\r\n")

//443
# define ERR_USERONCHANNEL(client, nick, channel) (":" + SERVERNAME + " 443 " + client + " " + nick + " " + channel + " :is already on channel\r\n")

// 461
# define ERR_NEEDMOREPARAMS(client, cmd) (":" + SERVERNAME + " 461 " + client + " " + cmd + " :Not enough parameters\r\n")

//462
# define ERR_ALREADYREGISTERED(client) (":" + SERVERNAME + " 462 " + client + " :You may not reregister\r\n")

//464
# define ERR_PASSWDMISMATCH(client) (":" + SERVERNAME + " 464 " + client + " :Password incorrect\r\n")

//471
# define ERR_CHANNELISFULL(client, channel) (":" + SERVERNAME + " 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")

//473
# define ERR_INVITEONLYCHAN(client, channel) (":" + SERVERNAME + " 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")

//474
# define ERR_BANNEDFROMCHAN(client, channel) (":" + SERVERNAME + " 474 " + client + " " + channel + " :Cannot join channel (+b)\r\n")

//475
# define ERR_BADCHANNELKEY(client, channel) (":" + SERVERNAME + " 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")

//476
# define ERR_BADCHANMASK(channel) (":" + SERVERNAME + " 476 " + channel + " :Bad Channel Mask\r\n")

//482
# define ERR_CHANOPRIVSNEEDED(client, channel) (":" + SERVERNAME + " 482 " + client + " " + channel + " :You're not channel operator\r\n")

# define ERROR(error) ("ERROR: " + error + "\r\n")


# define ERR_NOEXISTINGUSER(client, nick) (":" + SERVERNAME +client + " " + nick + " :No existing user\r\n")
# define ERR_PASSFIRST(client) (":" + SERVERNAME +client + " :Must confirm password first\r\n")
# define ERR_NICKFIRST(client) (":" + SERVERNAME +client + " :Must set nickname first\r\n")
# define ERR_NOTREGISTERED(client) (":" + SERVERNAME + " " + client + " :You may register first\r\n")
# define ERR_ALREADYINCHANNEL(client, channel) (":" + SERVERNAME + " " + client + " " + channel + " :User already in channel\r\n")
# define ERR_CMODEUNKNOWNFLAG(channel) (":" + SERVERNAME + " " + channel + " :Unknown MODE flag\r\n")
# define ERR_INVALIDMODEARG(channel, mode) (":" + SERVERNAME + " " + channel + " :Invalid MODE flag arguments " + mode + "\r\n")

# define RPL_USERJOIN(client, channel) (client + " is joining the channel " + channel + "\r\n")
# define RPL_USERLEFT(client, channel) (client + " has left the channel " + channel + "\r\n")

#endif
