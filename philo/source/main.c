#include "../headers/philo.h"

int main(int argc, char **argv)
{
	t_context	context;

	context = (t_context){0};
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments\n");
		printf("Usage: ./philo <num> <die> <eat> <sleep> [must_eat]\n");
		return (1);
	}
	if (util_args(argv, &context))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	if (context_init(&context))
	{
		context_clear(&context);
		return (1);
	}
	if (simulation_start(&context))
	{
		context_clear(&context);
		return (1);
	}
	monitor_start(&context);
	simulation_join(&context);
	context_clear(&context);
	return (0);
}
