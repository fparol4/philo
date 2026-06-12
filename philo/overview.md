# Philosophers Project Overview

This document explains the current implementation from an evaluator/defense
point of view: what the program is expected to do, how the code is organized,
which concurrency decisions were made, and which edge cases matter.

## Subject Expectation

The program simulates the dining philosophers problem.

Command format:

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Expected behavior:

- Each philosopher is a thread.
- Each fork is a mutex.
- A philosopher must take two forks before eating.
- A philosopher dies if the time since their last meal reaches `time_to_die`.
- A death message must be printed once, and no normal action should be printed
  after the simulation is marked over.
- If `number_of_times_each_philosopher_must_eat` is provided, the simulation
  stops when every philosopher has eaten at least that many times.
- Arguments must be positive integer values. The optional eat count must also
  be positive when present.

## Project Flow

The main flow is:

1. `main.c` validates the number of arguments.
2. `util_args()` parses and validates argument values.
3. `context_init()` prepares the simulation state:
   - initializes the global action mutex;
   - records the simulation start time;
   - allocates and initializes fork mutexes;
   - allocates and initializes philosopher state.
4. `simulation_start()` creates one thread per philosopher.
5. The main thread runs `monitor_start()`.
6. The monitor stops the simulation on death or on the optional all-eaten
   condition.
7. `simulation_join()` waits for all philosopher threads.
8. `context_clear()` destroys mutexes and frees allocated memory.

The main thread is the monitor. Philosopher threads only run their own routine.

## Core Data Structures

`t_context` owns global simulation state:

- `n_philos`: number of philosophers.
- `t_die`, `t_eat`, `t_sleep`: configured times in milliseconds.
- `max_eat`: optional eat target, or `-1` when not provided.
- `simulation_over`: shared stop flag.
- `t_start`: simulation start timestamp.
- `philos`: philosopher array.
- `forks`: fork mutex array.
- `mtx_action`: mutex used for printing and for the shared stop flag.

`t_philo` owns per-philosopher state:

- `id`: philosopher id, starting at 1.
- `eaten`: number of completed meal starts.
- `last_meal`: timestamp of the last meal start.
- `thread`: philosopher thread.
- `l_fork`, `r_fork`: pointers to fork mutexes.
- `mtx_state`: protects `eaten` and `last_meal`.
- `context`: pointer back to the shared context.

## Timing Model

Time is measured in milliseconds using `gettimeofday()`.

The timestamp type is `long`, not `int`, because epoch milliseconds do not fit
inside a 32-bit signed integer. Input durations remain `int`, because they are
parsed from the command line and checked against `INT_MAX`.

Important functions:

- `time_now()` returns current time in milliseconds.
- `time_lapsed(timestamp)` returns elapsed milliseconds since `timestamp`.
- `time_sleep(philo, ms)` sleeps in a loop and checks `simulation_over()` while
  waiting.

The custom sleep loop is used because `usleep(ms * 1000)` would not react until
the whole sleep finished. Here, a philosopher can stop shortly after the monitor
ends the simulation.

## Death Detection

The monitor repeatedly checks every philosopher:

```c
if (time_lapsed(last_meal) >= time_to_die)
```

The comparison is `>=` because a philosopher is dead as soon as elapsed time
reaches `time_to_die`, not only after it becomes greater.

`last_meal` is protected by the philosopher's `mtx_state`, so the monitor does
not read it while a philosopher is updating it.

When a death is detected:

- `action_death()` locks `mtx_action`;
- checks if the simulation is already over;
- sets `simulation_over = 1`;
- prints the death message;
- unlocks `mtx_action`.

This prevents multiple death messages and prevents normal action messages from
printing after the simulation is over.

## Print Synchronization

All normal action prints go through `print_action()`.

`print_action()` locks `mtx_action`, checks `simulation_over`, prints only if
the simulation is still active, and unlocks the mutex.

This has two purposes:

- output lines do not interleave;
- actions are not printed after death/all-eaten stop is registered.

The same mutex protects `simulation_over`, so reads and writes of the stop flag
are synchronized.

## Philosopher Routine

Each philosopher repeats:

1. take forks;
2. eat;
3. sleep;
4. think.

The loop stops whenever an action returns non-zero, usually because
`simulation_over()` became true.

For one philosopher, a special routine is used:

- lock one fork;
- print `"has taken a fork"`;
- wait until death;
- unlock the fork;
- return.

This is necessary because one philosopher can never acquire two forks.

## Fork Strategy

Each fork is a `pthread_mutex_t`.

Fork assignment alternates by philosopher parity:

- odd philosophers take one fork order;
- even philosophers take the opposite order.

This reduces the chance of every philosopher locking one fork and waiting
forever for the other.

