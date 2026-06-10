#include "../headers/philo.h"

int	shared_digit(char *v)
{
	int i;

	i = 0;
	while (v[i])
	{
		if (!(v[i] >= '0' && v[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	shared_atoi(char *v)
{
	int	i;
	int	signal;
	int	result;

	i = 0;
	signal = 1;
	result = 0;
	if (!shared_digit(v))
		return (-1);
	while ((v[i] >= 9 && v[i] <= 13) || v[i] == 32)
		i++;
	if (v[i] == '+' || v[i] == '-')
	{
		if (v[i] == '-')
			signal = -1;
		i++;
	}
	while (v[i] >= '0' && v[i] <= '9')
	{
		result = result * 10 + (v[i] - '0');
		i++;
	}
	return ((int)result * signal);
}

int shared_check_props(t_context *context)
{
	if (context->n_philos < 1)
		return (0);
	if (context->t_die < 1)
		return (0);
	if (context->t_eat < 1)
		return (0);
	if (context->t_sleep < 1)
		return (0);
	if (context->m_eat != -1 && context->m_eat < 1)
		return (0);
	return (1);
}

int shared_args(char **argv, t_context *context)
{
	int i;

	i = 1;
	while (argv[i])
	{
		if (!shared_digit(argv[i]))
			return (-1);
		i++;
	}
	context->n_philos = shared_atoi(argv[1]);
	context->t_die = shared_atoi(argv[2]);
	context->t_eat = shared_atoi(argv[3]);
	context->t_sleep = shared_atoi(argv[4]);
	if (argv[5])
		context->m_eat = shared_atoi(argv[5]);
	else
		context->m_eat = -1;
	if (shared_check_props(context))
		return (0);
	return (1);
}
