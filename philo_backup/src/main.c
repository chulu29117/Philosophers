/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 09:24:55 by clu               #+#    #+#             */
/*   Updated: 2025/03/13 11:29:37 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Main function: initializes the program, starts the simulation,
 * and cleans up resources.
 */
int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (print_usage());
	if (!parse_arguments(argc, argv, &data))
		return (1);
	data.eating_philos = 0;
	// Initialize simulation duration.
	// If a required number of meals is specified, we estimate duration based on eating and sleeping times.
	// Otherwise, we set a default duration (e.g., 5 times the time_to_die).
	if (data.num_times_to_eat > 0)
		data.sim_duration = data.num_times_to_eat * (data.time_to_eat + data.time_to_sleep);
	else
		data.sim_duration = data.time_to_die * 5;	// Initialize the host mutex used to protect the eating counter.
	if (pthread_mutex_init(&data.host_mutex, NULL) != 0)
	{
		printf("Failed to initialize host_mutex");
		return (1);
	}
	if (!init_forks(&data))
		return (1);
	if (!init_philos(&data))
	{
		free(data.forks);
		return (1);
	}
	if (!start_sim(&data))
	{
		free(data.forks);
		free(data.philos);
		return (1);
	}
	// Join only the monitor thread since philosopher threads are detached.
	join_threads(&data);
	// Cleanup all resources.
	destroy_mutexes(&data);
	free(data.forks);
	free(data.philos);
	pthread_mutex_destroy(&data.host_mutex);
	return (0);
}

/**
 * Joins the monitor thread.
 * (Philosopher threads are detached to prevent blocking on deadlocks.)
 */
void	join_threads(t_data *data)
{
	pthread_join(data->monitor, NULL);
}

/**
 * Destroys all mutex locks.
 */
void	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
}

/**
 * Cleans up resources after simulation ends.
 */
void	end_sim(t_data *data)
{
	join_threads(data);
	destroy_mutexes(data);
	free(data->forks);
	free(data->philos);
	pthread_mutex_destroy(&data->host_mutex);
}
