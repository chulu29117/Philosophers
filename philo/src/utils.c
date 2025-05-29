/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:29:14 by clu               #+#    #+#             */
/*   Updated: 2025/05/29 23:09:00 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	timestamp(t_data *data)
{
	struct timeval	time;
	
	if (gettimeofday(&time, NULL) < 0)
	{
		data->stop_sim = true;
		return (-1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	cleanup(t_data *data, int clean_mutex)
{
	int	i;
	
	i = -1;
	if (data)
	{
		if (data->philos)
			free(data->philos);
		if (data->forks)
		{
			while (++i < data->N_philos)
				pthread_mutex_destroy(&data->forks[i].hold);
			free(data->forks);
		}
		if (clean_mutex)
			pthread_mutex_destroy(&data->log_mutex);
	}
}

int	handle_err(t_data *data, char *msg, int clean)
{
	cleanup(data, clean);
	printf("%s\n", msg);
	return (-1);
}
