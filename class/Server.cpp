/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 10:16:39 by mjourno           #+#    #+#             */
/*   Updated: 2023/11/03 15:50:09 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char *arg) : addr(sockaddr_in()) {
	fd = socket(AF_INET, SOCK_STREAM, 0); //socket to listen on port
	//if (fd == -1)
	//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno); //error
	socklen_t	len = sizeof(addr);
	std::memset(&addr, 0, len);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	// This ip address will change according to the machine
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //changer ip
	std::stringstream	ss(arg);
	int	i;
	ss >> i;
	addr.sin_port = htons(i);
	bind(fd, (struct sockaddr *) &addr, len);
	//if (bind(fd, (struct sockaddr *) &addr, len) == -1)
	//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);
	listen(fd, 10);
	//if (listen(fd, 10) == -1) //changer 10
	//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);

	struct epoll_event ev = {}, events[MAX_EVENTS];
	epollfd = epoll_create1(0);
	//if (epollfd == -1)
	//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
	//if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);

	struct sigaction act, oldact;
	act.sa_handler = sigint_handler;
	sigemptyset(&act.sa_mask); // if == -1 errno
	act.sa_flags = 0;
	sigaction(SIGINT, NULL, &oldact); // if == -1 errno
	sigaction(SIGINT, &act, NULL); // if == -1 errno

	while (1) {
		number_fds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		//if (number_fds == -1)
		//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);

		for (i = 0; i < number_fds; i++) {
			//Premiere connexion
			if (events[i].data.fd == fd) {
				//cfd = accept(fd, (struct sockaddr *) &peer_addr, &peer_addr_size);
				clients.push_back(Client());
				clients.back().fd = accept(fd, (struct sockaddr *) &clients.back().addr, &clients.back().addr_size);
				//if (cfd == -1)
				//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);

				fcntl(clients.back().fd, F_SETFL, O_NONBLOCK);
				//if (fcntl(cfd, F_SETFL, O_NONBLOCK) == -1) //setnonblocking
				//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = clients.back().fd;

				epoll_ctl(epollfd, EPOLL_CTL_ADD, clients.back().fd, &ev);
				//if (epoll_ctl(epollfd, EPOLL_CTL_ADD, cfd, &ev) == -1)
				//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);

				std::cout << "connected" << std::endl;

				char *ip;
				ip = inet_ntoa(clients.back().addr.sin_addr);
				std::cout << "ip: " << ip << " port: " << ntohs(clients.back().addr.sin_port) << std::endl;

				char buffer1[256], buffer2[256];
				std::memset(&buffer2, 0, 256);
				recv(clients.back().fd, buffer2, 256, 0);
				//if (recv(cfd, buffer2, 256, 0) == -1 && errno != EAGAIN) // verif eagain
				//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);
				std::cout << "Client : " << buffer2 << std::endl;

				std::memset(&buffer1, 0, 256);
				strcpy(buffer1, "Hello");
				send(clients.back().fd, buffer1, 256, 0);
				//if (send(cfd, buffer1, 256, 0) == -1)
				//	return print_error(__FILE__, __LINE__, std::strerror(errno), errno);
			}//deja connecté
			else {
				//do_use_fd(events[n].data.fd);
				std::cout << "else" << std::endl;
			}
		}
	}
}

Server::~Server() {
	close(fd);
	close(epollfd);
	for (int i = 0; i < number_fds - 1; i++) {
		close(clients[i].fd);
	}
	//if == -1...
}