/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:43:08 by clu               #+#    #+#             */
/*   Updated: 2025/05/28 02:46:04 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Main routine: parse args, init resources, start sim, then cleanup
*/
int	main(int argc, char **argv)
{
	t_data	data;

	/* Initialize simulation data */
	data.eating_philos = 0;
	data.sim_stopped = false;
	data.forks = NULL;
	data.philo = NULL;
	/* Validate argument count */
	if (argc != 5 && argc != 6)
		return (print_usage());
	/* Parse and validate simulation parameters */
	if (!parse_arguments(argc, argv, &data))
		return (print_usage());
	/* Initialize forks, philosophers, and start simulation */
	if (!init_forks(&data) || !init_philos(&data) || !start_sim(&data))
	{
		/* Cleanup on initialization failure */
		if (data.forks)
			cleanup_forks(&data, data.num_philos);
		if (data.philo)
			free(data.philo);
		return (EXIT_FAILURE);
	}
	/* Wait for monitor thread to signal end */
	join_thread(&data);
	/* Cleanly end simulation (joins threads, frees resources) */
	end_sim(&data);
	return (EXIT_SUCCESS);
}

/*
** Destroys all mutexes allocated for philosophers, forks, printing, and host control
*/
void	destroy_mutexes(t_data *data)
{
	int	i;
	
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->philo[i].meal_mutex);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
}

/*
** Comprehensive cleanup: destroy mutexes and free allocated memory
*/
void	end_sim(t_data *data)
{
	destroy_mutexes(data);
	free(data->forks);
	free(data->philo);
}

/*
** Join all philosopher threads and the monitor thread.
** This ensures all threads complete before exiting the program.
*/
void	join_thread(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philo[i].thread, NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
}