/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/26 15:58:54 by ftaffore          #+#    #+#             */
/*   Updated: 2015/04/26 15:58:55 by ftaffore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	pid_t							pid;
	int ret;
	char			*str = strdup("ls");
	printf("'%p', '%s'\n", str, str);

	if ((pid = fork()) < 0) {
		return (1);
	}
	if (pid == 0) {
		execle("/bin/sh", "sh", "-c", str, NULL);
		printf("exec fail : %s\n", strerror(errno));
	}
	else {
		wait(&ret);
		printf("father stop %d\n", ret);
	}

	return (0);
}