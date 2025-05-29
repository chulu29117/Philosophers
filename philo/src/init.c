/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:56:13 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 00:54:58 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	init_threads(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	i = -1;
	if (pthread_create(&monitor_thread, NULL, monitor, data) != 0)
		return (handle_err(data, "Failed to create monitor thread", 1));
	while (++i < data->N_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_routines,
				&data->philos[i]) != 0)
		{
			data->stop_sim = true;
			pthread_join(monitor_thread, NULL);
			return (thread_err(data, "Failed to create threads", i));
		}
	}
	if (pthread_join(monitor_thread, NULL) != 0)
		return (handle_err(data, "Failed to join monitor thread", 1));
	while (i--)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (handle_err(data, "Failed to join threads", 1));
	}
	return (0);
}

int	init_philos(t_philo *philos, int index, char **argv, t_data *data)
{
	int	argc;
	int	i;

	i = 1;
	argc = 0;
	while (argv[i++])
		argc++;
	philos->t_to_die = ft_atoi(argv[2]);
	philos->t_to_eat = ft_atoi(argv[3]);
	philos->t_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		philos->meal_count = ft_atoi(argv[5]);
	else
		philos->meal_count = -1;
	if (philos->t_to_die < 0 || philos->t_to_eat < 0
		|| philos->t_to_sleep < 0 || (argc == 6 && philos->meal_count <= 0))
		return (handle_err(data, "Error: bad arguments", 1));
	philos->full = false;
	philos->meals_eaten = 0;
	philos->thread = 0;
	philos->data = data;
	philos->id = index;
	philos->l_fork = &data->forks[index];
	philos->r_fork = &data->forks[(index + 1) % data->N_philos];
	philos->last_meal = data->sim_start;
	return (0);
}

int	init_data(t_data *data)
{
	int	i;
	
	i = -1;
	if (pthread_mutex_init(&data->log_mutex, NULL) != 0)
		return (handle_err(data, "Failed to init log_mutex", 0));
	data->forks = malloc(data->N_philos * sizeof(t_fork));
	if (!data->forks)
		return (handle_err(data, "Failed to malloc forks", 1));
	while (++i < data->N_philos)
	{
		if (pthread_mutex_init(&data->forks[i].hold, NULL) != 0)
		{
			while (i--)
			pthread_mutex_destroy(&data->forks[i].hold);
			return (handle_err(data, "Failed to init forks", 1));
		}
		data->forks[i].free = true;
	}
	data->died = false;
	data->sim_start = timestamp(data);
	if (data->sim_start < 0)
		return (handle_err(data, "Failed to gettimeofday", 1));
	return (0);
}

int	set_philos(t_data *data, char **argv)
{
	int	i;
	
	i = -1;
	data->philos = NULL;
	data->forks = NULL;
	if (init_data(data) < 0)
		return (-1);
	data->philos = malloc(data->N_philos * sizeof(t_philo));
	if (!data->philos)
		return (handle_err(data, "Failed to malloc philos", 1));
	while (++i < data->N_philos)
	{
		if (init_philos(&data->philos[i], i, argv, data) < 0)
			return (-1);
	}
	return (0);
}

int	set_data(t_data *data, int argc, char **argv)
{
	int	i;

	i = 0;
	while (argv[++i])
	{
		if (!validate(argv[i]))
			return (-1);
	}
	data->N_philos = ft_atoi(argv[1]);
	if (data->N_philos < 0 || data->N_philos > 1000)
		return (handle_err(data, "Error: bad arguments", 0));
	data->stop_sim = false;
	data->full_count = 0;
	if (argc == 6)
		data->limit = true;
	else
		data->limit = false;
	if (set_philos(data, argv) < 0)
		return (-1);
	return (0);
}
