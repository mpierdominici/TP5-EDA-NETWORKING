#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <cstdint>

class servidor
{
public:
	servidor(UINT32 port=12345);
	~servidor();
	void waitForCliente();
	bool receiveDataForCliente(char * buffer_t, int bufferSize);
private:
	UINT32 portNumber;
	boost::asio::io_service*  ioServer;
	boost::asio::ip::tcp::socket* ServerSocket;
	boost::asio::ip::tcp::acceptor* conectionServerAceptor;
};

