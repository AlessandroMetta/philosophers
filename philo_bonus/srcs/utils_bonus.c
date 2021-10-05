/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ametta <ametta@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 19:08:07 by ametta            #+#    #+#             */
/*   Updated: 2021/10/05 12:05:54 by ametta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo_bonus.h"

int	get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	message(t_args *table, int philo_number, char *msg)
{
	sem_wait(table->sem_write);
	printf("[%d]\tPhilosopher %d %s\n", get_time() - table->start_time,
		philo_number + 1, msg);
	if (msg[0] != 'd')
		sem_post(table->sem_write);
}

void	ft_usleep(uint64_t time_in_ms)
{
	uint64_t	start_time;

	start_time = 0;
	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

int	ft_str_is_number(char *string)
{
	int	i;

	i = 0;
	while (string[i] != '\0')
	{
		if (string[i] < '0' || string[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

void	close_processes(t_args *table)
{
	int	i;
	int	status;

	i = 0;
	waitpid(-1, &status, 0);
	if (WIFEXITED(status) || WIFSIGNALED(status))
	{
		while (i < table->philo_ammount)
			kill(table->philo[i++]->philo_pid, SIGKILL);
	}
}
