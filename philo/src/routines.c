/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:41:15 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 13:03:51 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	eating(t_philo *philos)
{
	if (philos->table->stop == true)
		return ;
	while (!philos->l_fork->free)
		check_death(philos, SLEEP);
	pthread_mutex_lock(&philos->l_fork->hold);
	if (try_l_fork(philos) == true)
		return ;
	print_state(philos, FORK);
	if (philos->table->n_philos == 1)
		return (single_philo(philos));
	while (!philos->r_fork->free)
		check_death(philos, SLEEP);
	pthread_mutex_lock(&philos->r_fork->hold);
	if (try_r_fork(philos) == true)
		return ;
	start_eating(philos);
}	

void	sleeping(t_philo *philos)
{
	if (philos->table->stop == true)
		return ;
	print_state(philos, SLEEP);
	ft_usleep(philos, philos->t_to_sleep);
}

void	thinking(t_philo *philos)
{
	if (philos->table->stop == true)
		return ;
	print_state(philos, THINK);
	usleep(1000);
}

void	wait_start(t_philo *philos)
{
	if (philos->table->stop == true)
		return ;
	print_state(philos, THINK);
	ft_usleep(philos, 10);
}

void	*philo_routines(void *arg)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)arg;
	i = -1;
	if (philos->table->n_philos % 2 != 0)
		wait_start(philos);
	else
		thinking(philos);
	while (philos->table->stop == false)
	{
		if (philos->table->stop == true)
			break ;
		if (check_death(philos, SLEEP) == true)
			break ;
		if (philos->eat_count != -1 && i++ == philos->eat_count)
			break ;
		eating(philos);
		if (philos->table->n_philos != 1)
		{
			sleeping(philos);
			thinking(philos);
		}
	}
	return (NULL);
}