Even philosophers also wait `time_to_eat / 2` before starting. This staggers the
initial race for forks.

For odd numbers of philosophers, after printing `"is thinking"`, philosophers
wait `time_to_eat / 2`. This small pacing avoids repeated unfair fork races in
common evaluator cases such as:

```sh
./philo 5 800 200 200
./philo 5 610 200 200
```

## Eating State Update

After both forks are acquired, `philo_action_eat()` first checks
`simulation_over()`. If the simulation is already over, it unlocks both forks
and exits.

If the simulation is still active, it updates:

- `last_meal = time_now()`;
- `eaten++`.

This update happens before printing `"is eating"`.

Reason: logically, once both forks are held and the philosopher starts eating,
the monitor should see the new meal timestamp immediately. If printing happened
first, the monitor could observe an old `last_meal` during a scheduling delay
and incorrectly kill a philosopher who has already reached the eating state.

The update is protected by `mtx_state`.

## Can The Simulation End Between Fork Locks?

Yes.

There is a real window between:

```c
pthread_mutex_lock(philo->l_fork);
```

and:

```c
pthread_mutex_lock(philo->r_fork);
```

The monitor can mark the simulation over during that time.

The code handles the important output/correctness consequence: after the second
fork is acquired, `print_action()` checks `simulation_over()` before printing.
If the simulation is already over, the function unlocks any held forks and
returns.

One nuance: `pthread_mutex_lock()` is blocking. If a philosopher is waiting for
the second fork when the simulation ends, that thread may not wake immediately.
It will continue once the fork holder releases the mutex. In this implementation
that is acceptable because eating/sleeping actions use `time_sleep()`, which
checks `simulation_over()`, so threads eventually release forks and join.

A more complex implementation could use `pthread_mutex_trylock()` in a loop,
but it is not necessary for the expected mandatory evaluation behavior.

## All-Eaten Condition

If the optional argument is absent, `max_eat` is `-1`, and the monitor ignores
the all-eaten condition.

If it is present, `monitor_satisfied()` checks every philosopher's `eaten`
counter under `mtx_state`.

When all philosophers have eaten at least `max_eat` times, `action_alleat()`
sets `simulation_over = 1`. No special success message is printed.

Silent termination on all-eaten is expected.

## Argument Edge Cases

Rejected:

- wrong argument count;
- non-digit values;
- empty values;
- values greater than `INT_MAX`;
- zero philosophers;
- zero or negative time values;
- optional eat count of zero.

Accepted:

- positive numeric values only;
- optional eat count omitted, which means run until death or external stop.

The parser does not accept signs such as `+1` or `-1`, because only digit
characters are accepted.

## Runtime Edge Cases

### One Philosopher

Expected behavior:

```sh
./philo 1 200 100 100
```

The philosopher takes one fork and dies at about `200ms`.

They cannot eat because there is only one fork.

### Immediate Starvation

Example:

```sh
./philo 2 100 200 200
```

One philosopher starts eating, but `time_to_eat` is longer than `time_to_die`.
The monitor should print one death at about `100ms`.

### Enough Time To Live

Example:

```sh
./philo 4 410 200 200
```

No philosopher should die during a normal short timeout test, because the timing
allows alternating meals.

### Tight Timing

Example:

```sh
./philo 4 310 200 100
```

A philosopher is expected to die around `310ms`, because they cannot cycle back
to eating before `time_to_die`.

### Must-Eat Termination

Example:

```sh
./philo 5 800 200 200 7
```

The program should stop once every philosopher has started eating at least
seven times. It should not print a success message.

## Expected Defense Answers

### Why use one mutex for printing and `simulation_over`?

Because both operations are related to global simulation state. When printing,
the program must know whether the simulation is already over. Protecting both
with `mtx_action` prevents action messages from appearing after a death message.

### Why does each philosopher also have `mtx_state`?

`last_meal` and `eaten` are independent per-philosopher values read by the
monitor and written by philosopher threads. A per-philosopher mutex keeps those
reads/writes synchronized without blocking unrelated philosophers.

### Why update `last_meal` before printing `"is eating"`?

The real state transition is that eating starts after both forks are acquired.
Updating `last_meal` first prevents the monitor from seeing an old meal time
while the philosopher is already logically eating.

### Why use `long` for timestamps?

Milliseconds since epoch are too large for `int`. `long` avoids overflow in
`time_now()`, `t_start`, and `last_meal`.

### Why is the death comparison `>=`?

At exactly `time_to_die` milliseconds since the last meal, the philosopher is
dead. Waiting for `>` would allow them to live one millisecond longer than the
definition.

### Why not use plain `usleep(time * 1000)`?

