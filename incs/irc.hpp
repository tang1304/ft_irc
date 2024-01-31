/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgellon <tgellon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:20:16 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/31 10:52:04 by tgellon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include "config.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "rpl.hpp"

class Server;

int			checkArgs(const std::string &port, const std::string &password);
size_t		len(std::string s);
vecStr		splitCmds(std::string &input, const std::string &delimiter);
vecVecStr	splitCmd(vecStr &cmds, const std::string &delimiter);

#endif