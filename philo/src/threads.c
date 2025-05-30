/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:47:50 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 12:15:42 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_usleep(t_philo *philos, long duration)
{
	long	start;

	start = timestamp(philos->table);
	while (!philos->table->stop
		&& (timestamp(philos->table) - start) < duration)
		usleep(500);
}

long check_time(t_philo *philos)
{
    long result;
    pthread_mutex_lock(&philos->meal_mutex); // Lock around last_ate access
    result = timestamp(philos->table) - philos->table->start_time;
    pthread_mutex_unlock(&philos->meal_mutex);
    return (result);
}

// New function for checking death
bool is_philo_dead(t_philo *philos)
{
    pthread_mutex_lock(&philos->meal_mutex);
    if (timestamp(philos->table) - philos->last_ate >= philos->t_to_die)
    {
        pthread_mutex_unlock(&philos->meal_mutex);
        return (true);
    }
    pthread_mutex_unlock(&philos->meal_mutex);
    return (false);
}

void print_state(t_philo *philos, int state)
{
    pthread_mutex_lock(&philos->table->print_mutex);
    if (philos->table->stop && state != DIED)
    {
        pthread_mutex_unlock(&philos->table->print_mutex);
        return ;
    }
    long current_time = timestamp(philos->table) - philos->table->start_time; // Get time once

    if (state == FORK && !philos->table->stop)
        printf("%ld %d has taken a fork\n", current_time, philos->id + 1);
    else if (state == EATING && !philos->table->stop)
        printf("%ld %d is eating\n", current_time, philos->id + 1);
    else if (state == SLEEPING && !philos->table->stop)
        printf("%ld %d is sleeping\n", current_time, philos->id + 1);
    else if (state == THINKING && !philos->table->stop)
        printf("%ld %d is thinking\n", current_time, philos->id + 1);
    else if (state == DIED && !philos->table->died)
    {
        philos->table->died = true;
		philos->table->stop = true;
        printf("%ld %d died\n", current_time, philos->id + 1);
    }
    pthread_mutex_unlock(&philos->table->print_mutex);
}

void	start_meal(t_philo *philos)
{
	print_state(philos, EATING);
	pthread_mutex_lock(&philos->meal_mutex);
	philos->last_ate = timestamp(philos->table);
	pthread_mutex_unlock(&philos->meal_mutex);
	ft_usleep(philos, philos->t_to_eat);
	pthread_mutex_unlock(&philos->r_fork->hold);
	pthread_mutex_unlock(&philos->l_fork->hold);
}

int	thread_err(t_table *table, char *msg, int count)
{
	while (count--)
		pthread_join(table->philos[count].thread, NULL);
	cleanup(table, 1);
	printf("%s\n", msg);
	return (-1);
}