A plain sleep cannot stop early. The custom `time_sleep()` wakes frequently and
checks whether the simulation has ended.

### Can there still be small timing differences?

Yes. Thread scheduling and OS timing can introduce small variations. The code
uses millisecond timestamps and frequent monitor checks to keep behavior close
to the required timing.

### Can a philosopher block on the second fork after death?

Temporarily, yes. A blocking mutex wait cannot be interrupted by
`simulation_over`. The implementation still releases forks and joins because
other running philosophers detect the stop condition during `time_sleep()`.

## Evaluation Test Plan

Run tests from the project root after rebuilding:

```sh
make re
```

If `timeout` is used, exit status `124` means the program was still running
when `timeout` stopped it. That is expected for no-death tests without
`must_eat`.

### Build And Norm

```sh
make re
norminette .
```

Expected:

- `make re` succeeds with `-Wall -Wextra -Werror`;
- `norminette .` reports `OK` for every source/header file.

### Invalid Argument Tests

```sh
./philo
./philo 0 800 200 200
./philo 5 0 200 200
./philo 5 800 0 200
./philo 5 800 200 0
./philo 5 800 200 200 0
./philo 5 abc 200 200
./philo 5 800 200 200 extra extra
```

Expected:

- program prints an error;
- program exits with status `1`;
- no crash, no hang.

These tests verify argument count, digit-only parsing, and rejection of zero
values.

### Single Philosopher

```sh
./philo 1 800 200 200
```

Expected:

- philosopher takes one fork;
- philosopher never eats;
- one death is printed around `800ms`.

This is an official evaluator case.

### Two Philosopher Death Timing

```sh
./philo 2 60 200 200
./philo 2 100 200 200
./philo 2 200 300 100
```

Expected:

- exactly one death;
- death is not delayed by more than about `10ms`;
- no action lines appear after the death line.

The sheet specifically asks to test two philosophers with different timings.

### No-Death Official Cases

```sh
timeout 3 ./philo 5 800 200 200
timeout 3 ./philo 4 410 200 200
```

Expected:

- no `died` line;
- command exits with status `124` because `timeout` stops it;
- output remains ordered and readable.

These are official evaluator cases.

### Expected Death Official Case

```sh
./philo 4 310 200 100
```

Expected:

- exactly one philosopher dies;
- death happens around `310ms`;
- no normal action is printed after death.

This is an official evaluator case.

### Must-Eat Official Case

```sh
./philo 5 800 200 200 7
```

Expected:

- no philosopher dies;
- simulation stops by itself;
- no success message is required;
- every philosopher has eaten at least seven times.

This is an official evaluator case.

### Extra Must-Eat Regression Cases

```sh
./philo 5 610 200 200 7
./philo 3 610 200 200 7
./philo 7 800 200 200 3
```

Expected:

- no death;
- natural termination;
- verifies odd philosopher counts and the thinking delay.

### Large But Allowed Count

```sh
timeout 3 ./philo 200 800 200 200
```

Expected:

- no death during the short timeout;
- output remains synchronized;
- no crash.

The sheet says not to test with more than 200 philosophers, so 200 is the upper
boundary worth checking.

### Leak Check

```sh
valgrind --leak-check=full --show-leak-kinds=all ./philo 5 800 200 200 2
```

Expected:

- all heap blocks are freed;
- no Valgrind errors.

Use a `must_eat` case so the program exits naturally.

### Race Check

```sh
valgrind --tool=helgrind ./philo 5 800 200 200 2
```

Expected:

- Helgrind reports no data race errors.

The evaluation sheet explicitly allows tools such as Helgrind or DRD to verify
the absence of data races.

## Verified Evaluation-Style Cases

The current behavior was checked with:

```sh
make re
norminette .
./philo 1 800 200 200
./philo 2 60 200 200
./philo 2 100 200 200
./philo 2 200 300 100
./philo 4 310 200 100
timeout 3 ./philo 4 410 200 200
timeout 3 ./philo 5 800 200 200
timeout 3 ./philo 5 610 200 200
timeout 3 ./philo 200 800 200 200
./philo 5 800 200 200 7
./philo 5 610 200 200 7
./philo 3 610 200 200 7
./philo 7 800 200 200 3
valgrind --leak-check=full --show-leak-kinds=all ./philo 5 800 200 200 2
valgrind --tool=helgrind ./philo 5 800 200 200 2
```

Expected and observed results:

- build succeeds with `-Wall -Wextra -Werror`;
- Norm passes;
- death cases print exactly one `died` line;
- no-death timeout cases keep running until killed by `timeout`;
- must-eat cases terminate without death;
- invalid arguments print an error and return status `1`;
- Memcheck reports no leaks;
- Helgrind reports no data race errors.
