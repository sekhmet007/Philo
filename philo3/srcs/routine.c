/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:22:49 by ecullier          #+#    #+#             */
/*   Updated: 2023/11/13 09:24:40 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

void	*rip(void *arg)
{
	t_philos	*philos;
	int			stop;

	philos = (t_philos *)arg;
	stop = 0;
	while (!stop)
	{
		pthread_mutex_lock(&philos->args->m_last_eat);
		if (get_time() - philos->rip_time >= philos->time_die)
		{
			pthread_mutex_unlock(&philos->args->m_last_eat);
			pthread_mutex_lock(&philos->args->m_prt_msg);
			print_status(philos, DIE);
			pthread_mutex_unlock(&philos->args->m_prt_msg);
		}
		else
			pthread_mutex_unlock(&philos->args->m_last_eat);
		pthread_mutex_lock(&philos->args->mutex_stop);
		stop = philos->args->stop + philos->stop;
		pthread_mutex_unlock(&philos->args->mutex_stop);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philos	*philos;
	int			stop;

	philos = (t_philos *)arg;
	philos->rip_time = philos->args->sart_programme;
	if (pthread_create(&philos->rip, NULL, &rip, philos))
		perror(ERROR_CREATE_PTHREAD);
	stop = 0;
	while (!stop)
	{
		philos_action(philos);
		pthread_mutex_lock(&philos->args->mutex_stop);
		stop = philos->args->stop + philos->stop;
		pthread_mutex_unlock(&philos->args->mutex_stop);
	}
	if (pthread_join(philos->rip, NULL))
		perror(ERROR_PTHREAD_JOIN );
	return (NULL);
}

void	destroy_mutex(t_arg *args, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < args->nbr_of_philo)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&args->m_last_eat);
	pthread_mutex_destroy(&args->m_prt_msg);
	pthread_mutex_destroy(&args->mutex_stop);
}

// Fonction pour déterminer quelle main (gauche ou droite) le philosophe doit utiliser pour ramasser les fourchettes

void	eating(t_philos *philos)
{
	if (philos->id_philo % 2 == 0)
		philos_2_d(philos);
	else
		philos_2_g(philos);
}
// Fonction pour effectuer les actions du philosophe, y compris manger, dormir et penser

void	philos_action(t_philos *philos)
{
	eating(philos);
	if (philos->count_eat != 1 && philos->count == philos->count_eat)
	{
		pthread_mutex_lock(&philos->args->mutex_stop);
		philos->stop = 1;
		pthread_mutex_unlock(&philos->args->mutex_stop);
		return ;
	}
	pthread_mutex_lock(&philos->args->m_prt_msg);
	print_status(philos, SLEEPING);
	pthread_mutex_unlock(&philos->args->m_prt_msg);
	timing(philos->time_sleep);
	pthread_mutex_lock(&philos->args->m_prt_msg);
	print_status(philos, THINKING);
	pthread_mutex_unlock(&philos->args->m_prt_msg);
	if (philos->n % 2 != 0)
		timing(100);
}