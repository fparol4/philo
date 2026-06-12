#include "../headers/philo.h"

int simulation_over(t_context *context)
{
	int	tmp;

	pthread_mutex_lock(&context->mtx_action);
	tmp = context->simulation_over;
	pthread_mutex_unlock(&context->mtx_action);
	return (tmp);
}

static void	simulation_stop(t_context *context)
{
	pthread_mutex_lock(&context->mtx_action);
	context->simulation_over = 1;
	pthread_mutex_unlock(&context->mtx_action);
}

int simulation_join(t_context *context)
{
	int	i;

	i = 0;
	while (i < context->n_philos)
	{
		pthread_join(context->philos[i].thread, NULL);
		i++;
	}
	return (0);
}

int simulation_start(t_context *context)
{
	int	i;

	i = 0;
	while (i < context->n_philos)
	{
		if (pthread_create(&context->philos[i].thread, NULL, philo_routine, &context->philos[i]))
		{
			simulation_stop(context);
			while (--i >= 0)
				pthread_join(context->philos[i].thread, NULL);
			return (1);
		}
		i++;
	}
	return (0);
}
