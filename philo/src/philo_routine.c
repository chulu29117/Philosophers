/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 23:23:44 by clu               #+#    #+#             */
/*   Updated: 2025/05/29 11:05:27 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** Handle the single-philosopher special case: take one fork and wait to die.
*/
static void	single_philo(t_philo *philos)
{
	pthread_mutex_lock(&philos->data->forks[philos->id - 1].lock);
	print_status(philos, FORK_TAKEN);
	ft_usleep(philos->data->t_to_die, philos);
	print_status(philos, DIED);
	pthread_mutex_unlock(&philos->data->forks[philos->id - 1].lock);
}

/*
** Determine fork order to avoid deadlock, then take them.
*/
static bool	pick_forks(t_philo *philos, int *first, int *second)
{
	int	left;
	int	right;

	left = philos->id - 1;
	right = philos->id % philos->data->num_philos;
	if (philos->id % 2 == 0)
	{
		*first = left;
		*second = right;
	}
	else
	{
		*first = right;
		*second = left;
	}
	pthread_mutex_lock(&philos->data->forks[*first].lock);
	print_status(philos, FORK_TAKEN);	
	pthread_mutex_lock(&philos->data->forks[*second].lock);
	print_status(philos, FORK_TAKEN);
	return (true);
}

/*
** Eat, then release forks, then sleep and think.
*/
static void	eat_sleep_think(t_philo *philos, int first, int second)
{
	/* eat */
	pthread_mutex_lock(&philos->meal_mutex);
	philos->last_meal = timestamp();
	philos->meals++;
	print_status(philos, EATING);
	pthread_mutex_unlock(&philos->meal_mutex);

	ft_usleep(philos->data->t_to_eat, philos);

	pthread_mutex_unlock(&philos->data->forks[first].lock);
	pthread_mutex_unlock(&philos->data->forks[second].lock);
	
	/* sleep and think */
	print_status(philos, SLEEPING);
	ft_usleep(philos->data->t_to_sleep, philos);
	print_status(philos, THINKING);
}

/*
** Main philosopher routine: loop until stop or meal limit reached.
*/
void	*philo_routine(void *arg)
{
	t_philo	*philos;
	int		first;
	int		second;

	philos = (t_philo *)arg;
	if (philos->data->num_philos == 1)
	{
		single_philo(philos);
		return (NULL);
	}
	if (philos->id % 2 == 0)
		usleep(SLEEP_INTERVAL);
	while (!philos->data->stop)
	{
		if (!pick_forks(philos, &first, &second))
			break ;
		eat_sleep_think(philos, first, second);
		if (philos->data->max_meals > 0
			&& philos->meals >= philos->data->max_meals)
			break ;
	}
	return (NULL);
}
