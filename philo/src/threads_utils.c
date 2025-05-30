/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:09:46 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 23:31:24 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	single_philo(t_philo *philos)
{
	philos->l_fork->free = true;
	pthread_mutex_unlock(&philos->l_fork->hold);
	ft_usleep(philos, philos->t_to_die);
}

bool	try_l_fork(t_philo *philos)
{
	philos->l_fork->free = false;
	if (philos->table->stop == true)
	{
		philos->l_fork->free = true;
		pthread_mutex_unlock(&philos->l_fork->hold);
		return (true);
	}
	return (false);
}

bool	try_r_fork(t_philo *philos)
{
	philos->r_fork->free = false;
	if (philos->table->stop == true)
	{
		philos->l_fork->free = true;
		pthread_mutex_unlock(&philos->l_fork->hold);
		philos->r_fork->free = true;
		pthread_mutex_unlock(&philos->r_fork->hold);
		return (true);
	}
	return (false);
}

int	init_threads(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->n_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL,
				philo_routines, &table->philos[i]) != 0)
		{
			table->stop = true;
			handle_err(table, "Failed to create philosopher thread", 1);
			return (1);
		}
	}
	while (i--)
	{
		if (pthread_join(table->philos[i].thread, NULL) != 0)
			handle_err(table, "Failed to join philosopher thread", 1);
		return (1);
	}
	return (0);
}
