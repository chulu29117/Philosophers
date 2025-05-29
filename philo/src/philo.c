/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:43:08 by clu               #+#    #+#             */
/*   Updated: 2025/05/29 00:35:05 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (!parse_args(ac, av, &data))
	{
		printf("%s", USAGE_1);
		printf("%s", USAGE_2);
		printf("%s", USAGE_3);
		return (1);
	}
	if (!init_data(&data) || !init_mutexes(&data) || !init_philos(&data)
		|| !start_sim(&data))
	{
		printf("Initialization failed\n");
		cleanup(&data);
		return (1);
	}
	cleanup(&data);
	return (0);
}
