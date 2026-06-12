#include "../headers/philo.h"

int context_clear(t_context *context)
{
	int	i;

	if (!context)
		return (1);
	i = 0;
	while (i < context->philos_ready)
		pthread_mutex_destroy(&context->philos[i++].mtx_state);
	i = 0;
	while (i < context->forks_ready)
		pthread_mutex_destroy(&context->forks[i++]);
	if (context->action_ready)
		pthread_mutex_destroy(&context->mtx_action);
	free(context->forks);
	free(context->philos);
	return (0);
}

int context_init(t_context *context)
{
	if (init_table(context))
		return (1);
	if (init_forks(context))
		return (1);
	if (init_philos(context))
		return (1);
	return (0);
}
