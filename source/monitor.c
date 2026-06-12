
#include "../headers/philo.h"

int monitor_start(t_context *context)
{
	int i;
	int t_lmeal;

	while (!sim_over(context))
	{
		i = 0;
		while (i < context->n_philos)
		{
			t_lmeal = philo_lastmeal(context->philos[i]);
			if (time_lapsed(t_lmeal) > context->t_die)
				return action_death(context, context->philos[i].id);
			if (monitor_satisfied(context))
				return action_alleat(context);
			i++;
			//usleep(100);
		}
	}
	return (0);
}
