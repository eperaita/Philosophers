/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperaita <eperaita@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:20:19 by eperaita          #+#    #+#             */
/*   Updated: 2021/12/11 19:35:17 by eperaita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../philo.h"


////////////////// TIMESTAMP //////////////////

long int o_clock(t_philo *philo)
{
    struct timeval now;
    long int ms;

    gettimeofday(&now, NULL);
    ms = (now.tv_sec * 1000 + now.tv_usec / 1000) - (philo->table->startime.tv_sec * 1000 + philo->table->startime.tv_usec / 1000);
    return (ms);
}


/////////////////////////DEADS///////////////////

static int is_dead(t_philo *philo)
{
	struct timeval now;
	long int period;

	gettimeofday(&now, NULL);
	period = (now.tv_sec * 1000 + now.tv_usec / 1000) - (philo->eatime.tv_sec * 1000 + philo->eatime.tv_usec / 1000);
	if (period > philo->table->t_die)
		return (1);
	return (0);
}

static int any_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->table->deadlock);
	if (philo->table->death)//hay alguien muerto
    {
		pthread_mutex_unlock(philo->table->deadlock);
	 	return(1);
	}
	if (is_dead(philo)) //estoy muerta
	{
		philo->table->death = 1;
		pthread_mutex_unlock(philo->table->deadlock);
		printf("%ld %d died\n", o_clock(philo) , philo->id);
		return (1);
	}
	pthread_mutex_unlock(philo->table->deadlock);
	return (0);
}

//////////////////MY USLIP //////////////////////////

int my_uslip(t_philo *philo, int time)
{
    struct timeval now;
    struct timeval end;
    int i;

    gettimeofday(&now, NULL);
    gettimeofday(&end, NULL);
    i = 0;
    while ((end.tv_sec * 1000 + end.tv_usec / 1000) - (now.tv_sec * 1000 + now.tv_usec / 1000)  < time)
    {
        usleep(100);
        gettimeofday(&end, NULL);
        if ((end.tv_sec * 1000 + end.tv_usec / 1000) - (now.tv_sec * 1000 + now.tv_usec / 1000) - i > 10)
        {
            i += 10;
            if (any_dead(philo))
                return (1);
        }
    }
	if (any_dead(philo))
                return (1);
    return (0);

}
//////////////////////////////////////7
int borrowed(t_philo *philo)
{
	int fork;

    if (philo->id == 1)
        fork = philo->table->nphilo - 1;
	else
		fork = philo->id - 2;
    return (fork);
}

static void get_fork_and_return(t_philo *philo, int on)
{
	pthread_mutex_lock(&philo->forks[philo->id - 1].mutex);
	pthread_mutex_lock(&philo->forks[borrowed(philo)].mutex);
	philo->forks[philo->id].fork = on;
	if (on == 0)
		printf("%ld %d has taken a fork\n", o_clock(philo) , philo->id);
	philo->forks[borrowed(philo)].fork = on;
	if (on == 0)
		printf("%ld %d has taken a fork\n", o_clock(philo) , philo->id);
	pthread_mutex_unlock(&philo->forks[philo->id - 1].mutex);
	pthread_mutex_unlock(&philo->forks[borrowed(philo)].mutex);
}

////////////////ROUTINE//////////////////
static int thinking(t_philo *philo)
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

static int sleeping(t_philo *philo)
{
	printf("%ld %d is sleeping\n", o_clock(philo) , philo->id);
	if(my_uslip(philo, philo->table->t_sleep))
        return (1);
	return (0);
}

static int eating(t_philo *philo)
{
	get_fork_and_return(philo, 0);
	gettimeofday(&philo->eatime, NULL);
	printf("%ld %d is eating\n", o_clock(philo) , philo->id);
	if (my_uslip(philo, philo->table->t_eat))
		return (1);
	get_fork_and_return(philo, 1);
	return (0);
}

static void philos_routine(t_philo *philo)
{
	while (!any_dead(philo))
	{
		if (philo->group == 2)
		{
			philo->group = 3;
			if (sleeping(philo))
				break;
		}
		if (philo->group == 3)
		{
			philo->group = 1;
			if((thinking(philo)))
				break;
		}
		if (philo->group == 1)
		{
			philo->group = 2;
			if (eating(philo))
				break;
		}
	}
}

static int my_group(t_philo *philo)
{
	int group;

	group = 0;
	if (philo->table->nphilo % 2 != 0 && philo->id == philo->table->nphilo)
		group = 3;
	else if (philo->id % 2 != 0)
		group = 1;
	else if	(philo->id % 2 == 0)
		group = 2;
	return (group);
}

void *philos_dictator(void *philo)
{
	t_philo me_philo;
	//int count;

	me_philo = *(t_philo *)philo; //copio la estructura, ahora solo scope local (hilo)
	pthread_mutex_unlock(me_philo.mutex);
	//////ESPERA A QUE EMPIECE EL TIEMPO/////////
	while (!me_philo.table->startime.tv_sec && !me_philo.table->startime.tv_usec)
		usleep(5);
	//////RECARGA VIDA Y ASIGNA GRUPO///////
	me_philo.eatime = me_philo.table->startime;
	me_philo.group = my_group(&me_philo);
	//printf("Soy la filósofa %d en %ld y soy grupo %d\n", me_philo.id, o_clock(&me_philo), me_philo.group);
	///////////////////ROUTINE//////////////
	philos_routine(&me_philo);	
	return (NULL);
}

