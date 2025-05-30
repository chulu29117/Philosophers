/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:41:15 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 10:03:29 by clu              ###   ########.fr       */
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

void	eating(t_philo *philos)
{
	t_fork	*first_fork;
	t_fork	*second_fork;

	if (philos->data->stop_sim || philos->full)
		return ;
	set_forks(philos, &first_fork, &second_fork);
	pthread_mutex_lock(&first_fork->hold);
	if (philos->data->stop_sim)
	{
		pthread_mutex_unlock(&first_fork->hold);
		return ;
	}
	print_state(philos, FORK);
	if (philos->data->N_philos == 1)
		return (single_philo(philos));
	if (check_time(philos, SLEEPING))
	{
		pthread_mutex_unlock(&first_fork->hold);
		return ;
	}
	pthread_mutex_lock(&second_fork->hold);
	if (philos->data->stop_sim)
	{
		pthread_mutex_unlock(&first_fork->hold);
		pthread_mutex_unlock(&second_fork->hold);
		return ;
	}
	print_state(philos, FORK);
	start_meal(philos);
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
		if (check_time(philos, SLEEPING))
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
