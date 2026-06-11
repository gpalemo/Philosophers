*This project has been created as part of the 42 curriculum by cmauley.*

# Philosophers

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c"
       alt="C language">
  <img src="https://img.shields.io/badge/School-42-000000?style=for-the-badge&logo=42"
       alt="42 School">
  <img src="https://img.shields.io/badge/Threads-pthreads-8A2BE2?style=for-the-badge"
       alt="pthreads">
  <img src="https://img.shields.io/badge/Status-Completed-00C853?style=for-the-badge"
       alt="Completed project">
</p>

<p align="center">
  <img src="image/dining-philosophers.webp"
       alt="Dining philosophers problem diagram"
       width="650">
</p>

<p align="center">
  Image source:
  <a href="https://www.naukri.com/code360/library/dining-philosopher-problem-using-semaphores-2383">
    Code360 - Dining Philosopher Problem Using Semaphores
  </a>
</p>

| Project status | Details |
|---|---|
| Completed part | Mandatory |
| Memory leaks | Tested with Valgrind |
| Data races | Tested with Helgrind |

## Table of Contents

- [Description](#description)
- [How It Works](#how-it-works)
- [Understanding Threads](#understanding-threads)
- [Understanding Mutexes](#understanding-mutexes)
- [Understanding Data Races](#understanding-data-races)
- [Understanding the Monitor](#understanding-the-monitor)
- [Common Mistakes and Important Points](#common-mistakes-and-important-points)
- [Instructions](#instructions)
- [Arguments](#arguments)
- [Examples](#examples)
- [Useful Tests](#useful-tests)
- [Resources](#resources)
- [Use of AI](#use-of-ai)

## Description

Philosophers is a 42 School project designed to introduce threads, mutexes,
and the challenges of concurrent programming.

The program simulates several philosophers sitting around a table. Each
philosopher alternates between three actions: eating, sleeping, and thinking.
To eat, a philosopher must obtain the two forks placed next to them.

The main goal is to avoid deadlocks, data races, and starvation while quickly
detecting the possible death of a philosopher.

## How It Works

- Each philosopher is represented by a thread.
- Each fork is protected by a mutex.
- A mutex protects shared data.
- A mutex prevents displayed messages from overlapping.
- Even- and odd-numbered philosophers do not pick up their forks in the same
  order, which prevents deadlocks.
- A monitor checks for philosopher deaths and the optional meal quota.
- The simulation stops when a philosopher dies or when every philosopher has
  reached the required meal quota.

## Understanding Threads

### What is a thread?

A traditional program usually executes its instructions one after another. A
**thread** is one execution path inside a program. Multiple threads therefore
allow several tasks to run at the same time.

In this project, each philosopher must live independently from the others:

```text
Philosopher 1 thread: eat -> sleep -> think -> ...
Philosopher 2 thread: eat -> sleep -> think -> ...
Philosopher 3 thread: eat -> sleep -> think -> ...
```

All these threads belong to the same program and share its memory. They can
therefore access the same table, the same forks, and the same simulation state.

> **Simple analogy:** the program is a kitchen, and the threads are several
> cooks working in that kitchen at the same time. They share the equipment,
> but each cook follows their own sequence of actions.

### How are threads created here?

The [`create_threads`](srcs/simulation.c#L34) function calls `pthread_create`
once for each philosopher:

```c
pthread_create(&table->philos[i].thread_id, NULL,
	philo_routine, &table->philos[i]);
```

The four important arguments are:

| Argument | Purpose |
|---|---|
| `&thread_id` | receives the identifier of the new thread |
| `NULL` | uses the default thread attributes |
| `philo_routine` | function executed by the thread |
| `&table->philos[i]` | philosopher passed to that function |

Each thread therefore starts inside
[`philo_routine`](srcs/routine.c#L41). The routine retrieves its philosopher,
possibly waits for a short time, and then repeats:

```text
take forks -> eat -> put down forks -> sleep -> think
```

### Why use `pthread_join`?

The main thread starts the philosophers, but it must also wait for them to
finish before destroying mutexes and freeing memory.

[`join_threads`](srcs/simulation.c#L54) calls `pthread_join` for every created
thread. Without this wait, `main` could clean the table while philosophers are
still using it.

```text
create threads
      |
      v
   monitor
      |
      v
stop simulation
      |
      v
pthread_join every thread
      |
      v
clean allocated memory
```

## Understanding Mutexes

### What is a mutex?

A **mutex** is a lock that protects a shared resource. Only one thread can own
that lock at a time.

```text
Thread A locks the mutex
Thread B wants the same mutex and must wait
Thread A unlocks the mutex
Thread B can finally continue
```

In this project, mutex calls are grouped in
[`mutex.c`](srcs/utils/mutex.c) so errors from `pthread_mutex_init`,
`pthread_mutex_lock`, `pthread_mutex_unlock`, and `pthread_mutex_destroy` can
be checked easily.

### Mutexes used in this project

| Mutex | What it protects | Why |
|---|---|---|
| Each fork's mutex | one specific fork | prevents two neighbors from taking the same fork |
| `data_mutex` | `last_meal_time`, `meals_counter`, `end_simulation` | prevents concurrent reads and writes |
| `print_mutex` | status output | prevents messages from overlapping |

Fork mutexes are initialized in [`init_forks`](srcs/init.c#L32). When a
philosopher eats, [`take_forks`](srcs/actions.c#L88) locks the forks, and
[`drop_forks`](srcs/actions.c#L18) unlocks them afterward.

### Why change the fork pickup order?

If every philosopher first picks up their left fork, each philosopher may end
up holding one fork while waiting forever for the second one. This is called a
**deadlock**.

```text
Philo 1 owns left and waits for right
Philo 2 owns left and waits for right
Philo 3 owns left and waits for right
=> nobody can make progress
```

[`choose_forks`](srcs/actions.c#L71) avoids this situation by giving even- and
odd-numbered philosophers a different order:

- even-numbered philosophers take the right fork first;
- odd-numbered philosophers take the left fork first.

The small initial delay for even-numbered philosophers in
[`philo_routine`](srcs/routine.c#L52) also reduces conflicts when the
simulation starts.

> **Essential rule:** every successful `lock` must have a matching `unlock`,
> including when an error occurs or the simulation stops.

## Understanding Data Races

### What is a data race?

A **data race** happens when multiple threads access the same data at the same
time, at least one thread modifies it, and no mutex protects these accesses.

Dangerous example:

```c
philo->meals_counter++;
```

While this value is being modified, the monitor could read `meals_counter`.
The result becomes unpredictable: the monitor may read an old or inconsistent
value, or the program may trigger undefined behavior.

### How are data races prevented here?

Shared data that may change during the simulation is always read and modified
while holding `data_mutex`.

In [`eat`](srcs/actions.c#L33), the philosopher protects the update of their
last meal time:

```c
safe_mutex_lock(&philo->table->data_mutex);
philo->last_meal_time = get_time();
safe_mutex_unlock(&philo->table->data_mutex);
```

In [`is_dead`](srcs/monitor.c#L18), the monitor uses the **same mutex** before
reading that value. Protecting only the write or only the read is not enough:
every related access must follow the same rule.

The same principle is used for:

- `meals_counter`, modified in `eat` and read by the monitor;
- `end_simulation`, modified when stopping and read by every thread;
- output, protected by `print_mutex` in
  [`print_status`](srcs/routine.c#L18) and
  [`print_death`](srcs/utils/utils.c#L27).

### How can data races be detected?

Helgrind observes memory accesses performed by threads:

```bash
valgrind --tool=helgrind ./philo 5 800 200 200 3
```

The expected result is:

```text
ERROR SUMMARY: 0 errors
```

Helgrind greatly slows the program down. Timings observed under Helgrind should
therefore not be used to judge the accuracy of death detection.

## Understanding the Monitor

The **monitor** runs in the main thread while the philosophers execute their
routines. Its role is to observe the simulation without acting as a
philosopher.

The [`monitor`](srcs/monitor.c#L85) function repeatedly performs two checks:

1. Check whether every philosopher has reached the optional meal quota.
2. Check whether a philosopher has exceeded `time_to_die`.

```text
                 +-------------------------+
                 | have all quotas been met? |
                 +------------+------------+
                              |
                    yes ------+------ no
                     |                 |
              stop simulation         v
                                  check deaths
                                        |
                         death ----------+---------- no death
                           |                         |
                      print "died"             short pause
                           |                         |
                    stop simulation <---------------+
```

### Detecting a death

[`is_dead`](srcs/monitor.c#L18) compares the current time with the beginning of
the philosopher's last meal:

```text
current time - last meal >= time_to_die
```

The `last_meal_time` value is updated when a philosopher **starts** eating, as
required by the subject.

When a death is detected, [`print_death`](srcs/utils/utils.c#L27) first locks
`print_mutex`, sets `end_simulation` to `1`, and then prints the death. Because
`print_status` checks the same state while holding the same output lock, no
normal status can be printed after `died`.

### Stopping after the meal quota

[`all_full`](srcs/monitor.c#L51) checks that each `meals_counter` has reached
the requested quota. The simulation does not stop as soon as one philosopher
reaches the quota: **all** philosophers must reach it.

The counter is incremented after a complete meal finishes in
[`eat`](srcs/actions.c#L33).

## Common Mistakes and Important Points

### Forgetting to unlock a mutex

A `return` placed between a `lock` and its matching `unlock` may leave the
mutex locked forever. Other threads will then remain blocked.

For every error branch, ask:

```text
Which mutexes does this thread currently own?
Must they be released before returning?
```

### Printing after a death

Reading `end_simulation`, releasing the mutex, and then printing without a
shared protection mechanism gives the monitor enough time to announce a death
between these operations.

In this project, `print_status` keeps `print_mutex` locked while checking the
state and printing. `print_death` uses the same mutex.

### Confusing `pthread_join` with stopping threads

`pthread_join` does not ask a thread to stop. It only waits for that thread to
finish. `end_simulation` must first be updated so the routines can exit, and
only then can the threads be joined.

### Handling a partial `pthread_create` failure

Creating the fourth thread may fail while the first three are already running.
The program must therefore:

1. count how many threads were successfully created;
2. request the simulation to stop;
3. wait only for those threads with `pthread_join`;
4. print the error and then clean up.

This logic is implemented in
[`create_threads`](srcs/simulation.c#L34) and
[`dinner_start`](srcs/simulation.c#L76).

### Using `usleep` directly for long actions

A long `usleep` prevents a philosopher from quickly noticing that the
simulation has stopped. [`safe_sleep`](srcs/utils/time.c#L18) therefore
performs short pauses and regularly checks `end_simulation`.

### The single philosopher case

With one philosopher, there is only one fork. The philosopher picks it up but
can never obtain a second fork to eat. They therefore die after `time_to_die`.
This behavior is handled in [`philo_routine`](srcs/routine.c#L47) and
[`take_forks`](srcs/actions.c#L98).

### Cleaning up after incomplete initialization

An allocation or mutex initialization may fail in the middle of `data_init`.
Initialization counters allow [`clean`](srcs/utils/utils.c#L61) to destroy
only mutexes that were successfully initialized.

### Running tests multiple times

Thread-related problems are often intermittent: a program may work nine times
and then fail on the tenth run, depending on the execution order chosen by the
operating system. Important scenarios should therefore be repeated.

## Instructions

### Compilation

```bash
make
```

The Makefile also provides the following rules:

```bash
make clean
make fclean
make re
```

### Execution

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep \
[number_of_times_each_philosopher_must_eat]
```

All durations are expressed in milliseconds.

## Arguments

- `number_of_philosophers`: number of philosophers and forks.
- `time_to_die`: maximum time without starting a meal before dying.
- `time_to_eat`: amount of time a philosopher spends eating.
- `time_to_sleep`: amount of time a philosopher spends sleeping.
- `number_of_times_each_philosopher_must_eat`: optional meal quota.

All arguments must be strictly positive integers.

## Examples

Simulation without a meal quota:

```bash
./philo 5 800 200 200
```

Simulation that stops after every philosopher has eaten at least three times:

```bash
./philo 5 800 200 200 3
```

Single philosopher case: the philosopher picks up their only fork, cannot eat,
and dies after `time_to_die`.

```bash
./philo 1 200 100 100
```

Messages always follow one of the required formats:

```text
timestamp philosopher_id has taken a fork
timestamp philosopher_id is eating
timestamp philosopher_id is sleeping
timestamp philosopher_id is thinking
timestamp philosopher_id died
```

## Useful Tests

Check for memory leaks:

```bash
valgrind --leak-check=full ./philo 5 800 200 200 3
```

Check for data races:

```bash
valgrind --tool=helgrind ./philo 5 800 200 200 3
```

Trigger a death and check that no message follows `died`:

```bash
./philo 4 310 200 100
```

Test a simulation with many threads:

```bash
./philo 200 800 200 200 2
```

## Resources

- [POSIX Threads Programming](https://hpc-tutorials.llnl.gov/posix/)
- [POSIX Threads in OS](https://www.geeksforgeeks.org/operating-systems/posix-threads-in-os/)
- [pthreads documentation](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [pthread_mutex_lock documentation](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [pthread_create documentation](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
- [The Dining Philosophers by Oceano](https://medium.com/@jalal92/the-dining-philosophers-7157cc05315)
- [Helpful video](https://www.youtube.com/watch?v=mvZKu0DfFLQ)
- Official Philosophers subject provided by 42 School.

## Use of AI

AI was used as a learning tool during development. It was mainly used to:

- progressively explain threads, mutexes, deadlocks, and data races;
- suggest test scenarios and analyze their results;
- verify edge cases, memory leaks, and data races;
- help organize comments and documentation, including translating this README
  into English.

The code was written by hand and studied step by step so that every decision
remains understood and can be explained during the evaluation.
