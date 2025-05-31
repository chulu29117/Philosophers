/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:47:50 by clu               #+#    #+#             */
/*   Updated: 2025/05/31 15:08:30 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** A cooler sleep function that allows interruption for checking death.
** It tells the philosopher to sleep for a specified duration in ms.
** Loop through until the specified time, it sleeps for 1 ms and check death.
*/
void	ft_usleep(t_philo *philos, long duration)
{
	long	start;

	start = timestamp(philos->table);
	while (philos->table->stop == false
		&& (timestamp(philos->table) - start) < duration)
		usleep(1000);
}

void	print_state(t_philo *philos, int state)
{
	if (philos->table->stop == true && state != DIED)
		return ;
	pthread_mutex_lock(&philos->table->print_mutex);
	if (state == FORK && philos->table->stop == false)
		printf("%ld %d has taken a fork\n", timestamp(philos->table)
			- philos->table->start_time, philos->id + 1);
	else if (state == EATING && philos->table->stop == false)
		printf("%ld %d is eating\n", timestamp(philos->table)
			- philos->table->start_time, philos->id + 1);
	else if (state == SLEEP && philos->table->stop == false)
		printf("%ld %d is sleeping\n", timestamp(philos->table)
			- philos->table->start_time, philos->id + 1);
	else if (state == THINK && philos->table->stop == false)
		printf("%ld %d is thinking\n", timestamp(philos->table)
			- philos->table->start_time, philos->id + 1);
	else if (state == DIED && philos->table->died == false)
	{
		philos->table->died = true;
		printf("%ld %d died\n", timestamp(philos->table)
			- philos->table->start_time, philos->id + 1);
	}
	pthread_mutex_unlock(&philos->table->print_mutex);
}

void	start_eating(t_philo *philos)
{
	print_state(philos, FORK);
	print_state(philos, EATING);
	pthread_mutex_lock(&philos->table->time_mutex);
	philos->last_ate = timestamp(philos->table);
	pthread_mutex_unlock(&philos->table->time_mutex);
	ft_usleep(philos, philos->t_to_eat);
	philos->l_fork->free = true;
	pthread_mutex_unlock(&philos->l_fork->hold);
	philos->r_fork->free = true;
	pthread_mutex_unlock(&philos->r_fork->hold);
}

int	thread_err(t_table *table, char *msg, int count)
{
	while (count--)
		pthread_join(table->philos[count].philo_thread, NULL);
	cleanup(table, 1);
	printf("%s\n", msg);
	return (-1);
}
