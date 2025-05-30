/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:47:50 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 10:23:00 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_usleep(t_philo *philos, long duration)
{
	long	start;

	start = timestamp(philos->data);
	while (!philos->data->stop_sim
		&& (timestamp(philos->data) - start) < duration)
		usleep(500);
}

long check_time(t_philo *philos)
{
    long result;
    pthread_mutex_lock(&philos->meal_mutex); // Lock around last_meal access
    result = timestamp(philos->data) - philos->data->sim_start;
    pthread_mutex_unlock(&philos->meal_mutex);
    return (result);
}

// New function for checking death
bool is_philo_dead(t_philo *philos)
{
    pthread_mutex_lock(&philos->meal_mutex);
    if (timestamp(philos->data) - philos->last_meal >= philos->t_to_die)
    {
        pthread_mutex_unlock(&philos->meal_mutex);
        return (true);
    }
    pthread_mutex_unlock(&philos->meal_mutex);
    return (false);
}

void print_state(t_philo *philos, int state)
{
    pthread_mutex_lock(&philos->data->log_mutex);
    if (philos->data->stop_sim && state != DIED)
    {
        pthread_mutex_unlock(&philos->data->log_mutex);
        return ;
    }
    long current_time = timestamp(philos->data) - philos->data->sim_start; // Get time once

    if (state == FORK && !philos->data->stop_sim)
        printf("%ld %d has taken a fork\n", current_time, philos->id + 1);
    else if (state == EATING && !philos->data->stop_sim)
        printf("%ld %d is eating\n", current_time, philos->id + 1);
    else if (state == SLEEPING && !philos->data->stop_sim)
        printf("%ld %d is sleeping\n", current_time, philos->id + 1);
    else if (state == THINKING && !philos->data->stop_sim)
        printf("%ld %d is thinking\n", current_time, philos->id + 1);
    else if (state == DIED && !philos->data->died)
    {
        philos->data->died = true;
		philos->data->stop_sim = true;
        printf("%ld %d died\n", current_time, philos->id + 1);
    }
    pthread_mutex_unlock(&philos->data->log_mutex);
}

void	start_meal(t_philo *philos)
{
	print_state(philos, EATING);
	pthread_mutex_lock(&philos->meal_mutex);
	philos->last_meal = timestamp(philos->data);
	pthread_mutex_unlock(&philos->meal_mutex);
	ft_usleep(philos, philos->t_to_eat);
	pthread_mutex_unlock(&philos->r_fork->hold);
	pthread_mutex_unlock(&philos->l_fork->hold);
}

int	thread_err(t_data *data, char *msg, int count)
{
	while (count--)
		pthread_join(data->philos[count].thread, NULL);
	cleanup(data, 1);
	printf("%s\n", msg);
	return (-1);
}
