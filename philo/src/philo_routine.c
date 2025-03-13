/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:33:27 by clu               #+#    #+#             */
/*   Updated: 2025/03/13 10:34:46 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * The main routine for each philosopher thread.
 * Implements a host mechanism to allow at most (N-1) philosophers to eat concurrently.
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int		host_sleep_interval;

	philo = (t_philo *)arg;
	data = philo->data;
	host_sleep_interval = SLEEP_INTERVAL;

	// For fairness: delay even-numbered philosophers slightly.
	if (philo->id % 2 == 0)
		ft_usleep(10);

	// Special case: single philosopher.
	if (data->num_philos == 1)
	{
		print_status(philo, FORK_TAKEN);
		ft_usleep(data->time_to_die);
		pthread_mutex_lock(&data->print_mutex);
		if (!data->sim_stopped)
		{
			data->sim_stopped = true;
			printf("%lld %d died\n", get_time() - data->start_time, philo->id);
			fflush(stdout);
		}
		pthread_mutex_unlock(&data->print_mutex);
		return (NULL);
	}
	// Dynamically adjust the host busy-wait sleep interval.
	// Use a very short interval for long simulations to minimize accumulated delay.
	if (data->sim_duration > 10000)
		host_sleep_interval = 1;  // 1 microsecond for long simulations (e.g., 30 seconds)
	else if (data->time_to_die <= data->time_to_eat + data->time_to_sleep + 50)
		host_sleep_interval = 10;
	else
		host_sleep_interval = SLEEP_INTERVAL; // default (e.g., 100 microseconds)

	while (!data->sim_stopped)
	{
		// Host mechanism: wait until fewer than (N-1) philosophers are eating.
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
			if (data->sim_stopped)
				break;
			usleep(host_sleep_interval);
		}
		if (data->sim_stopped)
			break;
		// Attempt to take forks.
		if (!take_forks(philo))
		{
			pthread_mutex_lock(&data->host_mutex);
			data->eating_philos--;
			pthread_mutex_unlock(&data->host_mutex);
			break;
		}
		// Eat: update last_meal and increment times_eaten.
		if (!eat(philo))
		{
			pthread_mutex_lock(&data->host_mutex);
			data->eating_philos--;
			pthread_mutex_unlock(&data->host_mutex);
			break;
		}
		put_forks(philo);
		// Finished eating; signal the host that this philosopher is done.
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
 * Attempt to take both forks in order to avoid deadlock.
 */
bool	take_forks(t_philo *philo)
{
    t_data	*data = philo->data;
    // Even-numbered philosopher: pick right fork first.
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&data->forks[philo->right_fork]);
        print_status(philo, FORK_TAKEN);
        pthread_mutex_lock(&data->forks[philo->left_fork]);
        print_status(philo, FORK_TAKEN);
    }
    else // odd-numbered: pick left fork first.
    {
        pthread_mutex_lock(&data->forks[philo->left_fork]);
        print_status(philo, FORK_TAKEN);
        pthread_mutex_lock(&data->forks[philo->right_fork]);
        print_status(philo, FORK_TAKEN);
    }
    if (data->sim_stopped)
        return (false);
    return (true);
}

/**
 * Philosopher eats: updates last_meal and increments times_eaten.
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
