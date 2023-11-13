/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:13:05 by ecullier          #+#    #+#             */
/*   Updated: 2023/11/13 09:18:53 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

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
