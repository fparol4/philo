#include "../headers/philo.h"

int sim_start(t_context *context)
{
	int	i;

	i = 0;
	while (i < context->n_philos)
	{
		if (pthread_create(&context->philos[i].thread, NULL, sim_routine, &context->philos[i]))
			return (1);
		i++;
	}
	return (0);
}
