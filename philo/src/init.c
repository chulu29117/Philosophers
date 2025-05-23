/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:42:40 by clu               #+#    #+#             */
/*   Updated: 2025/05/20 23:23:03 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Initialize mutexes for forks and the print mutex.
** Returns true on success; on failure, cleans up and returns false.
*/
bool	init_forks(t_data *data)
{
	int	i;

	/* Allocate an array of mutexes, one per philosopher/fork */
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (false);
	/* Initialize each fork mutex in the array */
	i = 0;
	while (i < data->num_philos)
	{
		/* Attempt to initialize the i-th fork mutex */
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			/* If init fails, destroy all already-initialized forks */
			printf("Failed to init fork mutex\n");
			cleanup_forks(data, i);
			return (false);
		}
		i++;
	}
	/* Initialize the mutex used for synchronized printing */
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		/* On failure, clean up all fork mutexes and allocated memory */
		printf("Failed to init print mutex\n");
		cleanup_forks(data, data->num_philos);
		return (false);
	}
	return (true);
}

/*
** Destroy up to 'initialized' fork mutexes and free the forks array.
*/
void	cleanup_forks(t_data *data, int initialized)
{
	int	i;
	
	i = 0;
	while (i < initialized)
	{
		// destroy each initialized fork mutex
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
}

/*
** Initialize a single philosopher structure:
**  - assign IDs, forks, and initial state
**  - initialize the per-philosopher meal mutex
** Returns true on success, false on failure.
*/
bool	init_philo(t_philo *philo, int i, t_data *data)
{
	philo->id = i + 1;
	philo->times_eaten = 0;								// no meals eaten yet
	philo->left_fork = i;								// left fork index
	philo->right_fork = (i + 1) % data->num_philos;		// right fork (wrap around)
	philo->last_meal = data->start_time;				// start with the simulation start time
	philo->data = data;
	/* Initialize meal mutex to protect last_meal & times_eaten */
	if (pthread_mutex_init(&philo->meal_mutex, NULL) != 0)
		return (false);
	return (true);	
}

/*
** Initialize all philosophers:
**  - allocate the array
**  - call init_philo for each entry
*/
bool	init_philos(t_data *data)
{
	int	i;

	/* Allocate array for philosopher structs */
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		if (!init_philo(&data->philos[i], i, data))
		{
			// Cleanup previously initialized philosopher mutexes
			while (--i >= 0)
				pthread_mutex_destroy(&data->philos[i].meal_mutex);
			free(data->philos);
			return (false);
		}
		i++;
	}
	return (true);
}

/*
** Start the simulation:
**  - record the actual start time
**  - create threads for each philosopher
**  - create a monitor thread
** Returns true on success, false on failure.
*/
bool	start_sim(t_data *data)
{
	int	i;
	
	/* Record the moment simulation begins for timestamping */
	data->start_time = get_time();
	/* Spawn philosopher threads */
	i = 0;
	while (i < data->num_philos)
	{
		/* Each thread runs philo_routine, taking & returning forks */
		if (pthread_create(&data->philos[i].thread, NULL
							, philo_routine, &data->philos[i]) != 0)
			return (false);
		i++;
	}
	/* Spawn a separate monitor thread to watch for death or completion */
	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
		return (false);
	return (true);
}
