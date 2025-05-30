/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:41:15 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 12:21:19 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	set_forks(t_philo *philos, t_fork **first, t_fork **second)
{
	if (philos->id % 2 == 0)
	{
		*first = philos->l_fork;
		*second = philos->r_fork;
	}
	else
	{
		*first = philos->r_fork;
		*second = philos->l_fork;
	}
}

void eating(t_philo *philos)
{
	t_fork  *first_to_lock;
	t_fork  *second_to_lock;

	if (philos->table->stop || philos->full)
		return;
	set_forks(philos, &first_to_lock, &second_to_lock);
	pthread_mutex_lock(&first_to_lock->hold);
	if (philos->table->stop)
	{
		pthread_mutex_unlock(&first_to_lock->hold);
		return;
	}
	print_state(philos, FORK);
	if (philos->table->n_philos == 1)
	{
		pthread_mutex_unlock(&first_to_lock->hold);
		ft_usleep(philos, philos->t_to_die);
		return;
	}
	pthread_mutex_lock(&second_to_lock->hold);
	if (philos->table->stop)
	{
		pthread_mutex_unlock(&first_to_lock->hold);
		pthread_mutex_unlock(&second_to_lock->hold);
		return;
	}
	print_state(philos, FORK);
	start_meal(philos);
	pthread_mutex_unlock(&first_to_lock->hold);
	pthread_mutex_unlock(&second_to_lock->hold);
}

void	sleeping(t_philo *philos)
{
	if (philos->table->stop)
		return ;
	print_state(philos, SLEEPING);
	ft_usleep(philos, philos->t_to_sleep);
}

void	thinking(t_philo *philos)
{
	if (philos->table->stop)
		return ;
	print_state(philos, THINKING);
	usleep(500);
}

void	waiting(t_philo *philos)
{
	if (philos->table->stop)
		return;
	print_state(philos, THINKING);
	ft_usleep(philos, 40);
}

void	*philo_routines(void *arg)
{
	t_philo	*philos;
	int		meals;

	philos = (t_philo *)arg;
	meals = 0;
	if (philos->id % 2 != 0)
		waiting(philos);
	else
		thinking(philos);
	while (!philos->table->stop)
	{
		if (check_time(philos))
			break ;
		eating(philos);
		if (philos->eat_count != -1 && ++meals == philos->eat_count)
		{
			pthread_mutex_lock(&philos->table->count_mutex);
			philos->full = true;
			philos->table->n_philo_full++;
			if (philos->table->n_philo_full >= philos->table->n_philos)
				philos->table->stop = true;
			pthread_mutex_unlock(&philos->table->count_mutex);
			break ;
		}
		sleeping(philos);
		thinking(philos);
	}
	return (NULL);
}
