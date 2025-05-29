/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:47:50 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 00:38:13 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_usleep(t_philo *philos, long duration)
{
	long	start;

	start = timestamp(philos->data);
	while (!philos->data->stop_sim
		&& (timestamp(philos->data) - start) < duration)
	{
		usleep(1000);
		check_time(philos, SLEEPING);
	}
}

long	check_time(t_philo *philos, int	state)
{
	if (state == EATING)
		return (timestamp(philos->data) - philos->data->sim_start);
	else if (state == SLEEPING)
	{
		if (timestamp(philos->data) - philos->last_meal >= philos->t_to_die)
		{
			philos->data->stop_sim = true;
			print_state(philos, DIED);
			return (1);
		}
	}
	return (0);
}

void	print_state(t_philo *philos, int state)
{
	if (philos->data->stop_sim && state != DIED)
		return ;
	pthread_mutex_lock(&philos->data->log_mutex);
	if (state == FORK && !philos->data->stop_sim)
		printf("%ld %d has taken a fork\n",
			check_time(philos, 2), philos->id + 1);
	else if (state == EATING && !philos->data->stop_sim)
		printf("%ld %d is eating\n",
			check_time(philos, 2), philos->id + 1);
	else if (state == SLEEPING && !philos->data->stop_sim)
		printf("%ld %d is sleeping\n",
			check_time(philos, 2), philos->id + 1);
	else if (state == THINKING && !philos->data->stop_sim)
		printf("%ld %d is thinking\n",
			check_time(philos, 2), philos->id + 1);
	else if (state == DIED && !philos->data->died)
	{
		philos->data->died = true;
		printf("%ld %d died\n", check_time(philos, 2), philos->id + 1);
	}
	pthread_mutex_unlock(&philos->data->log_mutex);
}

void	start_meal(t_philo *philos)
{
    print_state(philos, EATING);
    philos->last_meal = timestamp(philos->data);
    ft_usleep(philos, philos->t_to_eat);
    
    // Track meals here where we know eating actually happened
    if (philos->data->limit && philos->meal_count > 0)
    {
        pthread_mutex_lock(&philos->data->log_mutex);
        philos->meals_eaten++;
        if (philos->meals_eaten >= philos->meal_count && !philos->full)
        {
            philos->full = true;
            philos->data->full_count++;
        }
        pthread_mutex_unlock(&philos->data->log_mutex);
    }
    
    philos->r_fork->free = true;
    philos->l_fork->free = true;
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
