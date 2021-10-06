/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ametta <ametta@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 18:46:41 by ametta            #+#    #+#             */
/*   Updated: 2021/10/06 15:29:34 by ametta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t		philo_thread;
	pthread_t		monitor_thread;
	int				philo_number;
	int				last_meal_time;
	int				meal_counter;
	int				philo_left_fork;
	int				philo_right_fork;
	int				is_eating;
	pthread_mutex_t	mutex_eating;
	struct s_args	*table;
}				t_philo;

typedef struct s_args
{
	int				philo_ammount;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_ammount;
	int				start_time;
	pthread_mutex_t	mutex_write;
	pthread_mutex_t	*mutex_forks;
	int				finish;
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

#endif