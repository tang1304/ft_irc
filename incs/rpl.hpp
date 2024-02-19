/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:21:25 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/19 13:26:20 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

# include "config.hpp"

# define RPL_CMD(nick, username, cmd, args) (":" + nick + "!" + username + "@" + LOCALHOST + " " + cmd + " " + args + "\r\n")

# define RPL_MODE(nick, username, chan, sign, mode ,args) (":" + nick + "!" + username + "@" + LOCALHOST + " MODE " + chan + " " \
+ sign + mode + " " + args + "\r\n")

//1
# define RPL_WELCOME(nickname, user_id) (":" + SERVERNAME + " 001 " + nickname + \
	" :Welcome to the " + SERVERNAME + " Network, " + user_id + "[!" + user_id + "@" + LOCALHOST + "]\r\n")

//2
# define RPL_YOURHOST(client) (":" + SERVERNAME + " 002 " + client + " :Your host is " + SERVERNAME \
	+ ", running version 1.0\r\n")

//3
# define RPL_CREATED(client, date) (":" + SERVERNAME + " 003 " + client + " :This server was created " + date + "\r\n")

//4
# define RPL_MYINFO(client) (":" + SERVERNAME + " 004 " + client + " " + SERVERNAME + " 1.0, o, -+iotlk\r\n")

//5
# define RPL_ISUPPORT(client, tokens) (":" + SERVERNAME + " 005 " + client + " " + TOKENS + " :are supported by this server" + "\r\n")

//315
# define RPL_ENDOFWHO(client, mask) (":" + SERVERNAME + " 315 " + client + " "+ mask + " :End of WHO list\r\n")

//322
# define RPL_LIST(client, channel, count, topic) (":" + SERVERNAME + " 322 " + client + " "+ channel + " " + count + " :" + topic + "\r\n")

//323
# define RPL_LISTEND(client) (":" + SERVERNAME+ " 323 " + client + " :End of /LIST\r\n")

//324
# define RPL_CHANNELMODEIS(client, channel, modes) (":" + SERVERNAME + " 324 " + client + " " + channel + " " + modes + "\r\n")

//331
# define RPL_NOTOPIC(client, channel) (":" + SERVERNAME + " 331 " + client + " " + channel + " :No topic is set\r\n")

//332
# define RPL_TOPIC(client, channel, topic) (":" + SERVERNAME + " 332 " + client + " " + channel + " " + topic + "\r\n")

//333
# define RPL_TOPICWHOTIME(client, chan, nick, setat, date) (":" + SERVERNAME + " 333 " + client + ": " + chan + " " + nick + " " + setat +  " " + date + "\r\n")

//341
# define RPL_INVITING(client, nick, channel) (":" + SERVERNAME + " 341 " + client + " invited " + nick + " to the channel " + channel + "\r\n")

//352
# define RPL_WHOREPLY(client, channel, userName, nick, flags, realName) (":" + SERVERNAME + " 352 " + client + " " + channel + \
	" " + userName + " " + LOCALHOST + " " + SERVERNAME + " " + nick + " " + flags + " " + realName + "\r\n")

//353
# define RPL_NAMREPLY(client, chan, list) (":" + SERVERNAME + " 353 " + client + " = " + chan + " " + list + "\r\n")


//366
# define RPL_ENDOFNAMES(client, chan) (":" + SERVERNAME + " 366 " + client + " :End of /NAMES list\r\n")

//372
# define RPL_MOTD(client) (":" + SERVERNAME + " 372 " + client + " : " + MOTD + "\r\n")

//375
# define RPL_MOTDSTART(client) (":" + SERVERNAME + " 375 " + client + " :- " + SERVERNAME + " Message of the day - \r\n")

//376
# define RPL_ENDOFMOTD(client) (":" + SERVERNAME + " 376 " + client +  " :End of /MOTD command.\r\n")

//401
# define ERR_NOSUCHNICK(client, nick) (":" + SERVERNAME + " 401 " + client + " '" + nick + "' :No such nick/channel\r\n")

//403
# define ERR_NOSUCHCHANNEL(client, channel) (":" + SERVERNAME + " 403 " + client + " " + channel + " :No such channel\r\n")

//404
# define ERR_CANNOTSENDTOCHAN(client, channel) (":" + SERVERNAME + " 404 " + client + " :Cannot send to channel\r\n")

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

//441
# define ERR_USERNOTINCHANNEL(client, nick, channel) (":" + SERVERNAME + " 441 " + client + " " + nick + " " + channel + " :They aren't on that channel\r\n")

//442
# define ERR_NOTONCHANNEL(client, channel) (":" + SERVERNAME + " 442 " + client + " " + channel + " :You're not on that channel\r\n")

//443
# define ERR_USERONCHANNEL(client, nick, channel) (":" + SERVERNAME + " 443 " + client + " " + nick + " " + channel + " :is already on channel\r\n")

//461
# define ERR_NEEDMOREPARAMS(client, cmd) (":" + SERVERNAME + " 461 " + client + " " + cmd + " :Not enough parameters\r\n")

//462
# define ERR_ALREADYREGISTERED(client) (":" + SERVERNAME + " 462 " + client + " :You may not reregister\r\n")

//464
# define ERR_PASSWDMISMATCH(client) (":" + SERVERNAME + " 464 " + client + " :Password incorrect\r\n")

//471
# define ERR_CHANNELISFULL(client, channel) (":" + SERVERNAME + " 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")

//473
# define ERR_INVITEONLYCHAN(client, channel) (":" + SERVERNAME + " 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")

//475
# define ERR_BADCHANNELKEY(client, channel) (":" + SERVERNAME + " 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")

//476
# define ERR_BADCHANMASK(channel) (":" + SERVERNAME + " 476 " + channel + " :Bad Channel Mask\r\n")

//482
# define ERR_CHANOPRIVSNEEDED(client, channel) (":" + SERVERNAME + " 482 " + client + " " + channel + " :You're not channel operator\r\n")

//525
# define ERR_INVALIDKEY(client, channel) (":" + SERVERNAME + " 525 " + client + " " + channel + " :Key is not well-formed (alphanumeric characters only)\r\n")

//696
# define ERR_INVALIDMODEPARAM(client, channel, mode, param) (":" + SERVERNAME + " 696 " + client + " " + \
			channel + " " + mode + " " + param + " :Invalid parameter\r\n")

# define ERROR(error) ("ERROR: " + error + "\r\n")
# define INFO(msg) ("INFO: " + msg + "\r\n")

#endif
