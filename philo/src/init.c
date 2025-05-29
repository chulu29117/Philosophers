/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:42:40 by clu               #+#    #+#             */
/*   Updated: 2025/05/29 17:21:04 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	init_data(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	data->forks = malloc(sizeof(t_fork) * data->num_philos);
	if (!data->philos || !data->forks)
	{
		free(data->philos);
		free(data->forks);
		return (false);
	}
	memset(data->philos, 0, sizeof(*data->philos) * data->num_philos);
	memset(data->forks, 0, sizeof(*data->forks) * data->num_philos);
	data->start = timestamp();
	pthread_mutex_lock(&data->stop_mutex);
	data->stop = false;
	pthread_mutex_unlock(&data->stop_mutex);
	return (true);
}

bool	init_mutexes(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&data->waiter, NULL) != 0 )
		return (false);
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
		return (false);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i].lock, NULL) != 0)
			return (false);
		i++;
	}
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
		{
			data->stop = true;
			while (--i >= 0)
				pthread_join(data->philos[i].thread, NULL);
			pthread_join(data->monitor_thread, NULL);
			return (false);
		}
		usleep(500);
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
	if (data->forks)
	{
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i].lock);
			i++;
		}
		free(data->forks);
	}
	i = 0;
	if (data->philos)
	{
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->philos[i].meal_mutex);
			i++;
		}
		free(data->philos);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->waiter);
	pthread_mutex_destroy(&data->stop_mutex);
}
