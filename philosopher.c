/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 19:55:49 by ecullier          #+#    #+#             */
/*   Updated: 2023/11/12 18:32:26 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

//action
//Fonction pour simuler les actions d'un philosophe ramassant la fourchette droite

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
// Fonction pour simuler les actions d'un philosophe ramassant la fourchette gauche

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
//init
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
	if (check_errors(ac, av) == -1)
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

void	init_forks(t_philos *philoss, pthread_mutex_t *forks, int n)
{
	int	i;

	pthread_mutex_init(&forks[0], NULL);
	philoss[0].fourchette_g = &forks[0];
	philoss[0].fourchette_d = NULL;
	philoss[0].fourchette_d = &forks[n - 1];
	i = 1;
	while (i < n)
	{
		pthread_mutex_init(&forks[i], NULL);
		philoss[i].fourchette_g = &forks[i];
		philoss[i].fourchette_d = &forks[i - 1];
		i++;
	}
}

//main
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
	init_philoss(philos, &args);
	init_forks(philos, forks, args.nbr_of_philo);
	init_thread(philos, &args, thread);
	destroy_mutex(&args, forks);
	ft_free(philos, thread, forks);
	return (0);
}

// utils
int	itoa_len(long nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		len++;
	if (nb < 0)
	{
		nb *= -1;
		len++;
	}
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int nb)
{
	char	*str;
	long	n;
	int		i;

	n = nb;
	i = itoa_len(n);
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	str[i--] = '\0';
	if (n == 0)
		str[0] = 48;
	if (n < 0)
	{
		str[0] = '-';
		n = n * -1;
	}
	while (n > 0)
	{
		str[i] = 48 + (n % 10);
		n = n / 10;
		i--;
	}
	return (str);
}

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (src[j])
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

int	create_string(t_philos *philos, char *status, char **str)
{
	char	*ptr[3];
	int		reslen;
	int		i;

	ptr[0] = ft_itoa((int)(get_time() - philos->args->sart_programme));
	ptr[1] = ft_itoa(philos->id_philo);
	ptr[2] = status;
	reslen = ft_strlen(ptr[0]) + ft_strlen (ptr[1]) + ft_strlen(ptr[2]) + 4;
	*str = malloc(reslen);
	*str[0] = '\0';
	i = 0;
	while (i < 3)
	{
		ft_strcat(*str, ptr[i]);
		if (i < 2)
		{
			ft_strcat(*str, " ");
			free(ptr[i]);
		}
		i++;
	}
	ft_strcat(*str, "\n");
	return (reslen);
}

void	print_status(t_philos *philos, char *status)
{
	char	*str;
	int		len;

	pthread_mutex_lock(&philos->args->mutex_stop);
	if (philos->args->stop == 1)
	{
		pthread_mutex_unlock(&philos->args->mutex_stop);
		return ;
	}
	if (!ft_strcmp(status, DIE))
		philos->args->stop = 1;
	pthread_mutex_unlock(&philos->args->mutex_stop);
	len = create_string(philos, status, &str);
	write(1, str, len);
	free(str);
}


int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

long	ft_atoi(char *str)
{
	int		i;
	long	res;
	int		sign;

	i = 0;
	res = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}
//routine

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
//time
u_int64_t	get_time(void)
{
	unsigned int	ret;
	struct timeval	time;

	ret = 0;
	if (gettimeofday(&time, NULL) == -1)
	{
		perror(ERROR_GETTIME);
		return (0);
	}
	ret = (time.tv_sec *(u_int64_t) 1000 + time.tv_usec / 1000);
	return (ret);
}

/*void	timing(unsigned int time_in_ms)
{
	unsigned int	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}*/

void	timing(u_int64_t time_msg)//timing
{
	u_int64_t	start_time;
	u_int64_t	current_time;

	start_time = get_time();
	current_time = start_time;
	while (current_time - start_time < time_msg)
	{
		usleep(time_msg / 10);
		current_time = get_time();
	}
}
