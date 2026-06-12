/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 19:53:03 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/12 19:53:03 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*philo_alone(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_action(philo, "has taken a fork");
	time_sleep(philo, philo->context->t_die);
	pthread_mutex_unlock(philo->l_fork);
	return (NULL);
}

long	philo_lastmeal(t_philo *philo)
{
	long	last_meal;

	pthread_mutex_lock(&philo->mtx_state);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->mtx_state);
	return (last_meal);
}

void	*philo_routine(void *philo_ctx)
{
	t_philo	*philo;

	philo = ((t_philo *)philo_ctx);
	if (philo->context->n_philos == 1)
		return (philo_alone(philo));
	if (philo->id % 2 == 0)
		time_sleep(philo, philo->context->t_eat / 2);
	while (!simulation_over(philo->context))
	{
		if (philo_action_forks(philo))
			break ;
		if (philo_action_eat(philo))
			break ;
		if (philo_action_sleep(philo))
			break ;
		if (philo_action_think(philo))
			break ;
	}
	return (NULL);
}
