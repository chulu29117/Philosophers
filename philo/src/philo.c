/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:43:08 by clu               #+#    #+#             */
/*   Updated: 2025/05/20 23:19:39 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Main routine: parse args, init resources, start sim, then cleanup
*/
int	main(int argc, char **argv)
{
	t_data	data;

	/* Validate argument count */
	if (argc != 5 && argc != 6)
		return (print_usage());
	/* Parse and validate simulation parameters */
	if (!parse_arguments(argc, argv, &data))
		return (EXIT_FAILURE);
	/* Initialize runtime counters and flags */
	data.eating_philos = 0;
	data.sim_stopped = false;
	/* Initialize host mutex for sim_stopped control */
	if (pthread_mutex_init(&data.host_mutex, NULL) != 0)
	{
		printf("Failed to init host_mutex\n");
		return (EXIT_FAILURE);
	}
	/* Initialize forks, philosophers, and start simulation */
	if (!init_forks(&data) || !init_philos(&data) || !start_sim(&data))
	{
		/* Cleanup on initialization failure */
		free(data.forks);
		free(data.philos);
		return (EXIT_FAILURE);
	}
	/* Wait for monitor thread to signal end */
	pthread_join(data.monitor, NULL);
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
	pthread_mutex_destroy(&data->host_mutex);
}

/*
** Comprehensive cleanup: destroy mutexes and free allocated memory
*/
void	end_sim(t_data *data)
{
	destroy_mutexes(data);
	free(data->forks);
	free(data->philos);
}
