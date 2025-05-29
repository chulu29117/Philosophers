/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:09:46 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 00:33:44 by clu              ###   ########.fr       */
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
	if (philos->data->stop_sim)
	{
		philos->l_fork->free = true;
		return (false);
	}
	return (true);
}

bool	try_r_fork(t_philo *philos)
{
	philos->r_fork->free = false;
	if (philos->data->stop_sim)
	{
		philos->r_fork->free = true;
		return (false);
	}
	return (true);
}

void	*monitor(void *arg)
{
    t_data	*data;
    int		i;

    data = (t_data *)arg;
    while (!data->stop_sim)
    {
        i = -1;
        while (++i < data->N_philos)
        {
            pthread_mutex_lock(&data->log_mutex);
            if (timestamp(data) - data->philos[i].last_meal >= data->philos[i].t_to_die)
            {
                data->stop_sim = true;
                print_state(&data->philos[i], DIED);
                pthread_mutex_unlock(&data->log_mutex);
                return (NULL);
            }
            pthread_mutex_unlock(&data->log_mutex);
        }
        // Check if all philosophers are full
        if (data->limit)
        {
            pthread_mutex_lock(&data->log_mutex);
            if (data->full_count >= data->N_philos)
            {
                data->stop_sim = true;
                pthread_mutex_unlock(&data->log_mutex);
                return (NULL);
            }
            pthread_mutex_unlock(&data->log_mutex);
        }
        usleep(1000);
    }
    return (NULL);
}
