/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:21:25 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/30 15:52:02 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define ERR_NEEDMOREPARAMS(client, cmd) (client + " " + cmd + " :Not enough parameters\r\n")
# define ERR_PASSWDMISMATCH(client) (client + " :Password incorrect\r\n")
# define ERR_ALREADYREGISTERED(client) (client + " :You may not reregister\r\n")
# define ERR_NONICKNAMEGIVEN(client) (client + " :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(client, nick) (client + " " + nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(client, nick) (client + " " + nick + " :Nickname is already in use\r\n")
# define ERRUSER0(client) (client + " :Must set password first\r\n")
# define ERR_TOOMANYCHANNELS(client, channel) (client + " " + channel + " :You have joined too many channels\r\n")
# define ERR_BADCHANNELKEY(client, channel) (client + " " + channel + " :Cannot join channel (+k)\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) (client + " " + channel + " :Cannot join channel (+b)\r\n")
# define ERR_CHANNELISFULL(client, channel) (client + " " + channel + " :Cannot join channel (+l)\r\n")
# define ERR_INVITEONLYCHAN(client, channel) (client + " " + channel + " :Cannot join channel (+i)\r\n")
# define ERR_BADCHANMASK(channel) (channel + " :Bad Channel Mask\r\n")

# define ERR_PASSFIRST(client) (client + " :Must confirm password first\r\n")
# define ERR_NICKFIRST(client) (client + " :Must set nickname first\r\n")
#endif
