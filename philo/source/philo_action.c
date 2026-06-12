/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 19:53:03 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/12 19:53:03 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

static void	s_unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

int	philo_action_forks(t_philo *philo)
{
	if (simulation_over(philo->context))
		return (1);
	pthread_mutex_lock(philo->l_fork);
	if (print_action(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(philo->l_fork);
		return (1);
	}
	pthread_mutex_lock(philo->r_fork);
	if (print_action(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		return (1);
	}
	return (0);
}

int	philo_action_eat(t_philo *philo)
{
	if (simulation_over(philo->context))
	{
		s_unlock_forks(philo);
		return (1);
	}
	pthread_mutex_lock(&philo->mtx_state);
	philo->last_meal = time_now();
	philo->eaten++;
	pthread_mutex_unlock(&philo->mtx_state);
	if (print_action(philo, "is eating"))
	{
		s_unlock_forks(philo);
		return (1);
	}
	time_sleep(philo, philo->context->t_eat);
	s_unlock_forks(philo);
	return (simulation_over(philo->context));
}

int	philo_action_sleep(t_philo *philo)
{
	if (print_action(philo, "is sleeping"))
		return (1);
	time_sleep(philo, philo->context->t_sleep);
	return (simulation_over(philo->context));
}

int	philo_action_think(t_philo *philo)
{
	if (print_action(philo, "is thinking"))
		return (1);
	if (philo->context->n_philos % 2 != 0)
		time_sleep(philo, philo->context->t_eat / 2);
	return (simulation_over(philo->context));
}
