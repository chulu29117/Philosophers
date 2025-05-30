/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:41:15 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 10:36:42 by clu              ###   ########.fr       */
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

	if (philos->data->stop_sim || philos->full)
		return;
	set_forks(philos, &first_to_lock, &second_to_lock);
	// Attempt to lock first fork
	pthread_mutex_lock(&first_to_lock->hold);
	if (philos->data->stop_sim)
	{
		pthread_mutex_unlock(&first_to_lock->hold);
		return;
	}
	print_state(philos, FORK);

	// Special case for 1 philosopher (you already have this)
	if (philos->data->N_philos == 1)
	{
		// For a single philo, after taking one fork, they should die.
		// Your single_philo unlocks it immediately and waits for t_to_die. This should cause death.
		pthread_mutex_unlock(&first_to_lock->hold); // Release the only fork
		ft_usleep(philos, philos->t_to_die);        // Wait until death
		return;
	}

	// Attempt to lock second fork
	pthread_mutex_lock(&second_to_lock->hold);
	if (philos->data->stop_sim)
	{
		pthread_mutex_unlock(&first_to_lock->hold);
		pthread_mutex_unlock(&second_to_lock->hold);
		return;
	}
	print_state(philos, FORK); // Print second fork taken

	// Now they have both forks, start eating
	start_meal(philos); // This updates last_meal and prints EATING
	// ft_usleep(philos, philos->t_to_eat); // This is part of start_meal now.

	// Release forks after eating
	pthread_mutex_unlock(&first_to_lock->hold);
	pthread_mutex_unlock(&second_to_lock->hold);
}

void	sleeping(t_philo *philos)
{
	if (philos->data->stop_sim)
		return ;
	print_state(philos, SLEEPING);
	ft_usleep(philos, philos->t_to_sleep);
}

void	thinking(t_philo *philos)
{
	if (philos->data->stop_sim)
		return ;
	print_state(philos, THINKING);
	usleep(500);
}

void	waiting(t_philo *philos)
{
	if (philos->data->stop_sim)
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
	while (!philos->data->stop_sim)
	{
		if (check_time(philos))
			break ;
		eating(philos);
		if (philos->meal_count != -1 && ++meals == philos->meal_count)
		{
			pthread_mutex_lock(&philos->data->count_mutex);
			philos->full = true;
			philos->data->full_count++;
			if (philos->data->full_count >= philos->data->N_philos)
				philos->data->stop_sim = true;
			pthread_mutex_unlock(&philos->data->count_mutex);
			break ;
		}
		sleeping(philos);
		thinking(philos);
	}
	return (NULL);
}
