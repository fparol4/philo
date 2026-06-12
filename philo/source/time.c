/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 19:53:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/12 19:53:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

long	time_now(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000L + (time.tv_usec / 1000));
}

long	time_lapsed(long timestamp)
{
	return (time_now() - timestamp);
}

void	time_sleep(t_philo *philo, int ms)
{
	long	start;
	long	now;

	start = time_now();
	now = time_now();
	while (now - start < ms)
	{
		if (simulation_over(philo->context))
			break ;
		usleep(500);
		now = time_now();
	}
}
