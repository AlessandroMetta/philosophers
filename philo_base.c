#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t m_forks[2];
pthread_mutex_t m_write;

void *routine(void *nu)
{
	while (1)
	{
	pthread_mutex_lock(&m_forks[0]);
	pthread_mutex_lock(&m_write);
	printf("philo %d take a fork\n", (int) nu);
	pthread_mutex_unlock(&m_write);
	pthread_mutex_lock(&m_forks[1]);
	pthread_mutex_lock(&m_write);
	printf("philo %d take a fork\n", (int) nu);
	printf("philo %d is eating...\n", (int) nu);
	pthread_mutex_unlock(&m_write);
	usleep(100);
	pthread_mutex_lock(&m_write);
	printf("philo %d eaten\n", (int) nu);
	pthread_mutex_unlock(&m_write);
	pthread_mutex_unlock(&m_forks[0]);
	pthread_mutex_unlock(&m_forks[1]);
	pthread_mutex_lock(&m_write);
	printf("philo %d is sleeping...\n", (int) nu);
	pthread_mutex_unlock(&m_write);
	usleep(100);
	pthread_mutex_lock(&m_write);
	printf("philo %d thinking\n", (int) nu);
	pthread_mutex_unlock(&m_write);
	usleep(100);
	}
	return (NULL);
}

int main()
{
	pthread_t philo1, philo2;

	pthread_mutex_init(&m_forks[0], NULL);
	pthread_mutex_init(&m_forks[1], NULL);
	pthread_mutex_init(&m_write, NULL);

	pthread_create(&philo1, NULL, routine, (void *)1);
	usleep(100);
	pthread_create(&philo2, NULL, routine, (void *)2);
	pthread_join(philo1, NULL);
	pthread_join(philo2, NULL);
	pthread_mutex_destroy(&m_forks[0]);
	pthread_mutex_destroy(&m_forks[1]);
	pthread_mutex_destroy(&m_write);
	return (0);
}