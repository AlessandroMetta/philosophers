#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

pthread_mutex_t	m_write;
pthread_mutex_t	*m_forks;
pthread_mutex_t	m_somebodydied;
int				no_philo[5];			//number of philosophers and forks
uint64_t		startTime;
uint64_t		*lastTimeEat;

uint64_t getTime()
{
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void message(int nu, char *msg)
{
	pthread_mutex_lock(&m_write);
	printf("[%llu]\tPhilosopher %d %s\n", getTime() - startTime, nu, msg);
	pthread_mutex_unlock(&m_write);
}

void *amidie(void *arg)
{
	int i = 0;
	int nu = *((int *) arg);
	while (1)
	{
		if (getTime() > lastTimeEat[nu] + (uint64_t)no_philo[1])
		{
			message(nu + 1, "died");
			pthread_mutex_unlock(&m_somebodydied);
			i = 0;
			while (i < no_philo[0])
			{
				pthread_mutex_destroy(&m_forks[i]);
				i++;
			}
			pthread_mutex_destroy(&m_write);
			pthread_mutex_destroy(&m_somebodydied);
			exit(1);
		}
	}
}

void *routine(void *arg)
{
	pthread_t philo;
	int nu = *((int *) arg);
	lastTimeEat[nu] = getTime();
	pthread_create(&philo, NULL, amidie, (void *)&nu);
	pthread_detach(philo);
	while (5)
	{
		// locking forks mutex
		pthread_mutex_lock(&m_forks[nu]);
		message(nu + 1, "take a fork");
		pthread_mutex_lock(&m_forks[(nu + 1) % no_philo[0]]);
		message(nu + 1, "take a fork");

		// eating
		lastTimeEat[nu] = getTime();
		message(nu + 1, "is eating");
		usleep(no_philo[2] * 1000);

		// sleeping
		pthread_mutex_unlock(&m_forks[nu]);
		pthread_mutex_unlock(&m_forks[(nu + 1) % no_philo[0]]);
		message(nu + 1, "is sleeping");
		usleep(no_philo[3] * 1000);
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
		no_philo[i] = atoi(argv[i + 1]);
		i++;
	}
	if (argv[i + 1])
		no_philo[i] = atoi(argv[i + 1]);
	else
		no_philo[i] = 0;

	// birthTime and lastTimeEat allocation
	lastTimeEat = malloc(sizeof(uint64_t) * no_philo[0]);

	// mallocing the thread and the mutex
	m_forks = malloc(sizeof(pthread_mutex_t) * no_philo[0]);
	if (!m_forks)
		return (1);

	// initialization of forks(mutex)
	i = 0;
	while (i < no_philo[0])
	{
		pthread_mutex_init(&m_forks[i], NULL);
		i++;
	}

	// initialization of write mutex
	pthread_mutex_init(&m_write, NULL);

	// initialization and lock the finish program mutex
	pthread_mutex_init(&m_somebodydied, NULL);
	pthread_mutex_lock(&m_somebodydied);

	startTime = getTime();
	// creation of philosophers threads
	i = 0;
	while (i < no_philo[0])
	{
		pthread_create(&philo, NULL, routine, (void *)&i);
		pthread_detach(philo);
		usleep(100);
		i++;
	}
	while (1){}
	return (0);
}
