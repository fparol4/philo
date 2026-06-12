#include "../headers/philo.h"

void *philo_alone(t_philo *philo)
{
	pthread_mutex_lock(&philo->l_fork);
	action_print(philo, "has taken a fork");
	time_sleep(philo->context->t_sleep);
	pthread_mutex_unlock(&philo->l_fork);
	return (NULL);
}

void *philo_routine(void *philo_ctx)
{
	t_philo *philo;

	philo = ((t_philo *) philo_ctx);
	if (philo->context->n_philos == 1)
		return philo_alone(philo);
	if (philo->id % 2 == 0)
		time_sleep((philo->context->t_die) / 2);
	while (!sim_over(philo->context))
	{
		philo_action_forks(philo);
		philo_action_eat(philo);
		philo_action_sleep(philo);
		philo_action_think(philo);
	}
}
