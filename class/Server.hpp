/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 10:16:34 by mjourno           #+#    #+#             */
/*   Updated: 2023/11/04 11:02:38 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../Include.hpp"
# include "Client.hpp"
# include <map>

# define MAX_EVENTS 10

class Server {
	private:
		int	fd, epollfd, number_fds;
		struct sockaddr_in	addr;
		std::map<int, Client>	clients;
		struct epoll_event	ev, events[MAX_EVENTS];
	public:
		Server(char *port, char *pass);
		void	Launch();
		~Server();

};

#endif