/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 19:53:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/12 19:53:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

int	monitor_satisfied(t_context *context)
{
	int	i;

	if (context->max_eat == -1)
		return (0);
	i = 0;
	while (i < context->n_philos)
	{
		pthread_mutex_lock(&context->philos[i].mtx_state);
		if (context->philos[i].eaten < context->max_eat)
		{
			pthread_mutex_unlock(&context->philos[i].mtx_state);
			return (0);
		}
		pthread_mutex_unlock(&context->philos[i].mtx_state);
		i++;
	}
	return (1);
}

int	monitor_start(t_context *context)
{
	int		i;
	long	t_lmeal;

	while (!simulation_over(context))
	{
		i = 0;
		while (i < context->n_philos)
		{
			t_lmeal = philo_lastmeal(&context->philos[i]);
			if (time_lapsed(t_lmeal) >= context->t_die)
				return (action_death(context, context->philos[i].id));
			i++;
		}
		if (monitor_satisfied(context))
			return (action_alleat(context));
		usleep(100);
	}
	return (0);
}
