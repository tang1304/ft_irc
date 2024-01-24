/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:20:16 by tgellon           #+#    #+#             */
/*   Updated: 2024/01/24 14:49:57 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include "config.hpp"
# include "Server.hpp"

int			checkArgs(const std::string &port, const std::string &password);
std::size_t	len(std::string s);

#endif