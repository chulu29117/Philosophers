/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:41:15 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 00:36:38 by clu              ###   ########.fr       */
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
    
    // Lock first fork
    pthread_mutex_lock(&first_fork->hold);
    if (philos->data->stop_sim)
    {
        pthread_mutex_unlock(&first_fork->hold);
        return ;
    }
    first_fork->free = false;
    print_state(philos, FORK);
    
    if (philos->data->N_philos == 1)
        return (single_philo(philos));
        
    // Lock second fork
    pthread_mutex_lock(&second_fork->hold);
    if (philos->data->stop_sim)
    {
        first_fork->free = true;
        pthread_mutex_unlock(&first_fork->hold);
        pthread_mutex_unlock(&second_fork->hold);
        return ;
    }
    second_fork->free = false;
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
	usleep(1000);
}

void	waiting(t_philo *philos)
{
	if (philos->data->stop_sim)
		return;
	print_state(philos, THINKING);
	ft_usleep(philos, 10);
}

void	*philo_routines(void *arg)
{
    t_philo	*philos;
    
    philos = (t_philo *)arg;
    if (philos->id % 2 != 0)
        waiting(philos);
    else
        thinking(philos);
    while (!philos->data->stop_sim && !philos->full)
    {
        if (check_time(philos, 3))
            break ;
        eating(philos);
        if (!philos->full)
        {
            sleeping(philos);
            thinking(philos);
        }
    }
    return (NULL);
}
