/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:33:48 by clu               #+#    #+#             */
/*   Updated: 2025/03/11 23:59:48 by clu              ###   ########.fr       */
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
bool	init_philosopher(t_philo *philo, int i, t_data *data)
{
	philo->id = i + 1;
	philo->times_eaten = 0;
	philo->data = data;
	philo->left_fork = i;
	philo->right_fork = (i + 1) % data->num_philos;
	philo->last_meal_time = get_time();
	if (pthread_mutex_init(&philo->meal_mutex, NULL) != 0)
		return (false);
	return (true);
}

/**
 * Initializes the philosophers' data structures
 */
bool	init_philosophers(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		if (!init_philosopher(&data->philos[i], i, data))
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
bool	start_simulation(t_data *data)
{
	int	i;

	data->start_time = get_time();
	data->simulation_stopped = false;
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, 
				philosopher_routine, &data->philos[i]) != 0)
			return (false);
		i++;
	}
	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
		return (false);
	return (true);
}



/* ************************************************************************** */
#include "philo.h"

/**
 * Joins philosopher threads
 */
void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
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
void	end_simulation(t_data *data)
{
	join_threads(data);
	destroy_mutexes(data);
	free(data->forks);
	free(data->philos);
}

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
	if (!init_forks(&data))
		return (1);
	if (!init_philosophers(&data))
	{
		free(data.forks);
		return (1);
	}
	if (!start_simulation(&data))
	{
		free(data.forks);
		free(data.philos);
		return (1);
	}
	end_simulation(&data);
	return (0);
}
