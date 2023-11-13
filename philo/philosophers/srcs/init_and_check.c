/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:21:32 by ecullier          #+#    #+#             */
/*   Updated: 2023/11/13 10:35:58 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

int	check_errors(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (ft_atoi(av[i]) == 0 || ft_atoi(av[i]) > INT_MAX \
			|| ft_atoi(av[i]) < 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_info(int ac, char **av, t_arg *args)
{
	if (check_errors(ac, av) == 1)
		return (-1);
	args->nbr_of_philo = (int)ft_atoi(av[1]);
	args->time_to_die = (u_int64_t)ft_atoi(av[2]);
	args->time_to_eat = (u_int64_t)ft_atoi(av[3]);
	args->time_to_sleep = (u_int64_t)ft_atoi(av[4]);
	if (ac == 6)
		args->nbr2repas = (int)ft_atoi(av[5]);
	else
		args->nbr2repas = -1;
	args->stop = 0;
	pthread_mutex_init(&args->mutex_stop, NULL);
	pthread_mutex_init(&args->m_prt_msg, NULL);
	pthread_mutex_init(&args->m_last_eat, NULL);
	return (0);
}
