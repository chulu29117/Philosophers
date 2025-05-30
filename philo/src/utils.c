/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:29:14 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 12:26:22 by clu              ###   ########.fr       */
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
	
	i = -1;
	if (table)
	{
		if (table->philos)
		{
			free(table->philos);
			table->philos = NULL;
		}
		if (table->forks)
		{
			while (++i < table->n_philos)
				pthread_mutex_destroy(&table->forks[i].hold);
			free(table->forks);
			table->forks = NULL;
		}
		if (cleann_mutex)
			pthread_mutex_destroy(&table->print_mutex);
	}
}

int	handle_err(t_table *table, char *msg, int clean)
{
	cleanup(table, clean);
	printf("%s\n", msg);
	return (-1);
}
