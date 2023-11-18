/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Include.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 15:18:24 by mjourno           #+#    #+#             */
/*   Updated: 2023/11/04 11:02:15 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <sstream>

//socket
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
//close
# include <unistd.h>
//errno
# include <cerrno>
# include <cstring>
//inet_ntoa
# include <netinet/in.h>
# include <arpa/inet.h>
//getprotobyname
# include <netdb.h>
//epoll
# include <sys/epoll.h>
//fcntl
# include <fcntl.h>
//sigaction
# include <signal.h>
# include <iostream>

# include <string>

# include <vector>
# include <map>

# define CRLF std::string("\r\n")

class SigInt : public std::exception {
	virtual const char *what() const throw() {
		return "Ctrl + C";
	}
};

int		print_error(std::string file, int line, std::string error, int err);
void	sigint_handler(int signum);

std::vector<std::string>	split(std::string str);

#endif