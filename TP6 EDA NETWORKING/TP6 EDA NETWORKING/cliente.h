#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
class cliente
{
public:
	cliente();
	~cliente();
	void ConectToServer(const char* ipServer, const char* portNumber);
	bool sendData(char * dataToSend, unsigned int sizeData);
	void writeCompletitionCallback(const boost::system::error_code & error, std::size_t transfered_bytes);
private:
	boost::asio::ip::tcp::resolver* clienteResolver;
	boost::asio::ip::tcp::socket* clienteSocket;
	boost::asio::ip::tcp::resolver::iterator endpoint;
	boost::asio::io_service* ioCliente;
};

