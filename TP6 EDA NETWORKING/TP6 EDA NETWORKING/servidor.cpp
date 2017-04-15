#include "stdafx.h"
#include "servidor.h"

//servidor()
//recive como paramtro un int, que representa el puerto que el srvidor escucha,
//en caso que ese puerto no sea indicado, el puerto por defecto es 12345.
//
servidor::servidor(UINT32 port)
{
	portNumber = port;//asigno el puerto al servidor
	ioServer = new boost::asio::io_service();//creo el io_service del servidor
	ServerSocket = new boost::asio::ip::tcp::socket(*ioServer);//creo el socket del servidor
	conectionServerAceptor = new boost::asio::ip::tcp::acceptor(*ioServer,
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), portNumber));//creo el aceptor del servidor
	//std::cout << std::endl << "El puerto " << portNumber << " se creo" << std::endl;
}

servidor::~servidor()
{
	delete ioServer;
	delete ServerSocket;
	delete conectionServerAceptor;
}

//waitForCliente()
//metodo bloqueante, que espera la conexion del cliente.
//

void servidor::waitForCliente()
{
	conectionServerAceptor->accept(*ServerSocket);
}
//receiveDataForCliente
//previamente se deve llamar a waitforcleinte()
//recive como paramteros un arreglo de char(buffer) y una int,
//con la cantidad de elementos de dicho arreglo.
//si se puedo recivir toda la informacion devuelve un true, caso contrario
//devuelve un false.
bool servidor::receiveDataForCliente(char * buffer_t, int bufferSize)
{
	UINT16 longitudDelMensaje = 0;
	boost::system::error_code error;
	char bufferTemp[900];
	do
	{
		ServerSocket->read_some(boost::asio::buffer(bufferTemp), error);//recive la informacion del cliente y la guarda en bufferTemp

	} while (!error);

	if (error == boost::asio::error::eof)
	{
		if (longitudDelMensaje < bufferSize)
		{
			for (size_t i = 0; i < bufferSize; i++)//transfiero la informacion de un buffer al otro
			{
				buffer_t[i] = bufferTemp[i];
			}
			return true;
		}
		else// en caso que el buffer enviado por el usuario sea muy chico envio false
		{
			return false;
		}

	}
	else
	{
		return false;
	}
		

	
}
