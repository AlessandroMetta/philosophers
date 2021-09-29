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
	uint64_t		start_time;
	pthread_mutex_t	m_write;
	pthread_mutex_t	*m_forks;
	uint64_t		finish;
	pthread_t		**philo;
}				t_args;

typedef struct s_philo
{
	uint64_t	id;
	uint64_t	last_meal;
	uint64_t	meal_counter;
	t_args		*table;
	pthread_mutex_t	m_eating;
	int			iseating;
}				t_philo;
/*
typedef struct s_table
{
	pthread_mutex_t	m_die_checker;
	pthread_mutex_t	m_write;
	pthread_mutex_t	*m_forks;
	pthread_mutex_t	m_somebodydied;
	uint64_t		start_time;
	uint64_t		finish;
}				t_table;

typedef struct s_all
{
	t_args		*args;
	t_philo		*philos;
	t_table		*table;
}				t_table;
*/
uint64_t	get_time();
void		message(int nu, char *msg, t_args *table);
void		ft_usleep(uint64_t time_in_ms);
int			init(t_args *table, int argc, char **argv);
void		take_fork(t_philo *ph);
void		eat(t_philo *ph);
void		go_to_bed(t_philo *ph);
int			ft_is_number(char *string);
int			ft_atoi(const char *str);