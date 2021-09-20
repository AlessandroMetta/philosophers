#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_life
{
	pthread_mutex_t	m_write;
	pthread_mutex_t	*m_forks;
	pthread_mutex_t	m_somebodydied;
	int				no_philo[5];			//number of philosophers and forks
	int				nu;
	uint64_t		startTime;
	uint64_t		*lastTimeEat;
}				t_life;

uint64_t getTime();
void message(int nu, char *msg, t_life *table);
