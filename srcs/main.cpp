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

#include <iostream>
#include "Taskmaster.hpp"

int 					main(int ac, char **av) {
	try {
		Taskmaster		taskmaster(ac, av);

		//taskmaster.run();
	} catch (std::exception & e) {
		return 1;
	}
	return 0;
}