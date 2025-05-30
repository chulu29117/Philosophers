/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:09:46 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 10:05:41 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	single_philo(t_philo *philos)
{
	pthread_mutex_unlock(&philos->l_fork->hold);
	ft_usleep(philos, philos->t_to_die);
}

// bool	try_l_fork(t_philo *philos)
// {
// 	philos->l_fork->free = false;
// 	if (philos->data->stop_sim)
// 	{
// 		philos->l_fork->free = true;
// 		return (false);
// 	}
// 	return (true);
// }

// bool	try_r_fork(t_philo *philos)
// {
// 	philos->r_fork->free = false;
// 	if (philos->data->stop_sim)
// 	{
// 		philos->r_fork->free = true;
// 		return (false);
// 	}
// 	return (true);
// }

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	long	time_last_meal;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->N_philos && !data->stop_sim)
		{
			pthread_mutex_lock(&data->philos[i].meal_mutex);
			time_last_meal = timestamp(data) - data->philos[i].last_meal;
			if (time_last_meal >= data->philos[i].t_to_die)
			{
				pthread_mutex_lock(&data->log_mutex);
				data->stop_sim = true;
				pthread_mutex_unlock(&data->log_mutex);
				pthread_mutex_unlock(&data->philos[i].meal_mutex);
				print_state(&data->philos[i], DIED);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philos[i].meal_mutex);
		}
		pthread_mutex_lock(&data->count_mutex);
		if (data->limit)
		{
    		printf("DEBUG: full_count=%d, N_philos=%d\n", data->full_count, data->N_philos);
		}
		if (data->limit && data->full_count >= data->N_philos)
		{
			data->stop_sim = true;
			data->full_count++;
			pthread_mutex_unlock(&data->count_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&data->count_mutex);
		pthread_mutex_lock(&data->log_mutex);
		if (data->stop_sim)
		{
			pthread_mutex_unlock(&data->log_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->log_mutex);
		usleep(500);
	}
	return (NULL);
}
