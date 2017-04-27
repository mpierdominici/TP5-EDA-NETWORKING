// TP6 EDA NETWORKING.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"


extern "C" {
#include "parseCmdLine.h"
#include "moreString.h"
}

#include "read_IPS.h"
#include "iniciar.h"
#include "cliente.h"
#include "servidor.h"
#include "Graphic.h"
#include "Animation.h"
#include <new>
#include <iostream>
#include <cstdint>
#include <cstdio>

#define IPS_FILE		"directions.txt"
#define	PORT			"12345"
#define N_ANIMATIONS	6

#define IP_SIZE			30 //?????????????????????????????????????????????

enum modes { LISTENING, SENDING, PLAYING, MAKING, FINISHED };


//PATH de los archivos de cada animacion. cada imagen debe estar numerada a partir del 1 (con el numero al final) Y FORMATO PNG
#define A_PATH "Img/Cat Running/Cat Running-F"
#define B_PATH "Img/Explosion 1/Explosion 1-F"
#define C_PATH "Img/Explosion 2/Explosion 2-F"
#define D_PATH "Img/Homer Dance/homerdance-F"
#define E_PATH "Img/Sonic/Sonic Running-F"
#define F_PATH "Img/Super Mario/Super Mario Running-F"

#define MAX_STRLEN 50


//FRAME RATE de cada animacion (en segundos)
#define A_FR	0.100	
#define B_FR	0.120
#define C_FR	0.100
#define D_FR	0.100
#define E_FR	0.040
#define F_FR	0.060

//CADA CUANTOS FRAMES se debe mover cada animacion
#define A_STEP	1
#define B_STEP	0
#define C_STEP	0
#define D_STEP	1
#define E_STEP	1
#define F_STEP	4

//VELOCIDAD de cada animacion (en px/step, positivo hacia la derecha)
#define A_SPEED	20
#define B_SPEED 0
#define C_SPEED 0
#define D_SPEED 100
#define E_SPEED -20
#define F_SPEED	70

//NUMERO DE IMAGENES que componen cada animacion
//TIENE QUE SER MENOR QUE 100 PARA CADA UNA!!!
#define A_IMGS	12
#define B_IMGS	8
#define C_IMGS	48
#define D_IMGS	10
#define E_IMGS	10
#define F_IMGS	12


#if (A_IMGS > 99 || B_IMGS > 99 || C_IMGS > 99 || D_IMGS > 99 || E_IMGS > 99 || F_IMGS > 99 )
#error "Too many images for a single animation"
#endif


//ANCHO de las imagenes de cada animacion (en pixeles)
#define A_WIDTH	688
#define B_WIDTH 450
#define C_WIDTH 256
#define D_WIDTH 320
#define E_WIDTH	700
#define F_WIDTH 650

//ALTURA de las imagenes de cada animacion (en pixeles)
#define A_HEIGHT	387
#define B_HEIGHT	274
#define C_HEIGHT	256
#define D_HEIGHT	320
#define E_HEIGHT	600
#define F_HEIGHT	660



//ID DE LA PRIMERA IMAGEN de cada animacion
#define A_ID	0
#define B_ID	A_ID + A_IMGS 
#define C_ID	B_ID + B_IMGS
#define D_ID	C_ID + C_IMGS
#define E_ID	D_ID + D_IMGS
#define F_ID	E_ID + E_IMGS

#define N_IMGS	F_ID + F_IMGS


using namespace std;

bool initialize(Graphic **, Animation *);	//inicializar esas dos cosas con los params definidos arriba
int32_t	parserCallback(char *, char*, void *);

