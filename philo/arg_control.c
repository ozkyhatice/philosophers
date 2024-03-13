/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fekiz <fekiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:08:08 by fekiz             #+#    #+#             */
/*   Updated: 2024/03/11 16:20:19 by fekiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	ft_atoi(char *str)
{
	long	num;
	long	sign;
	int		i;

	i = 0;
	num = 0;
	sign = 1;
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		num += str[i] - 48;
		i++;
	}
	num *= sign;
	if (num > 2147483647 || num < -2147483648)
		return (0);
	return (sign * num);
}

long	get_ms(t_list *data, bool ok, bool eat_pause, int id)
{
	int		i;

	if (ok == true)
	{
		i = -1;
		while (++i < data->number_philo)
		{
			data->philos[i].id = i;
			data->philos[i].data = data;
			data->philos[i].is_dead = false;
			data->philos[i].last_eat = get_mss(data);
			if (pthread_create(&data->philos[i].thread, NULL,
					life, &data->philos[i]) != 0)
				return (-1);
			usleep(100);
		}
		return (-2);
	}
	if (eat_pause == true)
		ms_sleep(&data->philos[id], data->time_to_eat);
	return (get_mss(data));
}

int	data_creat(char **str, t_list *data)
{
	int	i;

	i = -1;
	data->number_philo = (int)ft_atoi(str[1]);
	data->time_to_die = ft_atoi(str[2]);
	data->time_to_eat = ft_atoi(str[3]);
	data->time_to_sleep = ft_atoi(str[4]);
	data->start_time = get_mss(data);
	data->eat_count = 0;
	data->must_eat = -1;
	if (str[5])
		data->must_eat = ft_atoi(str[5]);
	if (pthread_mutex_init(&data->is_eat, NULL) != 0
		|| pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (1);
	data->philos = malloc(sizeof(t_philo) * data->number_philo);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_philo);
	if (!(data->philos) || !(data->forks))
		return (1);
	while (++i < data->number_philo)
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
	if (get_ms(data, true, false, 0) == -1)
		return (1);
	return (0);
}

int	num_control(char **str, t_list *data)
{
	int	i;
	int	j;

	j = 1;
	while (str[j])
	{
		i = 0;
		while (str[j][i] && (str[j][i] == 32
			|| (str[j][i] >= 9 && str[j][i] <= 13)))
			i++;
		while (str[j][i])
		{
			if (str[j][i] >= '0' && str[j][i] <= '9')
				i++;
			else
				return (1);
		}
		j++;
	}
	if (data_creat(str, data) == 1)
		return (1);
	return (0);
}

void	*life(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	while (1)
	{
		pthread_mutex_lock(&p->data->forks[p->id]);
		writes(p, 0);
		pthread_mutex_lock(&p->data->forks[(p->id + 1)
			% p->data->number_philo]);
		writes(p, 0);
		writes(p, 3);
		pthread_mutex_lock(&p->data->is_eat);
		p->data->eat_count++;
		p->last_eat = get_mss(p->data);
		pthread_mutex_unlock(&p->data->is_eat);
		ms_sleep(p, p->data->time_to_eat);
		writes(p, 1);
		pthread_mutex_unlock(&p->data->forks[p->id]);
		pthread_mutex_unlock(&p->data->forks[(p->id + 1)
			% p->data->number_philo]);
		ms_sleep(p, p->data->time_to_sleep);
		writes(p, 2);
	}
	return (NULL);
}
