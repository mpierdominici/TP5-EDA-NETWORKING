extern "C" {
#include "parseCmdLine.h"
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
#define N_ANIMATIONS	6

#define IP_SIZE			30 //?????????????????????????????????????????????

enum modes {LISTENING, SENDING, MAKING, FINISHED};


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

bool initialize(Graphic *, Animation *);	//inicializar esas dos cosas con los params definidos arriba
int32_t	parserCallback(char *, char*, void *);

int main (int argc,char *argv[])
{
	char * YouGo;
	bool isValid;
	int mode = LISTENING;

	int nPCs = getNumberOfLines(IPS_FILE); //numero de lineas == numero de compus
	if (nPCs <=0)
		return EXIT_FAILURE;

	if (initialize(g,a) == false)		//inicializar la parte grafica
		return EXIT_FAILURE;

	try {
		YouGo =  new char[nPCs+2];			//YouGo va a ser el bloque de datos que mando y recibo
	} catch (bad_alloc& error) {
		cerr << "bad_alloc caught: " << error.what() << endl;
	}

	int parsedArgs = parseCmdLine(argc, argv, parserCallback, NULL);
	//va a devolver:
	//-1 si hubo error de forma o si habia algun argumento distinto de "iniciar"
	//0 si no habia ningun argumento --> hay que esperar a que la maquina que inicia nos mande YouGo
	//1 si se recibio el argumento "iniciar" --> el usuario tiene que determinar la secuencia y la animacion
	//otra cosa: seria que pusiste muchas veces "iniciar", lo contamos como error
	switch (parsedArgs) {
		case 1:
			mode = MAKING;
		break;

		case 0:	
			mode = LISTENING;	
		break;

		case -1: default:
			return EXIT_FAILURE;
	}
	
	while (mode != FINISHED) { //repetir hasta que el usuario apriete escape cuando tiene que determinar la nueva secuencia
		switch(mode) {
			case LISTENING: { //esperando a recibir el paquete YouGo
				servidor s;
				s.waitForCliente();	//quedarse aca hasta que se conecte con el que le va a mandar el paquete
				s.receiveDataForCliente(YouGo, nPCs+2);	//recibir YouGo (asumo que me lo mandan bien)

				Graphic * g = NULL;
				Animation a [N_ANIMATIONS];
				if (initialize(g,a) != true)
					return EXIT_FAILURE;

				a[YouGo[0]-'A'].play();	//en el primer elemento esta la animacion, la reproduzco

				if (YouGo[1] == nPCs)	
					mode = MAKING;		//si el contador es igual al numero de pc, hay que volver a determinar una secuencia
				else
					mode = SENDING;		//si no, hay que mandar el paquete al siguiente
			}
			break;
			
			case MAKING: {
				do {
					isValid = iniciar(nPCs, YouGo);	//recibir input del usuario hasta que me lo de bien
				} while (isValid == false);

				if (YouGo[1] == 0)	
					mode = FINISHED; //la rutina "iniciar" pone un 0 en el primer elemento si el usuario quiere terminar
				else
					mode = SENDING;  //si el usuario no apreto escape, mandar el nuevo paquete
			}
			break;

			case SENDING: {
				cliente c;
				char nextIP[IP_SIZE];
				read_IPs(IPS_FILE, nextIP, ++YouGo[1]);
				//aumentar el contador de compus y conseguir la IP de la compu que viene ahora
				
				c.ConectToServer(nextIP, "papa");	//PAPA ES EL PUERTO!!! ACA DESPUES VA A HABER OTRA COSA PERO PONELE
				c.sendData(YouGo, nPCs+2);			//mandar el paquete

				mode = LISTENING;					//esperar el proximo paquete
			}
			break;
	}

	return EXIT_SUCCESS;
}


int32_t	parserCallback(char * key, char * value, void * userData)
{
	if (key != NULL || strcmp(value, "iniciar")!= 0)
		return false;
	else
		return true;
}


bool initialize(Graphic * g, Animation *a)
{ //este bloque esta por separado para que las variables se destruyan una vez construido graphic y animations
	uint32_t nImages[N_ANIMATIONS] = {A_IMGS, B_IMGS, C_IMGS, D_IMGS, E_IMGS, F_IMGS};
	char * path[N_ANIMATIONS] = {A_PATH, B_PATH, C_PATH, D_PATH, E_PATH, F_PATH};
	uint32_t id[N_ANIMATIONS] = {A_ID, B_ID, C_ID, D_ID, E_ID, F_ID};
	double frameRate[N_ANIMATIONS] = {A_FR, B_FR, C_FR, D_FR, E_FR, F_FR};
	uint32_t moveInFrame[N_ANIMATIONS] = {A_STEP, B_STEP, C_STEP, D_STEP, E_STEP, F_STEP};
	int32_t speed[N_ANIMATIONS] = {A_SPEED, B_SPEED, C_SPEED, D_SPEED, E_SPEED, F_SPEED};
	uint32_t width[N_ANIMATIONS] = {A_WIDTH, B_WIDTH, C_WIDTH, D_WIDTH, E_WIDTH, F_WIDTH};
	uint32_t height[N_ANIMATIONS] = {A_HEIGHT, B_HEIGHT, C_HEIGHT, D_HEIGHT, E_HEIGHT, F_HEIGHT};

	char imgPath[N_IMGS][MAX_STRLEN];
	char * imgFiles[N_IMGS];
	unsigned int k = 0;
	char aux [3] = { '\0', '\0', '\0' };

	for (unsigned int i = 0; i < N_ANIMATIONS; i++) { //obtener el path de cada imagen
		for (unsigned int j = 1; j<=nImages[i]; j++) {
			imgFiles[k] = imgPath[k];
			_snprintf_s(aux, 3, "%d", j);   	
			strcpy_s(imgPath[k], MAX_STRLEN, path[i]);
			strcat_s(imgPath[k], MAX_STRLEN, aux);
			strcat_s(imgPath[k], MAX_STRLEN, ".png");

			k++;
		}
	}
		
	try {
	g = new Graphic(N_IMGS, imgFiles);	//inicializar la parte grafica
	} catch (bad_alloc& error) {
		cerr << "bad_alloc caught: " << error.what() << endl;
	}

	if (!g->isValid())
		return false;

	for(unsigned int i = 0; i < N_ANIMATIONS; i++) { //inicializar las animaciones
		a[i].setup(nImages[i], id[i], width[i], height[i], frameRate[i], moveInFrame[i], speed[i], g);
	}	//g se guardara solo la primera vez y luego se ignorara, ya que es una variable estatica de animation

	return true;
}
