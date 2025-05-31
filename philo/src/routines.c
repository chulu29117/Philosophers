/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:41:15 by clu               #+#    #+#             */
/*   Updated: 2025/05/31 14:42:24 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** Eating routine for philosophers.
** Each philosopher tries to take the left fork first, then the right fork.
** If no forks available, check for death and sleep.
** If there's only 1 philo, it will wait for the time to die.
** If successful, they start eating.
*/
void	eating(t_philo *philos)
{
	if (philos->table->stop == true)
		return ;
	while (!philos->l_fork->free && !philos->table->stop)
		usleep(1000);
	if (philos->table->stop)
		return ;
	pthread_mutex_lock(&philos->l_fork->hold);
	if (try_l_fork(philos))
		return ;
	print_state(philos, FORK);
	if (philos->table->n_philos == 1)
		return (single_philo(philos));
	while (!philos->r_fork->free && !philos->table->stop)
		usleep(1000);
	if (philos->table->stop)
	{
		pthread_mutex_unlock(&philos->l_fork->hold);
		return ;
	}
	pthread_mutex_lock(&philos->r_fork->hold);
	if (try_r_fork(philos))
		return ;
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
		eating(philos);
		if (philos->table->stop)
			break ;
		meals_eaten++;
		if (philos->eat_count != -2 && meals_eaten == philos->eat_count)
		{
			pthread_mutex_lock(&philos->table->full_mutex);
			{
				if (!philos->full)
				{
					philos->full = true;
					philos->table->n_philos_full++;
					if (philos->table->n_philos_full == philos->table->n_philos)
						philos->table->stop = true;
				}
			}
			pthread_mutex_unlock(&philos->table->full_mutex);
			return (NULL);
		}
		if (philos->table->n_philos > 1)
		{
			sleeping(philos);
			if (philos->table->stop)
				break ;
			thinking(philos);
			if (philos->table->stop)
				break ;
		}
	}
	return (NULL);
}
