/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ametta <ametta@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 18:47:06 by ametta            #+#    #+#             */
/*   Updated: 2021/10/05 12:05:33 by ametta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_philo
{
	pid_t			philo_pid;
	pthread_t		monitor_die;
	int				philo_number;
	int				last_meal_time;
	sem_t			*sem_eat;
	struct s_args	*table;
}				t_philo;

typedef struct s_args
{
	pthread_t		monitor_meal;
	int				philo_ammount;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_ammount;
	int				start_time;
	sem_t			*sem_write;
	sem_t			*sem_meal;
	sem_t			*sem_forks;
	t_philo			**philo;
}				t_args;

int			get_time(void);
void		message(t_args *table, int philo_number, char *msg);
void		ft_usleep(uint64_t time_in_ms);
t_args		*init(int argc, char **argv);
void		take_fork(t_philo *ph);
void		eat(t_philo *ph);
void		go_to_bed(t_philo *ph);
int			ft_str_is_number(char *string);
int			ft_atoi(const char *str);
int			checking_args_validity(char **argv, int argc);
void		close_processes(t_args *table);

#endif