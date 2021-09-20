#include "philo.h"

void *amidie(void *arg)
{
	int nu = *((int *) arg);
	while (1)
	{
		if (getTime() > table.lastTimeEat[nu] + (uint64_t)table.no_philo[1])
		{
			message(nu + 1, "died");
			pthread_mutex_unlock(&table.m_somebodydied);
			break ;
		}
	}
	return (NULL);
}

void *routine(void *arg)
{
	pthread_t philo;
	int nu = *((int *) arg);
	table.lastTimeEat[nu] = getTime();
	pthread_create(&philo, NULL, amidie, (void *)&nu);
	pthread_detach(philo);
	while (5)
	{
		// locking forks mutex
		pthread_mutex_lock(&table.m_forks[nu]);
		message(nu + 1, "take a fork");
		pthread_mutex_lock(&table.m_forks[(nu + 1) % table.no_philo[0]]);
		message(nu + 1, "take a fork");

		// eating
		table.lastTimeEat[nu] = getTime();
		message(nu + 1, "is eating");
		usleep(table.no_philo[2] * 1000);

		// sleeping
		pthread_mutex_unlock(&table.m_forks[nu]);
		pthread_mutex_unlock(&table.m_forks[(nu + 1) % table.no_philo[0]]);
		message(nu + 1, "is sleeping");
		usleep(table.no_philo[3] * 1000);
		message(nu + 1, "is thinking");
	}
}

int main(int argc, char **argv)
{
	pthread_t	philo;
	int			i;

	i = 0;
	(void)argc;

	// taking and converting the arguments
	while (i < 4)
	{
		table.no_philo[i] = atoi(argv[i + 1]);
		i++;
	}
	if (argv[i + 1])
		table.no_philo[i] = atoi(argv[i + 1]);
	else
		table.no_philo[i] = 0;

	// lastTimeEat allocation
	table.lastTimeEat = malloc(sizeof(uint64_t) * table.no_philo[0]);

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
		pthread_create(&philo, NULL, routine, (void *)&i);
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
