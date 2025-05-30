/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:02:51 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 12:23:28 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_atoi(const char *str)
{
	int	nb;
	
	nb = 0;
	while (*str >= '0' && *str <= '9')
	{
		if (nb > INT_MAX || (nb == INT_MAX && (*str - '0') > 7))
			return (-1);
		nb = nb * 10 + (*str - '0');
		str++;
	}
	if (nb == 0)
		return (-1);
	return (nb);
}

bool	validate(char *arg)
{
	if (!arg || !*arg)
		return (handle_err(NULL, "Empty arguments", 0), false);
	while (*arg)
	{
		if (*arg < '0' || *arg > '9')
			return (handle_err(NULL, "Positive INT only in arguments", 0), false);
		arg++;
	}
	return (true);
}
