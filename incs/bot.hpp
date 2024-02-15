/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:32:41 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/15 08:17:15 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "irc.hpp"
class bot
{
public:
private:
	bot();
	~bot();
	static std::vector<std::string>	words;
};
#endif