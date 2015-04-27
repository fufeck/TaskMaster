// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/13 12:49:09 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/13 12:49:10 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
#include "Taskmaster.hpp"

int 					main(int ac, char **av) {
	/*pid_t							pid;
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
	}
	printf("father stop %d\n", pid);

	return (0);
*/
	try {
		Taskmaster		taskmaster(ac, av);

		taskmaster.run();
	} catch (std::exception & e) {
		return 1;
	}
	return 0;
}