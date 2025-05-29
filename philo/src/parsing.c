/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:49:17 by clu               #+#    #+#             */
/*   Updated: 2025/05/29 17:04:21 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** Convert string to integer, with bounds checking
*/
int	ft_atoi(const char *str)
{
	int	i;
	int	num;

	i = 0;
	num = 0;
	// Check for empty string
	if (!str[0])
		return (-1);
	// Only allow digits, no signs or whitespace
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	// Convert to number with simple overflow check
	i = 0;
	while (str[i])
	{
		if (num > INT_MAX / 10
			|| (num == INT_MAX / 10 && (str[i] - '0') > INT_MAX % 10))
			return (-1);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num);
}

bool	parse_args(int ac, char **av, t_data *data)
{
	if (ac != 5 && ac != 6)
		return (false);

	// Parse arguments - ft_atoi now handles validation
	data->num_philos = ft_atoi(av[1]);
	data->t_to_die = ft_atoi(av[2]);
	data->t_to_eat = ft_atoi(av[3]);
	data->t_to_sleep = ft_atoi(av[4]);
	// Check for parsing errors
	if (data->num_philos == -1 || data->t_to_die == -1
		|| data->t_to_eat == -1 || data->t_to_sleep == -1)
		return (false);
	// Validate logical constraints
	if (data->num_philos <= 0 || data->t_to_die <= 0
		|| data->t_to_eat <= 0 || data->t_to_sleep <= 0)
		return (false);
	if (ac == 6)
	{
		data->max_meals = ft_atoi(av[5]);
		if (data->max_meals <= 0)
			return (false);
	}
	if (data->num_philos > 1000)
	{
		printf("Too many philosophers! Maximum is 1000.\n");
		return (false);
	}
	return (true);
}
