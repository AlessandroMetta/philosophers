#include "../incs/philo.h"

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
			table->check = 1;
			break ;
		}
		if (table->check)
			break;
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
	while (!table->check)
	{
		// locking forks mutex
		pthread_mutex_lock(&table->m_forks[nu]);
		message(nu + 1, "take a fork", table);
		pthread_mutex_lock(&table->m_forks[(nu + 1) % table->no_philo[0]]);
		message(nu + 1, "take a fork", table);

		// eating
		table->lastTimeEat[nu] = getTime();
		message(nu + 1, "is eating", table);
		ft_usleep(table->no_philo[2]);

		// sleeping
		pthread_mutex_unlock(&table->m_forks[nu]);
		pthread_mutex_unlock(&table->m_forks[(nu + 1) % table->no_philo[0]]);
		message(nu + 1, "is sleeping", table);
		ft_usleep(table->no_philo[3]);
		message(nu + 1, "is thinking", table);
	}
//	pthread_join(philo, NULL);
	return (NULL);
}

int main(int argc, char **argv)
{
	pthread_t	*philo;
	int			i;
	t_life		table;

	i = 0;

	// check the number of argument
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong argument number\n");
		return (1);
	}

	// taking and converting the arguments
	while (i < 4)
	{
		table.no_philo[i] = atoi(argv[i + 1]);
		i++;
	}
	if (argc == 6)
		table.no_philo[i] = atoi(argv[i + 1]);
	else
		table.no_philo[i] = 0;

	// lastTimeEat allocation
	table.lastTimeEat = (uint64_t *)malloc(sizeof(uint64_t) * table.no_philo[0]);

	philo = malloc(sizeof(pthread_t) * table.no_philo[0]);
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
		pthread_create(&philo[i], NULL, routine, (void *)&table);
		ft_usleep(100);
		i++;
	}
	table.check = 0;
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
	//exit(0);
	i = 0;
	while (i < table.no_philo[0])
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	free(philo);
	return (0);
}
