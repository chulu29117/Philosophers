/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:29:14 by clu               #+#    #+#             */
/*   Updated: 2025/05/31 15:08:24 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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
