/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:06:37 by clu               #+#    #+#             */
/*   Updated: 2025/05/31 14:45:33 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** Initialize the philo sim.
** Set up the table, forks, and philosophers.
** Create threads for each philosopher.
** Cleanup on exit.
*/
int	main(int argc, char **argv)
{
	t_table	table;

	table.forks = NULL;
	table.philos = NULL;
	if (argc < 5 || argc > 6)
		return (handle_err(NULL, USAGE, 0));
	if (set_table(&table, argv) < 0)
		return (-1);
	if (init_threads(&table) < 0)
		return (-1);
	cleanup(&table, 1);
	return (0);
}
