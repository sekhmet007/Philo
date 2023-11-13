/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 19:55:49 by ecullier          #+#    #+#             */
/*   Updated: 2023/11/13 09:45:23 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

//action
//Fonction pour simuler les actions d'un philosophe 
//ramassant la fourchette droite

void	philos_2_d(t_philos *philos)
{
	pthread_mutex_lock(philos->fourchette_d);
	pthread_mutex_lock(&philos->args->m_prt_msg);
	print_status(philos, TAKE_FORK);
	pthread_mutex_unlock(&philos->args->m_prt_msg);
	if (philos->n == 1)
	{
		pthread_mutex_unlock(philos->fourchette_d);
		timing(philos->time_die);
		return ;
	}
	pthread_mutex_lock(philos->fourchette_g);
	pthread_mutex_lock(&philos->args->m_prt_msg);
	print_status(philos, TAKE_FORK);
	pthread_mutex_unlock(&philos->args->m_prt_msg);
	pthread_mutex_lock(&philos->args->m_prt_msg);
	print_status(philos, EATING);
	pthread_mutex_unlock(&philos->args->m_prt_msg);
	philos->count++;
	pthread_mutex_lock(&philos->args->m_last_eat);
	philos->rip_time = get_time();
	pthread_mutex_unlock(&philos->args->m_last_eat);
	timing(philos->time_eat);
	pthread_mutex_unlock(philos->fourchette_d);
	pthread_mutex_unlock(philos->fourchette_g);
}
// Fonction pour simuler les actions d'un philosophe 
//ramassant la fourchette gauche

void	philos_2_g(t_philos *philos)
{
	pthread_mutex_lock(philos->fourchette_g);
	pthread_mutex_lock(&philos->args->m_prt_msg);
	print_status(philos, TAKE_FORK);
	pthread_mutex_unlock(&philos->args->m_prt_msg);
	if (philos->n == 1)
	{
		pthread_mutex_unlock(philos->fourchette_g);
		timing(philos->time_die);
		return ;
	}
	pthread_mutex_lock(philos->fourchette_d);
	pthread_mutex_lock(&philos->args->m_prt_msg);
	print_status(philos, TAKE_FORK);
	pthread_mutex_unlock(&philos->args->m_prt_msg);
	pthread_mutex_lock(&philos->args->m_prt_msg);
	print_status(philos, EATING);
	pthread_mutex_unlock(&philos->args->m_prt_msg);
	philos->count++;
	pthread_mutex_lock(&philos->args->m_last_eat);
	philos->rip_time = get_time();
	pthread_mutex_unlock(&philos->args->m_last_eat);
	timing(philos->time_eat);
	pthread_mutex_unlock(philos->fourchette_g);
	pthread_mutex_unlock(philos->fourchette_d);
}

void	ft_free(t_philos *philos, pthread_t *thread, pthread_mutex_t *forks)
{
	if (philos)
	{
		free(philos);
		philos = NULL;
	}
	if (thread)
	{
		free(thread);
		thread = NULL;
	}
	if (forks)
	{
		free(forks);
		forks = NULL;
	}
}

int	main(int ac, char **av)
{
	t_arg			args;
	t_philos		*philos;
	pthread_t		*thread;
	pthread_mutex_t	*forks;

	if (ac < 5 || ac > 6 || init_info(ac, av, &args) == -1)
	{
		printf(ERROR_INVALID_ARGS);
		return (1);
	}
	philos = NULL;
	thread = NULL;
	forks = NULL;
	if (initialized(&philos, &thread, &forks, args.nbr_of_philo) == -1)
		return (1);
	init_philos(philos, &args);
	init_forks(philos, forks, args.nbr_of_philo);
	init_thread(philos, &args, thread);
	destroy_mutex(&args, forks);
	ft_free(philos, thread, forks);
	return (0);
}
