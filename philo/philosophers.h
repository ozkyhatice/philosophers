/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fekiz <fekiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:08:35 by fekiz             #+#    #+#             */
/*   Updated: 2024/03/11 16:10:57 by fekiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct philos
{
	int				id;
	long			last_eat;
	bool			is_dead;
	pthread_t		thread;
	struct s_list	*data;
}	t_philo;

typedef struct s_list
{
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	long			ms;
	int				number_philo;
	int				cont;
	int				must_eat;
	int				eat_count;
	t_philo			*philos;
	struct timeval	tv;
	pthread_mutex_t	*forks;
	pthread_mutex_t	is_eat;
	pthread_mutex_t	write_mutex;
}	t_list;

long	ft_atoi(char *str);
long	get_ms(t_list *data, bool ok, bool eat_pause, int id);
long	get_mss(t_list *data);
int		data_creat(char **str, t_list *data);
int		num_control(char **str, t_list *data);
void	*life(void *philo);
void	writes(t_philo *p, int num);
void	main_thread(t_list *data);
void	ms_sleep(t_philo *p, long time);

#endif