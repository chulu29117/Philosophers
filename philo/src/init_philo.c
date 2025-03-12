/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 09:24:34 by clu               #+#    #+#             */
/*   Updated: 2025/03/12 11:36:08 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Cleanup function for init_forks in case of failure
 */
void	cleanup_forks(t_data *data, int i)
{
	while (--i >= 0)
		pthread_mutex_destroy(&data->forks[i]);
	free(data->forks);
}

/**
 * Initializes the forks (mutexes) for the simulation
 */
bool	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			cleanup_forks(data, i);
			return (false);
		}
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		cleanup_forks(data, data->num_philos);
		return (false);
	}
	return (true);
}

/**
 * Initializes a single philosopher's data
 */
bool	init_philo(t_philo *philo, int i, t_data *data)
{
	philo->id = i + 1;
	philo->times_eaten = 0;
	philo->data = data;
	philo->left_fork = i;
	philo->right_fork = (i + 1) % data->num_philos;
	philo->last_meal = get_time();
	if (pthread_mutex_init(&philo->meal_mutex, NULL) != 0)
		return (false);
	return (true);
}

/**
 * Initializes the philosophers' data structures
 */
bool	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		if (!init_philo(&data->philos[i], i, data))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->philos[i].meal_mutex);
			free(data->philos);
			return (false);
		}
		i++;
	}
	return (true);
}

/**
 * Creates threads for philosophers and monitor
 */
bool	start_sim(t_data *data)
{
	int	i;

	data->start_time = get_time();
	data->sim_stopped = false;
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, 
				philo_routine, &data->philos[i]) != 0)
			return (false);
        // Detach the thread so that we don't wait for it in join_threads.
        pthread_detach(data->philos[i].thread);
		i++;
	}
	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
		return (false);
	return (true);
}

