/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:49:17 by clu               #+#    #+#             */
/*   Updated: 2025/05/29 00:51:23 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** Convert string to integer, with bounds checking
*/
int	ft_atoi(const char *str)
{
	int					sign;
	unsigned long int	res;

	sign = 1;
	res = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
		if (res > LONG_MAX && sign == 1)
			return (-1);
		if (res > LONG_MAX && sign == -1)
			return (0);
	}
	return ((int)(res * sign));
}

bool	parse_args(int ac, char **av, t_data *data)
{
	int	i;

	if (ac != 5 && ac != 6)
		return (false);
	i = 1;
	while (i < ac)
	{
		if (av[i][0] == '\0')
			return (false);
		if ((av[i][0] < '0' || av[i][0] > '9'))
			return (false);
		i++;
	}
	data->num_philos = ft_atoi(av[1]);
	data->t_to_die = ft_atoi(av[2]);
	data->t_to_eat = ft_atoi(av[3]);
	data->t_to_sleep = ft_atoi(av[4]);
	if (data->num_philos < 1 || data->t_to_die < 0 || data->t_to_eat < 0
		|| data->t_to_sleep < 0)
		return (false);
	if (ac == 6)
	{
		data->max_meals = ft_atoi(av[5]);
		if (data->max_meals < 0)
			return (false);
	}
	else
		data->max_meals = 0;
	return (true);
}
