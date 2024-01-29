/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <rrebois@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 09:13:54 by rrebois           #+#    #+#             */
/*   Updated: 2024/01/29 09:13:54 by rrebois          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/irc.hpp"

static int	check_chan_first_char(vecPair pair)
{
	bool	valid;

	valid = false;
	for (vecPair::iterator	it = pair.begin(); it != pair.end(); it++)
	{
		if (it->first[0] == '#' || it->first[0] == '&')
			valid = true;
	}
	if (valid == true)
		return (1);
	return (0);
}

static void	user_join_chan()
{
	return ;
}

static int check_chan_name(std::string name)
{
	for (size_t i = 0; i < name.size(); i++)
	{
		if (name.c_str()[i] == 7 || name.c_str()[i] == ' ')
			return (0);
	}
	return (1);
}

static int	user_create_chan(std::string chan, std::string key, Client &user)
{(void)user;(void)key;(void)chan;
	if (!check_chan_name(chan))
		return (0);
	return (1);
}

int	join_cmd(int fd, vecStr &cmd, Server &serv)
{
	Client	user;
	vecPair	chanPass;
	bool	exists;

	chanPass = create_pair_cmd(cmd);
	user = serv.getClient(fd);

// TEST
	for (vecPair::iterator it = chanPass.begin(); it != chanPass.end(); it++)
	{
		std::cout << "Chan: " << it->first << " ";
		if (it->second.empty())
			std::cout << std::endl;
		else
			std::cout << "mdp: " << it->second << std::endl;
	}
// TEST END
	if (!check_chan_first_char(chanPass) && cmd.size() < 3)
		return (ERR_NEEDMOREPARAMS(user.getNickName(), cmd[1]), 1);
	for (vecPair::iterator it = chanPass.begin(); it != chanPass.end(); it++)
	{
		exists = false;
		for (vecChan::iterator itc = serv.getChannelList().begin(); itc != serv.getChannelList().end(); itc++)
		{
			// if (it->first == *itc.getName())
			// 	exists = true;
		}
		if (exists)
			user_join_chan();
		else
		{
			if (!user_create_chan(it->first, it->second, user))
				return (1); // ERREUR a ajouter
		}
	}
	// for (vecPair::iterator it = chanPass.begin(); it != chanPass.end(); it++)
	return (0);
}
