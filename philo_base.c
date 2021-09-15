#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

pthread_mutex_t	m_write;
pthread_mutex_t	*m_forks;
pthread_mutex_t	*m_someboy_died;
int				no_philo;//number of philosophers and forks

uint64_t getTime()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void message(int nu, char *msg)
{
	pthread_mutex_lock(&m_write);
	printf("[%llu]\tphilo %d %s\n", getTime(), nu, msg);
	pthread_mutex_unlock(&m_write);
}

void *routine(void *arg)
{
	int i = 0;
	int nu = *((int *) arg);
	while (i < 5)
	{
		i++;
		pthread_mutex_lock(&m_forks[nu]);
		message(nu, "take a fork");
		pthread_mutex_lock(&m_forks[(nu + i) % no_philo]);
		message(nu, "take a fork");
		message(nu, "is eating...");
		usleep(100);
		message(nu, "has finished eating");
		pthread_mutex_unlock(&m_forks[nu]);
		pthread_mutex_unlock(&m_forks[(nu + i) % no_philo]);
		message(nu, "is sleeping...");
		usleep(100);
		message(nu, "is thinking");
		usleep(100);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	pthread_t	*philo;
	int			i;

	i = 0;
	(void)argc;

	// taking and converting the arguments
	no_philo = atoi(argv[1]);

	// mallocing the thread and the mutex
	philo = malloc(sizeof(pthread_t) * no_philo);
	m_forks = malloc(sizeof(pthread_mutex_t) * no_philo);
	if (!philo)
		return (1);
	if (!m_forks)
		return (1);

	// initialization of forks(mutex)
	i = 0;
	while (i < no_philo)
	{
		pthread_mutex_init(&m_forks[i], NULL);
		i++;
	}

	// initialization of write mutex
	pthread_mutex_init(&m_write, NULL);
	// pthread_mutex_init(&m_somebody_died, NULL);

	// creation of philosophers threads
	i = 1;
	while (i <= no_philo)
	{
		pthread_create(&philo[i], NULL, routine, (void *)&i);
		pthread_detach(philo[i]);
		usleep(100);
		i++;
	}

	// closing the program
	
	sleep(5);
	i = 0;
	while (i < no_philo)
	{
		pthread_mutex_destroy(&m_forks[i]);
		i++;
	}
	pthread_mutex_destroy(&m_write);
	free(philo);
	return (0);
}