int main(int argc, char *argv[])
{
	char * YouGo;
	bool isValid;
	int mode = LISTENING;
	unsigned int myId;

	int nPCs = getNumberOfLines("directions.txt");//(IPS_FILE); //numero de lineas == numero de compus
	if (nPCs <= 1)
		return EXIT_FAILURE;

	try {
		YouGo = new char[nPCs + 2];			//YouGo va a ser el bloque de datos que mando y recibo
	}
	catch (bad_alloc& error) {
		cerr << "bad_alloc caught: " << error.what() << endl;
	}


	Graphic * g = NULL;
	Animation a[N_ANIMATIONS];
	if (initialize(&g, a) != true)		//configurar parte grafica (no crea el display!)
		return EXIT_FAILURE;

	unsigned int userData[3] = { nPCs, 0, 0 }; //datos para el callback:
											   // primero cuantas compus hay (para saber cuales serian validos), en el segundo dato se va a guardar el numero de compus
											   // y el tercero va a ser 1 si esta es la compu que inicia y viceversa

	int parsedArgs = parseCmdLine(argc, argv, parserCallback, &userData);
	// luego de esta llamada, NECESARIAMENTE debe haberse registrado el numero de pc en userData,
	// y si se parseo mas de un argumento, el otro tiene que haber sido si o si "iniciar".
	// si se recibieron mas argumentos (por ejemplo, varios numeros, o muchas veces "iniciar"): error

	if (userData[1] != 0 && (parsedArgs == 1 || (parsedArgs == 2 && userData[2] == 1))) {
		myId = userData[1]; //guardo en que pc estoy

		switch (parsedArgs) {
		case 2:
			mode = MAKING;
			break;

		case 1:
			mode = LISTENING;
			break;
		}
	}
	else
		return EXIT_FAILURE;

	while (mode != FINISHED) { //repetir hasta que el usuario apriete escape cuando tiene que determinar la nueva secuencia
		switch (mode) {
		case MAKING: {
			do {
				isValid = iniciar(nPCs, YouGo);	//recibir input del usuario hasta que me lo de bien
			} while (isValid == false);

			mode = (YouGo[2] == myId) ? PLAYING : SENDING; //si la primera compu soy yo, reproducir la animacion. si no, mandar
			if (mode == PLAYING)
			{
				YouGo[1]++;
			}
		}
					 break;

		case SENDING: {
			cliente c;
			char nextIP[IP_SIZE];
			read_IPs(IPS_FILE, nextIP, ++YouGo[1]);
			//aumentar el contador de compus y conseguir la IP de la compu que viene ahora

			c.ConectToServer(nextIP, PORT);
			c.sendData(YouGo, nPCs + 2);			//mandar el paquete

			mode = (YouGo[0] == 0) ? FINISHED : LISTENING;
			//si el paquete que mande era que termino, ya esta. si no, espero el proximo paquete
		}
					  break;

		case PLAYING: {
			if (YouGo[0] != 0) { //si hay una animacion, reproducirla
				if (!g->setupDisplay())
					return EXIT_FAILURE;

				a[YouGo[0] - 'a'].play();	//en el primer elemento esta la animacion, la reproduzco
				g->destroyDisplay();
				mode = (YouGo[1] == nPCs) ? MAKING : SENDING;
				//si soy la ultima compu, armar la nueva secuencia. si no, mandar el paquete.
			}
			else
				mode = (YouGo[1] == nPCs) ? FINISHED : SENDING;
			//si no habia animacion, me fijo si tengo que avisarle a las otras compus que ya esta o si fui la ultima
		}
					  break;

		case LISTENING: { //esperando a recibir el paquete YouGo
			servidor s;
			s.waitForCliente();	//quedarse aca hasta que se conecte con el que le va a mandar el paquete
			s.receiveDataForCliente(YouGo, nPCs + 2);	//recibir YouGo (asumo que me lo mandan bien)

			if (YouGo[0] != 0)
				mode = PLAYING; //si hay una animacion, reproducirla
			else if (YouGo[1] != nPCs)
				mode = SENDING;	//si no hay una animacion y no soy la ultima compu, mandar que hay que terminar
			else
				mode = FINISHED;//soy la ultima y no hay animacion: listo
		}
						break;
		}
	}
	return EXIT_SUCCESS;
}



int32_t	parserCallback(char * key, char * value, void * userData)
{
	int32_t valid = false;
	if (key == NULL) {
		unsigned int * ud = (unsigned int *)userData;

		if (!strcmp(value, "iniciar")) {
			ud[2] = 1;
			valid = true;
		}
		else {
			int isUnsInt;
			unsigned int valueNumber = getUnsInt(value, &isUnsInt);

			if (isUnsInt == true && (valueNumber > 0 && valueNumber <= ud[0])) {
				ud[1] = valueNumber;
				valid = true;
			}
		}
	}

	return valid;
}


bool initialize(Graphic ** g, Animation *a)
{ //este bloque esta por separado para que las variables se destruyan una vez construido graphic y animations
	uint32_t nImages[N_ANIMATIONS] = { A_IMGS, B_IMGS, C_IMGS, D_IMGS, E_IMGS, F_IMGS };
	char * path[N_ANIMATIONS] = { A_PATH, B_PATH, C_PATH, D_PATH, E_PATH, F_PATH };
	uint32_t id[N_ANIMATIONS] = { A_ID, B_ID, C_ID, D_ID, E_ID, F_ID };
	double frameRate[N_ANIMATIONS] = { A_FR, B_FR, C_FR, D_FR, E_FR, F_FR };
	uint32_t moveInFrame[N_ANIMATIONS] = { A_STEP, B_STEP, C_STEP, D_STEP, E_STEP, F_STEP };
	int32_t speed[N_ANIMATIONS] = { A_SPEED, B_SPEED, C_SPEED, D_SPEED, E_SPEED, F_SPEED };
	uint32_t width[N_ANIMATIONS] = { A_WIDTH, B_WIDTH, C_WIDTH, D_WIDTH, E_WIDTH, F_WIDTH };
	uint32_t height[N_ANIMATIONS] = { A_HEIGHT, B_HEIGHT, C_HEIGHT, D_HEIGHT, E_HEIGHT, F_HEIGHT };

	char imgPath[N_IMGS][MAX_STRLEN];
	char * imgFiles[N_IMGS];
	unsigned int k = 0;
	char aux[3] = { '\0', '\0', '\0' };

	for (unsigned int i = 0; i < N_ANIMATIONS; i++) { //obtener el path de cada imagen
		for (unsigned int j = 1; j <= nImages[i]; j++) {
			imgFiles[k] = imgPath[k];
			_snprintf_s(aux, 3, "%d", j);
			strcpy_s(imgPath[k], MAX_STRLEN, path[i]);
			strcat_s(imgPath[k], MAX_STRLEN, aux);
			strcat_s(imgPath[k], MAX_STRLEN, ".png");

			k++;
		}
	}

	try {
		*g = new Graphic(N_IMGS, imgFiles);	//inicializar la parte grafica
	}
	catch (bad_alloc& error) {
		cerr << "bad_alloc caught: " << error.what() << endl;
	}

	if (!(*g)->isValid())
		return false;

	for (unsigned int i = 0; i < N_ANIMATIONS; i++) { //inicializar las animaciones
		a[i].setup(nImages[i], id[i], width[i], height[i], frameRate[i], moveInFrame[i], speed[i], *g);
	}	//g se guardara solo la primera vez y luego se ignorara, ya que es una variable estatica de animation

	return true;
}

