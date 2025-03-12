/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:34:42 by clu               #+#    #+#             */
/*   Updated: 2025/03/12 10:53:28 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Parses command-line arguments and stores them in 'data'.
 * Returns false if any validation fails.
 */
bool	parse_arguments(int argc, char **argv, t_data *data)
{
	if (!is_num(argv[1]) || !is_num(argv[2])
		|| !is_num(argv[3]) || !is_num(argv[4]))
		return (false);

	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (data->num_philos <= 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (false);
	/* Optional 5th argument: number_of_times_each_philosopher_must_eat */
	if (argc == 6)
	{
		if (!is_num(argv[5]))
			return (false);
		data->num_times_to_eat = ft_atoi(argv[5]);
		if (data->num_times_to_eat <= 0)
			return (false);
	}
	else
		data->num_times_to_eat = 0;

	return (true);
}

/**
 * Checks if a string is numeric (optional sign + digits).
 */
bool	is_num(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (false);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

/**
 * Simple atoi with basic overflow check. Returns -1 if out of range.
 */
int	ft_atoi(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if (result * sign > INT_MAX || result * sign < INT_MIN)
			return (-1);
		i++;
	}
	return ((int)(result * sign));
}

/**
 * Prints usage information if the arguments are invalid.
 */
int	print_usage(void)
{
	printf("Usage: ./philo number_of_philosophers ");
	printf("time_to_die time_to_eat time_to_sleep ");
	printf("[number_of_times_each_philosopher_must_eat]\n");
	return (1);
}
