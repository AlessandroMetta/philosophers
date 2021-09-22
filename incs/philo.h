#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_args
{
	uint64_t		num_of_philo;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	uint64_t		meal_ammount;
	pthread_mutex_t	action;
	pthread_mutex_t	m_write;
	pthread_mutex_t	*m_forks;
	pthread_mutex_t	m_somebodydied;
	uint64_t		start_time;
	uint64_t		exit;
}				t_args;

typedef struct s_philo
{
	uint64_t	id;
	uint64_t	last_meal;
	t_args		*table;
	uint64_t	meal_counter;
	pthread_t	philo;
}				t_philo;

uint64_t	get_time();
void		message(int nu, char *msg, t_args *table);
void		ft_usleep(uint64_t time_in_ms);
int			init(t_args *table, int argc, char **argv);
void		take_fork(t_args *table, int nu);
void		eat(t_philo *ph, int nu);
void		go_to_bed(t_args *table, int nu);
