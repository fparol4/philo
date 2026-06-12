#include "../headers/philo.h"

int main(int argc, char **argv)
{
	t_context *context;

	if ((argc < 5 || argc > 6) && util_args(argv, context))
	{
		printf("Error: Invalid number of arguments\n");
		printf("Usage: ./philo <num> <die> <eat> <sleep> [must_eat]\n");
		context_clear(context);
	}
	if (context_init(context))
		return (context_clear(context));
	if (sim_start(context))
		return (context_clear(context));
	monitor_start(context);
	context_clear(context);
	return (0);
}
