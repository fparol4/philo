#include "../headers/philo.h"

int print_action(t_philo *philo, char *msg)
{
	int	timestamp;

	pthread_mutex_lock(&philo->context->mtx_action);
	if (philo->context->simulation_over)
	{
		pthread_mutex_unlock(&philo->context->mtx_action);
		return (1);
	}
	timestamp = time_lapsed(philo->context->t_start);
	printf("%d %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->context->mtx_action);
	return (0);
}

int action_death(t_context *context, int philo_id)
{
	int	timestamp;

	pthread_mutex_lock(&context->mtx_action);
	if (context->simulation_over)
	{
		pthread_mutex_unlock(&context->mtx_action);
		return (1);
	}
	context->simulation_over = 1;
	timestamp = time_lapsed(context->t_start);
	printf("%d %d died\n", timestamp, philo_id);
	pthread_mutex_unlock(&context->mtx_action);
	return (1);
}

int action_alleat(t_context *context)
{
	pthread_mutex_lock(&context->mtx_action);
	context->simulation_over = 1;
	pthread_mutex_unlock(&context->mtx_action);
	return (1);
}
