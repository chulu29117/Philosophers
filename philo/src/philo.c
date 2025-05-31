/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:06:37 by clu               #+#    #+#             */
/*   Updated: 2025/05/31 21:21:28 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** Parse and validate the arguments.
** Set up the table stuct, with forks, philos, and mutexes.
** Start the philo threads and a monitor thread.
** Join all threads and clean up resources upon completion.
*/
int	main(int argc, char **argv)
{
	t_table		table;
	pthread_t	monitor_thread;

	if (argc < 5 || argc > 6)
		return (handle_err(NULL, USAGE, 0));
	table.forks = NULL;
	table.philos = NULL;
	if (set_table(&table, argv) < 0)
		return (-1);
	if (start_threads(&table, &monitor_thread) < 0)
		return (-1);
	if (join_threads(&table, monitor_thread) < 0)
		return (-1);
	cleanup(&table, 1);
	return (0);
}
