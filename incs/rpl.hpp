/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:21:25 by tgellon           #+#    #+#             */
/*   Updated: 2024/02/07 09:10:00 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

# include "config.hpp"

# define RPL_QUIT(nick) (nick + " left the channel\r\n")

# define RPL_PRIVMSG(nick, username, msg) (":" + nick + "!" + username + "@" + LOCALHOST + " PRIVMSG : " + msg + "\r\n")

//	1
# define RPL_WELCOME( nickname, user_id) (":" + SERVERNAME + " 001 " + nickname + \
	" :Welcome to the" + SERVERNAME + " Network, " + user_id + "[!" + user_id + "@" + LOCALHOST + "]\r\n")

// 2
# define RPL_YOURHOST(client) (":" + SERVERNAME + " 002 " + client + " :Your host is " + SERVERNAME \
	+ ", running version 1.0\r\n")

// 3
# define RPL_CREATED(client, date) (":" + SERVERNAME + " 003 " + client + " :This server was created " + date + "\r\n")

//4
# define RPL_MYINFO(client) (":" + SERVERNAME + " 004 " + client + " " + SERVERNAME + " 1.0, -+iotlk\r\n")

//5
# define RPL_ISUPPORT(client, tokens) (":" + SERVERNAME + " 005 " + client + " " + tokens + "\r\n")

//372
# define RPL_MOTD(client) (":" + SERVERNAME + " 372 " + client + " : " + MOTD + "\r\n")

//375
# define RPL_MOTDSTART(client) (":" + SERVERNAME + " 375 " + client + " :- " + SERVERNAME + " Message of the day - \r\n")

//376
# define RPL_ENDOFMOTD(client) (":" + SERVERNAME + " 376 " + client +  " :End of /MOTD command.\r\n")

//401
# define ERR_NOSUCHNICK(client, nick) (":" + SERVERNAME + " 401 " + client + " '" + nick + "' :No such nick/channel\r\n")

//403
# define ERR_NOSUCHCHANNEL(client, chan) (":" + SERVERNAME + " 403 " + client + " " + chan + " :No such channel\r\n")

//404
# define ERR_CANNOTSENDTOCHAN(client, channel) (":" + SERVERNAME + " 404 " + client + " :Cannot send to channel\r\n")

//407
# define ERR_TOOMANYTARGETS(client) (":" + SERVERNAME + " 407 " + client + " :Duplicate recipients. No message delivered\r\n")

//411
# define ERR_NORECIPIENT(client, command) (":" + SERVERNAME + " 411 " + client + " :No recipient given (" + command + ")\r\n")

//412
# define ERR_NOTEXTTOSEND(client) (":" + SERVERNAME + " 412 " + client + " :No text to send\r\n")

//442
# define ERR_NOTONCHANNEL(client, chan) (":" + SERVERNAME + " 442 " + client + " " + chan + " :You're not on that channel\r\n")

//461
# define ERR_NEEDMOREPARAMS(client, cmd) (":" + SERVERNAME + " 461 " + client + " " + cmd + " :Not enough parameters\r\n")

# define ERROR(error) ("ERROR: " + error + "\r\n")
# define ERR_NEEDLESSPARAMS " :Too many parameters\n"
# define ERR_UNKNOWNCOMMAND(client, command) (client + " " + command + " :Unknown command\r\n")
# define ERR_PASSWDMISMATCH(client) (client + " :Password incorrect\r\n")
# define ERR_ALREADYREGISTERED(client) (client + " :You may not reregister\r\n")
# define ERR_NONICKNAMEGIVEN(client) (client + " :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(client, nick) (client + " " + nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(client, nick) (client + " " + nick + " :Nickname is already in use\r\n")
# define ERR_TOOMANYCHANNELS(client, channel) (client + " " + channel + " :You have joined too many channels\r\n")
# define ERR_BADCHANNELKEY(client, channel) (client + " " + channel + " :Cannot join channel (+k)\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) (client + " " + channel + " :Cannot join channel (+b)\r\n")
# define ERR_CHANNELISFULL(client, channel) (client + " " + channel + " :Cannot join channel (+l)\r\n")
# define ERR_INVITEONLYCHAN(client, channel) (client + " " + channel + " :Cannot join channel (+i)\r\n")
# define ERR_BADCHANMASK(channel) (channel + " :Bad Channel Mask\r\n")
# define ERR_CHANOPRIVSNEEDED(client, channel) (client + " " + channel + " :You're not channel operator\r\n")
# define ERR_USERONCHANNEL(client, nick, channel) (client + " " + nick + " " + channel + " :is already on channel\r\n")

# define ERR_PASSFIRST(client) (client + " :Must confirm password first\r\n")
# define ERR_NICKFIRST(client) (client + " :Must set nickname first\r\n")
# define ERR_NOTREGISTERED(client) (client + " :You may register first\r\n")

# define RPL_USERJOIN(client, channel) (client + " is joining the channel " + channel + "\r\n")
# define RPL_USERLEFT(client, channel) (client + " has left the channel " + channel + "\r\n")
# define RPL_TOPIC(client, channel, topic) (client + " " + channel + " :" + topic + "\r\n")
#endif
