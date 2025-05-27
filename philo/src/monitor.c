/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:30:58 by clu               #+#    #+#             */
/*   Updated: 2025/05/28 02:50:33 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Check if all philosophers have eaten the required number of times.
** Returns true if full, false otherwise.
*/
bool	check_if_full(t_data *data)
{
	int	i;

	if (!data->sim_duration)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philo[i].meal_mutex);
		if (data->philo[i].times_eaten < data->num_times_to_eat)
		{
			pthread_mutex_unlock(&data->philo[i].meal_mutex);
			return (false);
		}
		pthread_mutex_unlock(&data->philo[i].meal_mutex);
		i++;
	}
	return (true);
}

/*
** Loop through philosophers to detect death. Returns true to stop sim.
*/
bool death_loop(t_data *data)
{
	int			i;
	long long	diff;
	bool		should_die;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philo[i].meal_mutex);
		diff = get_time() - data->philo[i].last_meal;
		should_die = diff >= data->time_to_die;
		pthread_mutex_unlock(&data->philo[i].meal_mutex);
		if (should_die)
		{
			print_status(&data->philo[i], DIED);
			return (true);
		}
		i++;
	}
	return (false);
}

/*
** Monitor thread: loops until a death or all-fed condition triggers stop.
*/
void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!check_stop(data))
	{
		if (death_loop(data) || check_if_full(data))
		{
			stop_sim(data);
			return (NULL);
		}
		ft_usleep(1);
	}
	return (NULL);
}
