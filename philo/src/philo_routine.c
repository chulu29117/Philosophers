/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:33:27 by clu               #+#    #+#             */
/*   Updated: 2025/03/12 11:31:51 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * The main routine for each philosopher thread.
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	/* If there is only 1 philosopher, they will never get a second fork. */
	if (philo->data->num_philos == 1)
	{
		print_status(philo, FORK_TAKEN);
		ft_usleep(philo->data->time_to_die);
		print_status(philo, DIED);
		return (NULL);
	}
	while (!philo->data->sim_stopped)
	{
		while (1)
		{
			pthread_mutex_lock(&data->host_mutex);
			if (data->eating_philos < data->num_philos - 1)
			{
				data->eating_philos++;
				pthread_mutex_unlock(&data->host_mutex);
				break;
			}
			pthread_mutex_unlock(&data->host_mutex);
			if (data->sim_stopped) // Check if simulation is stopped before sleeping
				break;
			usleep(100); // Sleep briefly (100 microseconds) before checking again
		}
		if (data->sim_stopped)  // If sim_stopped is true, break out of the outer loop
    		break;
		// Now proceed with picking up forks, eating, etc.
		if (!take_forks(philo))
		{
			// If something goes wrong, make sure to decrement the counter
			pthread_mutex_lock(&data->host_mutex);
			data->eating_philos--;
			pthread_mutex_unlock(&data->host_mutex);
			break;
		}
		if (!eat(philo))
		{
			pthread_mutex_lock(&data->host_mutex);
			data->eating_philos--;
			pthread_mutex_unlock(&data->host_mutex);
			break;
		}
		put_forks(philo);
		// Done eating; signal the host that this philosopher is finished
		pthread_mutex_lock(&data->host_mutex);
		data->eating_philos--;
		pthread_mutex_unlock(&data->host_mutex);
		if (data->sim_stopped)
			break;
		sleep_think(philo);
	}
	return (NULL);
}

/**
 * Attempt to take both forks. Order the lock to avoid deadlock (smaller index first).
 */
bool	take_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		print_status(philo, FORK_TAKEN);
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		print_status(philo, FORK_TAKEN);
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		print_status(philo, FORK_TAKEN);
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		print_status(philo, FORK_TAKEN);
	}
	if (data->sim_stopped)
		return (false);
	return (true);
}

/**
 * Philosopher eats: updates last meal time and increments the times eaten.
 */
bool	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	print_status(philo, EATING);
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);

	ft_usleep(data->time_to_eat);
	if (data->sim_stopped)
		return (false);
	return (true);
}

/**
 * Release both forks.
 */
void	put_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
}

/**
 * Philosopher sleeps and then thinks.
 */
void	sleep_think(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(philo, SLEEPING);
	ft_usleep(data->time_to_sleep);
	if (data->sim_stopped)
		return;
	print_status(philo, THINKING);
}
