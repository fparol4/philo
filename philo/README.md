*This project has been created as part of the 42 curriculum by fcardozo*

# Philosophers

## Description

Philosophers is a C implementation of the dining philosophers problem. It
simulates philosophers sitting around a table, alternating between thinking,
eating, and sleeping while sharing forks represented by mutexes.Each philosopher
runs its own thread, and a monitor keeps checking for deaths and completions.

## Instructions

Enter the project directory:

```sh
cd philo
```

Build the project:

```sh
make
```

Run the simulation:

```sh
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

Example:

```sh
./philo 5 800 200 200
```

Remove generated objects:

```sh
make clean
```

Remove generated objects and the executable:

```sh
make fclean
```

Rebuild from scratch:

```sh
make re
```

## Features

- One thread per philosopher.
- Forks protected with pthread mutexes.
- Serialized output for philosopher actions.
- Monitor loop that stops the simulation when a philosopher dies.
- Optional meal limit to stop the simulation when every philosopher has eaten
  enough times.
- Input validation for argument count, numeric values, positive timings, and
  integer overflow.
- Single-philosopher case handling.

## Resources

- `man 3 pthread_create`
- `man 3 pthread_join`
- `man 3 pthread_mutex_init`
- `man 3 pthread_mutex_lock`
- `man 3 pthread_mutex_unlock`
- `man 2 gettimeofday`
- `man 3 usleep`

AI was used as a review, testing edge cases and writind this README.md
