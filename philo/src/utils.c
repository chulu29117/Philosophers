/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:29:14 by clu               #+#    #+#             */
/*   Updated: 2025/05/31 21:52:05 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_atoi(const char *str)
{
	long	nb;

	nb = 0;
	if (!str || *str == '\0')
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		if (nb > (INT_MAX / 10) || (nb == (INT_MAX / 10)
			&& (*str - '0') > (INT_MAX % 10)))
			return (-1);
		nb = nb * 10 + (*str - '0');
		str++;
	}
	if (nb == 0)
		return (-1);
	return (nb);
}

/*
** Getting the current time in ms.
** If gettimeofday fails, set stop flag to terminate the program.
** Return the current time in ms since epoch.
*/
long	timestamp(t_table *table)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) < 0)
	{
		table->stop = true;
		return (-1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	cleanup(t_table *table, int clean_mutex)
{
	int	i;

	if (!table)
		return ;
	if (clean_mutex)
	{
		i = -1;
		while (++i < table->n_philos)
			pthread_mutex_destroy(&table->forks[i].hold);
		pthread_mutex_destroy(&table->print_mutex);
		pthread_mutex_destroy(&table->full_mutex);
		pthread_mutex_destroy(&table->time_mutex);
	}
	if (table->forks)
		free(table->forks);
	if (table->philos)
		free(table->philos);
}

int	handle_err(t_table *table, char *msg, int clean)
{
	cleanup(table, clean);
	printf("%s\n", msg);
	return (-1);
}
