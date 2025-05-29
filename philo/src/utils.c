/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:51:32 by clu               #+#    #+#             */
/*   Updated: 2025/05/28 23:48:56 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	timestamp(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000L + time.tv_usec / 1000);
}

void	ft_usleep(long ms, t_data *data)
{
	long	start;

	start = timestamp();
	while (!data->stop && (timestamp() - start < ms))
		usleep(SLEEP_INTERVAL);
}

void	print_status(t_philo *philos, int state)
{
	long	time;

	pthread_mutex_lock(&philos->data->print_mutex);
	if (!philos->data->stop)
	{
		time = timestamp() - philos->data->start;
		if (state == FORK_TAKEN)
			printf("%ld %d has taken a fork\n", time, philos->id);
		else if (state == EATING)
			printf("%ld %d is eating\n", time, philos->id);
		else if (state == SLEEPING)
			printf("%ld %d is sleeping\n", time, philos->id);
		else if (state == THINKING)
			printf("%ld %d is thinking\n", time, philos->id);
		else if (state == DIED)
		{
			printf("%ld %d died\n", time, philos->id);
			philos->data->stop = true;
		}
	}
	pthread_mutex_unlock(&philos->data->print_mutex);
}
