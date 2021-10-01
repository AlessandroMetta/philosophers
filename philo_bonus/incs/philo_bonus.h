#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
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
	struct s_args	*table;
}				t_philo;

typedef struct s_args
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
	t_philo			**philo;
}				t_args;

uint64_t	get_time(void);
void		message(t_args *table, uint64_t philo_number, char *msg);
void		ft_usleep(uint64_t time_in_ms);
t_args		*init(int argc, char **argv);
void		take_fork(t_philo *ph);
void		eat(t_philo *ph);
void		go_to_bed(t_philo *ph);
int			ft_str_is_number(char *string);
int			ft_atoi(const char *str);
int			checking_args_validity(char **argv, int argc);

#endif