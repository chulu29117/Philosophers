/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:30:58 by clu               #+#    #+#             */
/*   Updated: 2025/05/29 17:18:20 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** Scan all philosophers for a death timeout. Returns true if one died.
*/
static bool	is_dead(t_data *data)
{
	int		i;
	long	diff;
	long	now;

	i = 0;
	now = timestamp();
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		diff = now - data->philos[i].last_meal;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		if (diff >= data->t_to_die)
		{
			pthread_mutex_lock(&data->stop_mutex);
			data->stop = true;
			pthread_mutex_unlock(&data->stop_mutex);
			print_status(&data->philos[i], DIED);
			return (true);
		}
		i++;
	}
	return (false);
}

/*
** Check if all philosophers have reached the meal limit.
** Returns true only when max_meals > 0 and everyone is full.
*/
static bool	is_all_full(t_data *data)
{
	int		i;
	bool	full;

	if (data->max_meals <= 0)
		return (false);
	full = true;
	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals < data->max_meals)
		{
			full = false;
			break ;
		}
		i++;
	}
	if (full)
	{
		pthread_mutex_lock(&data->stop_mutex);
		data->stop = true;
		pthread_mutex_unlock(&data->stop_mutex);
	}
	return (full);
}

/*
** Monitor thread: loop until someone dies or everyone is full.
*/
void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!data->stop)
	{
		if (is_dead(data))
			return (NULL);
		if (is_all_full(data))
		{
			pthread_mutex_lock(&data->stop_mutex);
			data->stop = true;
			pthread_mutex_unlock(&data->stop_mutex);
			return (NULL);
		}
		usleep(SLEEP_INTERVAL);
	}
	return (NULL);
}
