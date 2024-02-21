/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrebois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:39:37 by rrebois           #+#    #+#             */
/*   Updated: 2024/02/21 11:30:12 by rrebois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/irc.hpp"
#include "../../incs/bot.hpp"

int main(int ac, char **av)
{(void)av;
	if (ac != 3)
		return (std::cerr << RED << USAGEBOT << DEFAULT << std::endl, 1);
	return (0);

}