/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperaita <eperaita@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 21:03:51 by eperaita          #+#    #+#             */
/*   Updated: 2021/12/11 21:07:11 by eperaita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../philo.h"

//////////////////////////////////////
static int borrowed(t_philo *philo)
{
    int fork;

    if (philo->id == 1)
        fork = philo->table->nphilo - 1;
    else
        fork = philo->id - 2;
    return (fork);
}

void get_fork_and_return(t_philo *philo, int on)
{
    pthread_mutex_lock(&philo->forks[philo->id - 1].mutex);
    philo->forks[philo->id - 1].fork = on;
    if (on == 0)
        printf("%ld %d has taken a fork\n", o_clock(philo) , philo->id);
    pthread_mutex_lock(&philo->forks[borrowed(philo)].mutex);
    philo->forks[borrowed(philo)].fork = on;
    if (on == 0)
        printf("%ld %d has taken a fork\n", o_clock(philo) , philo->id);
    pthread_mutex_unlock(&philo->forks[philo->id - 1].mutex);
    pthread_mutex_unlock(&philo->forks[borrowed(philo)].mutex);
}

////////////////ROUTINE//////////////////


int thinking(t_philo *philo)
{
    printf("%ld %d is thinking\n", o_clock(philo) , philo->id);
    if (philo->table->nphilo % 2 != 0)
    {
        if(my_uslip(philo, ((philo->table->t_eat * 2) - philo->table->t_sleep)))
               return (1);
    }
    else if (philo->table->nphilo % 2 == 0)
    {
        if (my_uslip(philo, (philo->table->t_eat - philo->table->t_sleep)))
                return (1);
        }
    return (0);
}

int sleeping(t_philo *philo)
{
    printf("%ld %d is sleeping\n", o_clock(philo) , philo->id);
    if(my_uslip(philo, philo->table->t_sleep))
        return (1);
    return (0);
}

int eating(t_philo *philo)
{
    get_fork_and_return(philo, 0);
    gettimeofday(&philo->eatime, NULL);
    printf("%ld %d is eating\n", o_clock(philo) , philo->id);
    if (my_uslip(philo, philo->table->t_eat))
        return (1);
    get_fork_and_return(philo, 1);
    return (0);
}
