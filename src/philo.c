/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eperaita <eperaita@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:20:19 by eperaita          #+#    #+#             */
/*   Updated: 2021/12/11 21:07:27 by eperaita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../philo.h"

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

