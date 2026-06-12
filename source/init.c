#include "../headers/philo.h"

int init_forks(t_context *context)
{
	int i;

	i = 0;
	context->forks = malloc(context->n_philos * sizeof(pthread_mutex_t));
	if (!context->forks)
		return (-1);
	while (i < context->n_philos)
		pthread_mutex_init(&context->forks[i++], NULL);
	return (0);
}

int init_philos(t_context *context)
{
	int i;

	i = 0;
	context->philos = malloc(context->n_philos * sizeof(t_philo));
	if (!context->philos)
		return (-1);
	while (i < context->n_philos)
	{
		context->philos[i].id = i + 1;
		context->philos[i].l_fork = context->forks[i];
		context->philos[i].r_fork = context->forks[(i + 1) % context->n_philos];
		context->philos[i].last_meal = context->t_start;
	}
	return (0);
}

int init_table(t_context *context)
{
	pthread_mutex_init(&context->mtx_action, NULL);
	context->t_start = time_now();
	context->sim_over = 0;
	return (1);
}
