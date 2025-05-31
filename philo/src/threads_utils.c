/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:09:46 by clu               #+#    #+#             */
/*   Updated: 2025/05/31 15:08:39 by clu              ###   ########.fr       */
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
	int			i;
	pthread_t	monitor_thread;

	i = -1;
	while (++i < table->n_philos)
	{
		if (pthread_create(&table->philos[i].philo_thread, NULL,
				philo_routines, &table->philos[i]) != 0)
		{
			table->stop = true;
			return (handle_err(table, "Failed to create philosopher thread", 1));
		}
	}
	if (pthread_create(&monitor_thread, NULL, monitor, table) != 0)
	{
		table->stop = true;
		return (thread_err(table, "Failed to create monitor thread", table->n_philos));
	}
	i = table->n_philos;
	while (i-- > 0)
	{
		if (pthread_join(table->philos[i].philo_thread, NULL) != 0)
			return (handle_err(table, "Failed to join philosopher thread", 1));
	}
	if (pthread_join(monitor_thread, NULL) != 0)
		return (handle_err(table, "Failed to join monitor thread", 1));
	return (0);
}

/*
** This function will run in its own thread.
** It periodically wakes up, iterates through all philosophers,
** and if it finds one whose (current_time – last_ate) ≥ t_to_die,
** it flags `table->stop = true` and prints the death message.
*/
void	*monitor(void *arg)
{
	t_table	*table;
	int		i;
	long	now;
	long	last_ate_time;

	table = (t_table *)arg;
	while (!table->stop)
	{
		usleep(1000);
		now = timestamp(table);
		i = 0;
		while (i < table->n_philos && !table->stop)
		{
			pthread_mutex_lock(&table->time_mutex);
			last_ate_time = table->philos[i].last_ate;
			pthread_mutex_unlock(&table->time_mutex);
			if (now - last_ate_time >= table->philos[i].t_to_die)
			{
				table->stop = true;
				print_state(&table->philos[i], DIED);
				break ;
			}
			i++;
		}
	}
	return (NULL);
}
 