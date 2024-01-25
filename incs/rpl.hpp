/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:21:25 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/25 11:00:53 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define ERR_NEEDMOREPARAMS(client, cmd) (client + " " + cmd + " :Not enough parameters\r\n")
# define ERR_NEEDLESSPARAMS " :Too many parameters\n"
# define ERR_PASSWDMISMATCH " :Password incorrect\n"
# define ERR_ALREADYREGISTERED " :You may not reregister\n"
# define ERRUSER0 "Error.\nMust set password first\n"
# define INVCMD "Invalid command\n"
# define ERR_UNKNOWNCOMMAND(client, command) (client + " " + command + " :Unknown command\r\n")

#endif