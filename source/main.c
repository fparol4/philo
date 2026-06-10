#include "../headers/philo.h"

void clear_context(t_context *context)
{

}

int main(int argc, char **argv)
{
	t_context context;

	if ((argc < 5 || argc > 6) && util_args(argv, &context))
	{
		printf("Error: Invalid number of arguments\n");
		printf("Usage: ./philo <num> <die> <eat> <sleep> [must_eat]\n");
		clear_context(&context);
	}

	return 0;
}
