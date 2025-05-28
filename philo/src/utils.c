/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:51:32 by clu               #+#    #+#             */
/*   Updated: 2025/05/28 02:55:05 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Return current timestamp in milliseconds since the Unix epoch.
*/
long long	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) < 0)
		return (-1);
	return ((time.tv_sec * 1000LL) + (time.tv_usec / 1000LL));
}

/*
** A more precise usleep that checks the stop flag periodically
** so we can break out quickly if the simulation ends.
*/
void	ft_usleep(long long duration_ms)
{
	long long	start;

	start = get_time();
	while (get_time() - start < duration_ms)
		usleep(SLEEP_INTERVAL);
}

/*
** Thread-safe status printer:
** grabs print_mutex, computes timestamp since start,
** prints “<time> <id> <status>\n”, then unlocks.
*/
void	print_status(t_philo *philo, int status)
{
	long long	ts;
	bool		can_print;

	pthread_mutex_lock(&philo->data->print_mutex);
	can_print = !philo->data->sim_stopped;
	ts = get_time() - philo->data->start_time;
	if (can_print)
	{
		if (status == FORK_TAKEN)
			printf("%lld %d has taken a fork\n", ts, philo->id);
		else if (status == EATING)
			printf("%lld %d is eating\n", ts, philo->id);
		else if (status == SLEEPING)
			printf("%lld %d is sleeping\n", ts, philo->id);
		else if (status == THINKING)
			printf("%lld %d is thinking\n", ts, philo->id);
		else if (status == DIED)
		{
			printf("%lld %d died\n", ts, philo->id);
			philo->data->sim_stopped = true;
		}
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

/*
** Check if string contains only digits
*/
bool	is_num(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (false);
	if (str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

/*
** Convert string to integer, with bounds checking
*/
int	ft_atoi(const char *str)
{
	int					sign;
	unsigned long int	res;

	sign = 1;
	res = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
		if (res > LONG_MAX && sign == 1)
			return (-1);
		if (res > LONG_MAX && sign == -1)
			return (0);
	}
	return ((int)(res * sign));
}

/*
** Print usage instructions
*/
int	print_usage(void)
{
	printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat ");
	printf("time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	printf("\nAll arguments must be positive integers.\n");
	return (EXIT_FAILURE);
}

/*
** Stop the simulation by setting the stop flag
*/
void	stop_sim(t_data *data)
{
	pthread_mutex_lock(&data->print_mutex);
	data->sim_stopped = true;
	pthread_mutex_unlock(&data->print_mutex);
}

/*
** Parse and validate command-line arguments
*/
bool	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc != 5 && argc != 6)
		return (false);
	if (!is_num(argv[1]) || !is_num(argv[2])
		|| !is_num(argv[3]) || !is_num(argv[4])
		|| (argc == 6 && !is_num(argv[5])))
		return (false);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->sim_duration = false;
	data->num_times_to_eat = -1;
	if (argc == 6)
	{
		data->num_times_to_eat = ft_atoi(argv[5]);
		data->sim_duration = true;
	}
	if (data->num_philos <= 0 || data->time_to_die < 0)
		return (false);
	return (true);
}

/*
** Destroy up to 'initialized' fork mutexes and free the forks array.
*/
void	cleanup_forks(t_data *data, int initialized)
{
	int	i;
	
	i = 0;
	while (i < initialized)
	{
		// destroy each initialized fork mutex
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
}
