#include "../headers/philo.h"

int sim_over(t_context *context)
{
	int tmp;

	pthread_mutex_lock(&context->mtx_action);
	tmp = context->sim_over;
	pthread_mutex_unlock(&context->mtx_action);
	return (tmp);
}

int sim_start(t_context *context)
{
	int	i;

	i = 0;
	while (i < context->n_philos)
	{
		if (pthread_create(&context->philos[i].thread, NULL, philo_routine, &context->philos[i]))
			return (1);
		i++;
	}
	return (0);
}
