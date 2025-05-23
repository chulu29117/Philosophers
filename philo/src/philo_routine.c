/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 23:23:44 by clu               #+#    #+#             */
/*   Updated: 2025/05/22 11:50:13 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Attempt to take both forks in a deadlock-preventing order.
** Even-numbered philosopher: pick right fork first.
** Odd-numbered philosopher: pick left fork first.
*/
bool	take_forks(t_philo *philo)
{
	t_data *data;

	data = philo->data;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		print_status(philo, FORK_TAKEN);
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		print_status(philo, FORK_TAKEN);
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		print_status(philo, FORK_TAKEN);
		pthread_mutex_destroy(&data->forks[philo->right_fork]);
		print_status(philo, FORK_TAKEN);
	}
	if (data->sim_stopped)
		return (false);
	return (true);
}

/*
** Philosopher eats: updates last_meal and increments times_eaten.
*/
bool	eat(t_philo *philo)
{
	t_data *data;
	
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

/*
** Release both forks.
*/
void	put_forks(t_philo *philo)
{
	t_data *data;
	
	data = philo->data;
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
}

/*
** Philosopher sleeps and then thinks.
*/
void	sleep_think(t_philo *philo)
{
	t_data *data;

	data = philo->data;
	print_status(philo, SLEEPING);
	ft_usleep(data->time_to_die);
	if (data->sim_stopped)
		return ;
	print_status(philo, THINKING);
}

/*
** Special case handling for a single philosopher
*/
void	one_philo(t_philo *philo)
{
	t_data *data;

	data = philo->data;
	pthread_mutex_lock(&data->fork[philo->left_fork]);
	print_status(philo, FORK_TAKEN);
	ft_usleep(data->time_to_die);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
}

/*
** Philosopher life cycle (eating-sleeping-thinking),
** separated to keep functions under 25 lines for norminette
*/
bool	philo_cycle(t_philo *philo)
{
	t_data *data;

	data = philo->data;
	if (!take_forks(philo))
		return (false);
	if (!eat(philo))
	{
		put_forks(philo);
		return (false);
	}
	put_forks(philo);
	if (data->sim_stopped)
		return (false);
	sleep_think(philo);
	return (true);
}

/*
** Philosopher thread routine: life cycle until simulation stops
*/
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	// retrieve philosopher struct
	philo = (t_philo *)arg;
	data = philo->data;
	// stagger starts to reduce contention
	if (philo->id % 2 == 0)
		ft_usleep(10);
	// single philo case
	if (data->num_philos == 1)
	{
		one_philo(philo);
		return (NULL);
	}
	while (!data->sim_stopped)
	{
		if (!philo_cycle(philo))
			break ;
		pthread_mutex_lock(&data->host_mutex);
		if (data->sim_stopped)
		{
			pthread_mutex_unlock(&data->host_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->host_mutex);
	}
	return (NULL);
}
