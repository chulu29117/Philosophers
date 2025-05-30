/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:09:46 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 12:21:19 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	single_philo(t_philo *philos)
{
	pthread_mutex_lock(&philos->l_fork->hold);
	ft_usleep(philos, philos->t_to_die);
}

// bool	try_l_fork(t_philo *philos)
// {
// 	philos->l_fork->free = false;
// 	if (philos->table->stop)
// 	{
// 		philos->l_fork->free = true;
// 		return (false);
// 	}
// 	return (true);
// }

// bool	try_r_fork(t_philo *philos)
// {
// 	philos->r_fork->free = false;
// 	if (philos->table->stop)
// 	{
// 		philos->r_fork->free = true;
// 		return (false);
// 	}
// 	return (true);
// }

void	*monitor(void *arg)
{
	t_table	*table;
	int		i;
	long	time_last_meal;

	table = (t_table *)arg;
	while (1)
	{
		i = -1;
		while (++i < table->n_philos && !table->stop)
		{
			pthread_mutex_lock(&table->philos[i].meal_mutex);
			time_last_meal = timestamp(table) - table->philos[i].last_ate;
			if (time_last_meal >= table->philos[i].t_to_die)
			{
				table->stop = true;
				pthread_mutex_unlock(&table->philos[i].meal_mutex);
				print_state(&table->philos[i], DIED);
				return (NULL);
			}
			pthread_mutex_unlock(&table->philos[i].meal_mutex);
		}
		pthread_mutex_lock(&table->count_mutex);
		if (table->limit && table->n_philo_full >= table->n_philos)
		{
			table->stop = true;
			pthread_mutex_unlock(&table->count_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&table->count_mutex);
		if (table->stop)
			break ;
		usleep(500);
	}
	return (NULL);
}
