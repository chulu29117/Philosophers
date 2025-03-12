/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:33:36 by clu               #+#    #+#             */
/*   Updated: 2025/03/12 11:03:01 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Monitor thread: continuously checks whether any philosopher has died or
 * if all have eaten the required number of meals (if specified).
 */
void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!data->sim_stopped)
	{
		/* If the optional 'num_times_to_eat' was set and all have eaten, stop. */
		if (check_if_full(data))
            stop_sim(data);
		else
		{
			i = 0;
			while (i < data->num_philos && !data->sim_stopped)
			{
				if (check_if_dead(&data->philos[i]))
                    stop_sim(data);
				i++;
			}
		}
		ft_usleep(1); /* Sleep briefly to avoid hogging CPU. */
	}
	return (NULL);
}

/**
 * Checks if a single philosopher has died.
 */
bool	check_if_dead(t_philo *philo)
{
	long long	current_time;

	pthread_mutex_lock(&philo->meal_mutex);
	current_time = get_time();
	if ((current_time - philo->last_meal) >= philo->data->time_to_die)
	{
		print_status(philo, DIED);
		pthread_mutex_unlock(&philo->meal_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (false);
}

/**
 * Checks if all philosophers have eaten the required number of meals.
 * If 'num_times_to_eat' is 0 or not specified, this check is skipped.
 */
bool	check_if_full(t_data *data)
{
	int	i;

	if (data->num_times_to_eat <= 0)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].times_eaten < data->num_times_to_eat)
		{
			pthread_mutex_unlock(&data->philos[i].meal_mutex);
			return (false);
		}
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	return (true);
}

/**
 * Sets the simulation_stopped flag to true, signaling all threads to finish.
 */
void	stop_sim(t_data *data)
{
	data->sim_stopped = true;
}
