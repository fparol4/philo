int context_clear(t_context *context)
{
	int i;

	i = 0;
	while (i < context->n_philos)
		pthread_mutex_destroy(&context->forks[i++]);
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
