/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:30:42 by ecullier          #+#    #+#             */
/*   Updated: 2023/11/12 18:32:47 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <sys/time.h>
# include <time.h>

# define ERROR_NUM_ARGS "Error: invalid number of arguments"
# define ERROR_INVALID_ARGS "Error: invalid input arguments"
# define ERROR_MALLOC "Error: malloc"
# define ERROR_INIT_FORKS "Error: init forks"
# define ERROR_DATA_MUTEX "Error: init data mutex"
# define ERROR_INIT_PHILO "Error: init philos"
# define ERROR_CREATE_PTHREAD "Error: init phtread"
# define ERROR_PTHREAD_JOIN "Error: init phtread_join"
# define ERROR_GETTIME "Error: gettimeofday() failure "
# define HAPPY_END "Dinner is over 🤮 !"
# define TAKE_FORK "\033[1;34mhas taking a fork\033[0m"
# define EATING "\033[1;32mis 🍔 eating\033[0m"
# define SLEEPING "\033[1;35mis 💤 sleeping\033[0m"
# define THINKING "\033[1;36mis 🤯 thinking\033[0m"
# define DIE "\033[1;31m 💀 RIP\033[0m"

//structure
typedef pthread_mutex_t	t_mutex;

typedef struct 		s_arg
{
	int				nbr_of_philo;
	u_int64_t			time_to_die;
	u_int64_t			time_to_eat;
	u_int64_t			time_to_sleep;
	unsigned long int			sart_programme;
	int				nbr2repas;
	int				stop;

	t_mutex			mutex_stop;
	t_mutex			m_prt_msg;
	t_mutex			m_last_eat;


}		t_arg;

typedef	struct		s_philos
{
	t_arg				*args;
	u_int64_t			time_die;
	u_int64_t			time_eat;
	u_int64_t			time_sleep;
   	pthread_t 			pid_pthread;
	int				id_philo;
	int				n;
	int				count;
	int				count_eat;
	int				stop;
	u_int64_t				rip_time;
	t_mutex			*fourchette_d;
	t_mutex			*fourchette_g;
	pthread_t		rip;


}			t_philos;


int		check_errors(int ac, char **av);


void	philo_2_d(t_philos *philos);
void	philo_2_g(t_philos *philos);
void	eating(t_philos *philos);
void	philo_action(t_philos *philos);

int	init_info(int ac, char **av, t_arg *args);
void	init_philos(t_philos *philos, t_arg *args);

void	init_forks(t_philos *philoss, pthread_mutex_t *forks, int n);
void	ft_free(t_philos *philos, pthread_t *thread, pthread_mutex_t *forks);
int	malloc_error(t_philos *philos, pthread_t *thread, pthread_mutex_t *forks);
int	initialized(t_philos **philos, pthread_t **thread,
		pthread_mutex_t **forks, int n);
//utiles
int	itoa_len(long nb);
char	*ft_itoa(int nb);
long	ft_atoi(char *str);
char	*ft_strcat(char *dest, char *src);
int	create_string(t_philos *philo, char *status, char **str);
void	print_status(t_philos *philo, char *status);
int	ft_strlen(char *str);
int	ft_strcmp(const char *s1, const char *s2);


void	*rip(void *arg);
void	*routine(void *arg);
void	destroy_mutex(t_arg *args, pthread_mutex_t *forks);
int	init_thread(t_philos *philo, t_arg *args, pthread_t *thread);

//time
u_int64_t	get_time(void);
void	timing(u_int64_t time_msg);

#endif