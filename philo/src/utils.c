/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 23:35:10 by clu               #+#    #+#             */
/*   Updated: 2025/03/12 10:58:16 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

/**
 * Returns the current time in milliseconds.
 */
long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)(tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * Custom usleep that accounts for potential inaccuracies of usleep.
 * Sleeps in small increments until the desired time has elapsed.
 */
void	ft_usleep(long long time_in_ms)
{
	long long	start;

	start = get_time();
	while ((get_time() - start) < time_in_ms)
		usleep(100);
}

/**
 * Prints a status message (e.g., "is eating", "has taken a fork", etc.)
 * Uses the print_mutex to avoid interleaved prints.
 */
void	print_status(t_philo *philo, int status)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->sim_stopped)
	{
		timestamp = get_time() - philo->data->start_time;
		if (status == FORK_TAKEN)
			printf("%lld %d has taken a fork\n", timestamp, philo->id);
		else if (status == EATING)
			printf("%lld %d is eating\n", timestamp, philo->id);
		else if (status == SLEEPING)
			printf("%lld %d is sleeping\n", timestamp, philo->id);
		else if (status == THINKING)
			printf("%lld %d is thinking\n", timestamp, philo->id);
		else if (status == DIED)
			printf("%lld %d died\n", timestamp, philo->id);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

/**
 * Simple function to print an error message and exit.
 */
void	error_exit(char *msg)
{
	printf("%s\n", msg);
	exit(EXIT_FAILURE);
}
