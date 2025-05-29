/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:42:40 by clu               #+#    #+#             */
/*   Updated: 2025/05/29 00:45:11 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	init_data(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	data->forks = malloc(sizeof(t_fork) * data->num_philos);
	if (!data->philos || !data->forks)
		return (false);
	memset(data->philos, 0, sizeof(*data->philos) * data->num_philos);
	memset(data->forks, 0, sizeof(*data->forks) * data->num_philos);
	data->start = timestamp();
	data->stop = false;
	return (true);
}

bool	init_mutexes(t_data *data)
{
	int	i;

	/* 1) Initialize the print lock */
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (false);
	/* 2) Initialize each fork mutex */
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i].lock, NULL) != 0)
			return (false);
		data->forks[i].in_use = false;
		i++;
	}
	/* 3) Initialize each philosopher’s meal mutex */
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
			return (false);
		i++;
	}
	return (true);
}

bool	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals = 0;
		data->philos[i].last_meal = data->start;
		data->philos[i].data = data;
		i++;
	}
	return (true);
}

bool	start_sim(t_data *data)
{
	int	i;

	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_routine,
				&data->philos[i]) != 0)
			return (false);
		usleep(100);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
		pthread_join(data->philos[i++].thread, NULL);
	pthread_join(data->monitor_thread, NULL);
	return (true);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i].lock);
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	free(data->philos);
	free(data->forks);
}
