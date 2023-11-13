/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:12:21 by ecullier          #+#    #+#             */
/*   Updated: 2023/11/13 09:16:41 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

void	timing(u_int64_t time_msg)
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
