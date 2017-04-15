#include "stdafx.h"
#include "cliente.h"

cliente::cliente()
{
	ioCliente = new boost::asio::io_service();
	clienteSocket = new boost::asio::ip::tcp::socket(*ioCliente);
	clienteResolver = new boost::asio::ip::tcp::resolver(*ioCliente);
}

cliente::~cliente()
{
	delete ioCliente;
	delete clienteSocket;
	delete clienteResolver;
}

//ConectToServer()
//metodo bloqueante que espera a conectarse con un servidor
//recive como paramteros 2 string. el primero, ipserver, es la ip del servidor
//y el segundo string, portnumber, es el numero del puerto en el cual el servidor
//esta escuchando
void cliente::ConectToServer(const char * ipServer, const char * portNumber)
{
	endpoint = clienteResolver->resolve(boost::asio::ip::tcp::resolver::query(ipServer, portNumber));
	boost::asio::connect(*clienteSocket, endpoint);
}

//sendData()
//recive un arreglo de char, que son lo elementos que mandara. tambien recive
//un int con la cantidad de elementos que se necesitan enviar
//

bool cliente::sendData(char * dataToSend_t, unsigned int sizeData)
{
	char DataToSend[800];
	
	for (size_t i = 0; i < sizeData; i++)
	{
		DataToSend[i] = dataToSend_t[i];
	}
	std::cout << DataToSend[0];

	boost::function<void(const boost::system::error_code&, std::size_t)> handler(
		boost::bind(&cliente::writeCompletitionCallback, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	
	boost::asio::async_write(*clienteSocket, boost::asio::buffer(DataToSend), handler);

	return true;

}

void cliente::writeCompletitionCallback(const boost::system::error_code& error, std::size_t transfered_bytes) {
	std::cout << std::endl << "Write Callback called" << std::endl;
}