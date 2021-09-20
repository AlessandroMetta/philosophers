#include "philo.h"

void *amidie(void *arg)
{
	t_life *table=((t_life *) arg);
	int nu = table->nu;
	while (1)
	{
		if (getTime() > table->lastTimeEat[nu] + (uint64_t)table->no_philo[1])
		{
			message(nu + 1, "died", table);
			pthread_mutex_unlock(&table->m_somebodydied);
			break ;
		}
	}
	return (NULL);
}

void *routine(void *arg)
{
	t_life *table=((t_life *) arg);
	pthread_t philo;
	int nu = table->nu;
	table->lastTimeEat[nu] = getTime();
	pthread_create(&philo, NULL, amidie, arg);
	pthread_detach(philo);
	while (5)
	{
		// locking forks mutex
		pthread_mutex_lock(&table->m_forks[nu]);
		message(nu + 1, "take a fork", table);
		pthread_mutex_lock(&table->m_forks[(nu + 1) % table->no_philo[0]]);
		message(nu + 1, "take a fork", table);

		// eating
		table->lastTimeEat[nu] = getTime();
		message(nu + 1, "is eating", table);
		usleep(table->no_philo[2] * 1000);

		// sleeping
		pthread_mutex_unlock(&table->m_forks[nu]);
		pthread_mutex_unlock(&table->m_forks[(nu + 1) % table->no_philo[0]]);
		message(nu + 1, "is sleeping", table);
		usleep(table->no_philo[3] * 1000);
		message(nu + 1, "is thinking", table);
	}
}

int main(int argc, char **argv)
{
	pthread_t	philo;
	int			i;
	t_life		table;

	i = 0;

	// check the number of argument
	if (argc < 6 || argc > 7)
	{
		printf("Error: wrong argument number\n");
		return (1);
	}

	// taking and converting the arguments
	while (i < 5)
	{
		table.no_philo[i] = atoi(argv[i + 1]);
		i++;
	}
	if (argc == 7)
		table.no_philo[i] = atoi(argv[i + 1]);
	else
		table.no_philo[i] = 0;

	// lastTimeEat allocation
	table.lastTimeEat = (uint64_t *)malloc(sizeof(uint64_t) * table.no_philo[0]);

	// allocate and initializate the forks mutex (same number of the philo)
	table.m_forks = malloc(sizeof(pthread_mutex_t) * table.no_philo[0]);
	if (!table.m_forks)
		return (1);

	i = 0;
	while (i < table.no_philo[0])
	{
		pthread_mutex_init(&table.m_forks[i], NULL);
		i++;
	}

	// initialization of write mutex
	pthread_mutex_init(&table.m_write, NULL);

	// initialization and lock the finish program mutex
	pthread_mutex_init(&table.m_somebodydied, NULL);
	pthread_mutex_lock(&table.m_somebodydied);

	// start the cronometer
	table.startTime = getTime();

	// creation of philosophers threads
	i = 0;
	while (i < table.no_philo[0])
	{
		table.nu = i;
		pthread_create(&philo, NULL, routine, (void *)&table);
		pthread_detach(philo);
		usleep(100);
		i++;
	}

	// Wait until a philosopher die
	pthread_mutex_lock(&table.m_somebodydied);

	// Destroing and free everything
	pthread_mutex_destroy(&table.m_somebodydied);
	pthread_mutex_destroy(&table.m_write);

	i = 0;
	while (i < table.no_philo[0])
	{
		pthread_mutex_destroy(&table.m_forks[i]);
		i++;
	}

	return (0);
}
