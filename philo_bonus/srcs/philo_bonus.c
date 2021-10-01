/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ametta <ametta@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 19:07:54 by ametta            #+#    #+#             */
/*   Updated: 2021/10/01 19:12:25 by ametta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo_bonus.h"

void	*monitor(void *arg)
{
	t_philo	*ph;

	ph = ((t_philo *)arg);
	while (1)
	{
		sem_wait(ph->table->sem_write);
		if (get_time() - ph->last_meal_time >= ph->table->time_to_die)
		{
			message(ph->table, ph->philo_number, "died");
			exit(1);
		}
		sem_post(ph->table->sem_write);
		usleep(100);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*ph;

	ph = ((t_philo *)arg);
	while (ph->table->finish)
	{
		take_fork(ph);
		sem_post(ph->table->sem_forks);
		sem_wait(ph->table->sem_forks);
		message(ph->table, ph->philo_number, "is sleeping");
		ft_usleep(ph->table->time_to_sleep);
		message(ph->table, ph->philo_number, "is thinking");
	}
	return (NULL);
}

void	start_philo(t_args		*table)
{
	uint64_t	i;

	i = 0;
	while (i < table->philo_ammount)
	{
		table->philo[i]->last_meal_time = get_time();
		pthread_create(&table->philo[i]->philo_thread, NULL, routine,
			(void *)table->philo[i]);
		i++;
		usleep(100);
	}
	i = 0;
	while (i < table->philo_ammount)
	{
		pthread_create(&table->philo[i]->monitor_thread, NULL, monitor,
			(void *)table->philo[i]);
		i++;
		usleep(100);
	}
}

int	main(int argc, char **argv)
{
	t_args		*table;

	if (checking_args_validity(argv, argc))
		return (1);
	table = init(argc, argv);
	table->finish = 1;
	table->start_time = get_time();
	start_philo(table);
	while (table->finish)
		continue ;
	return (0);
}
