/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:02:24 by clu               #+#    #+#             */
/*   Updated: 2025/05/31 19:16:04 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** Philo eating routine if both forks are available.
** Takes the left fork, then the right fork.
** If successful, it starts eating and updates the last_ate time.
*/
static void	start_eating(t_philo *philos)
{
	t_table	*table;

	table = philos->table;
	print_state(philos, EATING);
	pthread_mutex_lock(&table->time_mutex);
	philos->last_ate = timestamp(table);
	pthread_mutex_unlock(&table->time_mutex);
	ft_usleep(philos, philos->t_to_eat);
	pthread_mutex_unlock(&philos->r_fork->hold);
	pthread_mutex_unlock(&philos->l_fork->hold);
}

/*
** Eating routine for philosophers.
** Each philosopher tries to take the left fork first, then the right fork.
** If no forks available, check for death and sleep.
** If there's only 1 philo, it will wait for the time to die.
** If successful, they start eating.
*/
void	eating(t_philo *philos)
{
	t_table	*table;

	table = philos->table;
	if (table->stop)
		return ;
	pthread_mutex_lock(&philos->l_fork->hold);
	print_state(philos, FORK);
	if (table->n_philos == 1)
	{
		single_philo(philos);
		return ;
	}
	pthread_mutex_lock(&philos->r_fork->hold);
	if (table->stop)
	{
		pthread_mutex_unlock(&philos->r_fork->hold);
		pthread_mutex_unlock(&philos->l_fork->hold);
		return ;
	}
	print_state(philos, FORK);
	start_eating(philos);
}

/*
** Sleeping routine for philosophers.
** Show the sleeping state and sleeps for t_to_sleep ms.
*/
void	sleeping(t_philo *philos)
{
	if (philos->table->stop == true)
		return ;
	print_state(philos, SLEEP);
	ft_usleep(philos, philos->t_to_sleep);
}

/*
** Thinking routine for philosophers.
** Show the thinking state and sleeps for a short time.
*/
void	thinking(t_philo *philos)
{
	if (philos->table->stop == true)
		return ;
	print_state(philos, THINK);
	usleep(1000);
}

/*
** Philos wait to start routine.
** Used to stagger the start of philosophers to avoid deadlock.
*/
void	wait_start(t_philo *philos)
{
	if (philos->table->stop == true)
		return ;
	print_state(philos, THINK);
	ft_usleep(philos, 10);
}
