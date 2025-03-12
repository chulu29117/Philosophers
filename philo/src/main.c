/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 09:24:55 by clu               #+#    #+#             */
/*   Updated: 2025/03/12 11:37:39 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Main function: initializes the program, starts simulation
 */
int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (print_usage());
	if (!parse_arguments(argc, argv, &data))
		return (1);
	data.eating_philos = 0;
	if (pthread_mutex_init(&data.host_mutex, NULL) != 0)
	{
		perror("Failed to initialize host_mutex");
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
	end_sim(&data);
	return (0);
}

/**
 * Joins philosopher threads
 */
void	join_threads(t_data *data)
{
	pthread_join(data->monitor, NULL);
}

/**
 * Destroys mutex locks
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
 * Waits for threads to finish and cleans up resources
 */
void	end_sim(t_data *data)
{
	join_threads(data);
	destroy_mutexes(data);
	free(data->forks);
	free(data->philos);
	pthread_mutex_destroy(&data->host_mutex);
}
