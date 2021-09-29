#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct philo_s
{
	pthread_t		philo_thread;
	pthread_t		monitor_thread;
	uint64_t		philo_number;
	uint64_t		last_meal_time;
	uint64_t		meal_counter;
	uint64_t		philo_left_fork;
	uint64_t		philo_right_fork;
	uint64_t		is_eating;
	pthread_mutex_t	mutex_eating;
	struct args_s	*table;
}				philo_t;

typedef struct args_s
{
	uint64_t		philo_ammount;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	uint64_t		meal_ammount;
	uint64_t		start_time;
	pthread_mutex_t	mutex_write;
	pthread_mutex_t	*mutex_forks;
	uint64_t		finish;
	philo_t			**philo_th;
}				args_t;

uint64_t	get_time();
void		message(args_t *table, uint64_t philo_number, char *msg);
void		ft_usleep(uint64_t time_in_ms);
int			init(args_t *table, int argc, char **argv);
void		take_fork(philo_t *ph);
void		eat(philo_t *ph);
void		go_to_bed(philo_t *ph);
int			ft_is_number(char *string);
int			ft_atoi(const char *str);
