/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:41:15 by clu               #+#    #+#             */
/*   Updated: 2025/05/31 18:31:22 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** Check if a philos has eaten enough meals to be considered full.
*/
static bool	check_full(t_philo *philos, int meals_eaten)
{
	if (philos->eat_count != -2 && meals_eaten == philos->eat_count)
	{
		pthread_mutex_lock(&philos->table->full_mutex);
		if (!philos->full)
		{
			philos->full = true;
			philos->table->n_philos_full++;
			if (philos->table->n_philos_full == philos->table->n_philos)
				philos->table->stop = true;
		}
		pthread_mutex_unlock(&philos->table->full_mutex);
		return (true);
	}
	return (false);
}

/*
** Cycle through the eating, sleeping, and thinking routines for a philo.
*/
static bool	philo_cycle(t_philo *philos, int *meals_eaten)
{
	eating(philos);
	if (philos->table->stop)
		return (false);
	(*meals_eaten)++;
	if (check_full(philos, *meals_eaten))
		return (false);
	if (philos->table->n_philos > 1)
	{
		sleeping(philos);
		if (philos->table->stop)
			return (false);
		thinking(philos);
		if (philos->table->stop)
			return (false);
	}
	return (true);
}

/*
** Main routine for each philosopher thread.
** Each philos go through the eating, sleeping, and thinking routines.
** Philos will stop if they reach the number of meals they need to eat.
** If the number of philos is not 1, sleep and think after eating.
*/
void	*philo_routines(void *arg)
{
	t_philo	*philos;
	int		meals_eaten;

	philos = (t_philo *)arg;
	meals_eaten = 0;
	if (philos->id % 2 != 0)
		wait_start(philos);
	else
		thinking(philos);
	while (philos->table->stop == false)
	{
		if (!philo_cycle(philos, &meals_eaten))
			break ;
	}
	return (NULL);
}
