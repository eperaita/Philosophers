/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperaita <eperaita@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 21:01:26 by eperaita          #+#    #+#             */
/*   Updated: 2021/12/11 17:05:54 by eperaita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_forks
{
    int fork;
    pthread_mutex_t mutex;
} t_forks;

typedef struct s_table
{
	pthread_t *thread;
    struct timeval startime;
    int     nphilo;
    int    t_die;
    int    t_eat;
    int    t_sleep;
    int     death;
    int ndinner;
	pthread_mutex_t	*deadlock;
}   t_table;

typedef struct s_philo
{
    int     id;
	int		group;
    struct  timeval eatime;
    t_table    *table;
    t_forks    *forks;
    pthread_mutex_t *mutex;
}   t_philo;


////////////////////////
void *philos_dictator(void *philo);

/////////UTILS////////
int ft_atoi(const char *str);



#endif
