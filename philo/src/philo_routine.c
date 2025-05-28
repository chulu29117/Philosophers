/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 23:23:44 by clu               #+#    #+#             */
/*   Updated: 2025/05/28 02:54:36 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Attempt to take both forks in a deadlock-preventing order.
** Ordering by lowest‐index first.
*/
bool take_forks(t_philo *philo)
{
	t_data *data;
	int		first;
	int		second;
	int		tmp;

	data = philo->data;
	first = philo->left_fork;
	second = philo->right_fork;
	if (first > second)
	{
		tmp    = first;
		first  = second;
		second = tmp;
	}
	pthread_mutex_lock(&data->forks[first]);
	print_status(philo, FORK_TAKEN);
	pthread_mutex_lock(&data->forks[second]);
	print_status(philo, FORK_TAKEN);
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
	ft_usleep(data->time_to_sleep);
	print_status(philo, THINKING);
}

/*
** Special case handling for a single philosopher
*/
void	one_philo(t_philo *philo)
{
	t_data *data;

	data = philo->data;
	// single philosopher takes left fork, simulates eating and sleeping
	pthread_mutex_lock(&data->forks[philo->left_fork]);
	print_status(philo, FORK_TAKEN);
	// starve until time_to_die
	ft_usleep(data->time_to_die);
	pthread_mutex_lock(&data->print_mutex);
	if (!data->sim_stopped)
	{
		// print death status
		printf("%lld %d died\n", get_time() - data->start_time, philo->id);
		data->sim_stopped = true;
	}
	pthread_mutex_unlock(&data->print_mutex);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	stop_sim(data);
}

/*
** One full life cycle: wait for slot, take forks, eat, release slot,
** then put forks, sleep, think.
*/
bool	philo_cycle(t_philo *philo)
{
	/* 1) Try to acquire forks.  If we fail immediately, back off. */
	if (!take_forks(philo))
		return (true);
	/* 2) Eat: update last meal and increment times eaten */
	if (!eat(philo))
	{
		put_forks(philo);
		return (false);
	}
	/* 3) Release forks */
	put_forks(philo);
	/* 4) Sleep and think */
	sleep_think(philo);
	return (true);
}

/*
** Helper to check the stop flag under mutex.
*/
bool	check_stop(t_data *data)
{
	bool	stopped;

	pthread_mutex_lock(&data->print_mutex);
	stopped = data->sim_stopped;
	pthread_mutex_unlock(&data->print_mutex);
	return (stopped);
}

/*
** Philosopher thread routine: life cycle until simulation stops
*/
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	/* Handle the single-philosopher case immediately, before any sleeps */
	if (data->num_philos == 1)
	{
		one_philo(philo);
		return (NULL);
	}
	/* Stagger odd IDs by half an eating cycle to reduce contention */
	if (philo->id % 2 == 0)
		ft_usleep(data->time_to_eat);
	/* Main loop: cycle until death or (optional) meal limit */
	while (true)
	{
		if (check_stop(data))
			break ;
		pthread_mutex_lock(&data->philo->meal_mutex);
		if (data->sim_stopped)
		{
			pthread_mutex_unlock(&data->philo->meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->philo->meal_mutex);
		if (!philo_cycle(philo))
			break ;
		/* Exit once this philosopher reached eat count, if set */
		if (data->sim_duration && philo->times_eaten >= data->num_times_to_eat)
			break ;
	}
	return (NULL);
}
