/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:11:45 by ecullier          #+#    #+#             */
/*   Updated: 2023/11/13 09:26:56 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

int	init_thread(t_philos *philos, t_arg *args, pthread_t *thread)
{
	int	i;

	i = 0;
	args->sart_programme = get_time();
	while (i < args->nbr_of_philo)
	{
		if (pthread_create(&thread[i], NULL, &routine, &philos[i]))
			perror(ERROR_CREATE_PTHREAD);
		i++;
	}
	i = 0;
	while (i < args->nbr_of_philo)
	{
		if (pthread_join(thread[i], NULL))
			perror(ERROR_PTHREAD_JOIN);
		i++;
	}
	return (0);
}

void	init_philos(t_philos *philos, t_arg *args)
{
	int	i;

	i = 0;
	while (i < args->nbr_of_philo)
	{
		philos[i].args = args;
		philos[i].id_philo = i + 1;
		philos[i].n = args->nbr_of_philo;
		philos[i].count = 0;
		philos[i].stop = 0;
		philos[i].time_die = args->time_to_die;
		philos[i].time_eat = args->time_to_eat;
		philos[i].time_sleep = args->time_to_sleep;
		philos[i].count_eat = args->nbr2repas;
		i++;
	}
}

void	init_forks(t_philos *philos, pthread_mutex_t *forks, int n)
{
	int	i;

	pthread_mutex_init(&forks[0], NULL);
	philos[0].fourchette_g = &forks[0];
	philos[0].fourchette_d = NULL;
	philos[0].fourchette_d = &forks[n - 1];
	i = 1;
	while (i < n)
	{
		pthread_mutex_init(&forks[i], NULL);
		philos[i].fourchette_g = &forks[i];
		philos[i].fourchette_d = &forks[i - 1];
		i++;
	}
}

int	malloc_error(t_philos *philos, pthread_t *thread, pthread_mutex_t *forks)
{
	ft_free(philos, thread, forks);
	perror("philos");
	return (-1);
}

int	initialized(t_philos **philos, pthread_t **thread,
		pthread_mutex_t **forks, int n)
{
	*philos = malloc(sizeof(t_philos) * n);
	if (!*philos)
		return (malloc_error(*philos, *thread, *forks));
	*thread = malloc(sizeof(pthread_t) * n);
	if (!*thread)
		return (malloc_error(*philos, *thread, *forks));
	*forks = malloc(sizeof(pthread_mutex_t) * n);
	if (!*forks)
		return (malloc_error(*philos, *thread, *forks));
	return (0);
}
