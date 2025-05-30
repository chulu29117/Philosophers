/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:56:13 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 13:08:27 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	init_philos(t_philo *philos, int index, char **argv, t_table *table)
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
	if (argc == 5)
		philos->eat_count = ft_atoi(argv[5]);
	else
		philos->eat_count = -2;
	if (philos->t_to_die < 0 || philos->t_to_eat < 0
		|| philos->t_to_sleep < 0 || philos->eat_count == -1)
		return (handle_err(table, "Error: bad arguments", 1));
	philos->table = table;
	philos->id = index;
	philos->thread = 0;
	philos->full = false;
	philos->l_fork = &table->forks[index];
	philos->r_fork = &table->forks[(index + 1) % table->n_philos];
	philos->last_ate = table->start_time;
	return (0);
}

int	init_table(t_table *table)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (handle_err(table, "Failed to init print_mutex", 0));
	table->forks = malloc(table->n_philos * sizeof(t_fork));
	if (!table->forks)
		return (handle_err(table, "Failed to malloc forks", 1));
	while (++i < table->n_philos)
	{
		if (pthread_mutex_init(&table->forks[i].hold, NULL) != 0)
		{
			while (i--)
				pthread_mutex_destroy(&table->forks[i].hold);
			return (handle_err(table, "Failed to init forks", 1));
		}
		table->forks[i].free = true;
	}
	table->died = false;
	table->start_time = timestamp(table);
	if (table->start_time < 0)
		return (handle_err(table, "Failed to gettimeofday", 1));
	return (0);
}

int	set_table(t_table *table, int argc, char **argv)
{
	int	i;

	i = 0;
	while (argv[++i])
	{
		if (!validate(argv[i]))
			return (-1);
	}
	table->n_philos = ft_atoi(argv[1]);
	if (table->n_philos < 0 || table->n_philos > 1000)
		return (handle_err(table, "Error: bad arguments", 0));
	table->stop = false;
	table->n_philos_full = 0;
	if (argc == 6)
		table->limit = true;
	else
		table->limit = false;
	if (set_philos(table, argv) < 0)
		return (-1);
	return (0);
}

int	set_philos(t_table *table, char **argv)
{
	int	i;

	i = -1;
	table->philos = NULL;
	table->forks = NULL;
	if (init_table(table) < 0)
		return (-1);
	table->philos = malloc(table->n_philos * sizeof(t_philo));
	if (!table->philos)
		return (handle_err(table, "Failed to malloc philos", 1));
	while (++i < table->n_philos)
	{
		if (init_philos(&table->philos[i], i, argv, table) < 0)
			return (-1);
	}
	return (0);
}
