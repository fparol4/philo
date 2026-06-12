#include "../headers/philo.h"

int init_forks(t_context *context)
{
	int	i;

	i = 0;
	context->forks = malloc(context->n_philos * sizeof(pthread_mutex_t));
	if (!context->forks)
		return (1);
	while (i < context->n_philos)
	{
		if (pthread_mutex_init(&context->forks[i], NULL) != 0)
			return (1);
		context->forks_ready++;
		i++;
	}
	return (0);
}

int init_philos(t_context *context)
{
	int	i;

	i = 0;
	context->philos = calloc(context->n_philos, sizeof(t_philo));
	if (!context->philos)
		return (1);
	while (i < context->n_philos)
	{
		context->philos[i].id = i + 1;
		context->philos[i].last_meal = context->t_start;
		context->philos[i].context = context;
		if ((i + 1) % 2 == 0)
		{
			context->philos[i].l_fork = &context->forks[(i + 1) % context->n_philos];
			context->philos[i].r_fork = &context->forks[i];
		}
		else
		{
			context->philos[i].l_fork = &context->forks[i];
			context->philos[i].r_fork = &context->forks[(i + 1) % context->n_philos];
		}
		if (pthread_mutex_init(&context->philos[i].mtx_state, NULL) != 0)
			return (1);
		context->philos_ready++;
		i++;
	}
	return (0);
}

int init_table(t_context *context)
{
	if (pthread_mutex_init(&context->mtx_action, NULL) != 0)
		return (1);
	context->action_ready = 1;
	context->t_start = time_now();
	return (0);
}
