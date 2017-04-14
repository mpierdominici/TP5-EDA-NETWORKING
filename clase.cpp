#include "Graphic.h"
#include "Animation.h"
#include <new>
#include <iostream>
#include <cstdint>
#include <cstdio>


#define N_ANIMATIONS 6

//PATH de los archivos de cada animacion. cada imagen debe estar numerada a partir del 1 (con el numero al final) Y FORMATO PNG
#define A_PATH "Img/Cat Running/Cat Running-F"
#define B_PATH "Img/Explosion 1/Explosion 1-F"
#define C_PATH "Img/Explosion 2/Explosion 2-F"
#define D_PATH "Img/Homer Dance/homerdance-F"
#define E_PATH "Img/Sonic/Sonic Running-F"
#define F_PATH "Img/Super Mario/Super Mario Running-F"
#define BACKGROUND	"Img/background.png"

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

//VELOCIDAD de cada animacion (en px/step)
#define A_SPEED	20
#define B_SPEED 0
#define C_SPEED 0
#define D_SPEED 100
#define E_SPEED 20
#define F_SPEED	70

//NUMERO DE IMAGENES que componen cada animacion
//TIENE QUE SER MENOR QUE 100 PARA CADA UNA!!!
#define A_IMGS	12
#define B_IMGS	8
#define C_IMGS	48
#define D_IMGS	10
#define E_IMGS	10
#define F_IMGS	12

//DIRECCION: 0 para las animaciones que se mueven a la dcha, 1 para la izq, si esta quieto es lo mismo
#define A_DIR	0
#define B_DIR	0
#define C_DIR	0
#define D_DIR	0
#define E_DIR	1
#define F_DIR	0


#if (A_IMGS > 99 || B_IMGS > 99 || C_IMGS > 99 || D_IMGS > 99 || E_IMGS > 99 || F_IMGS > 99 )
#error "Too many images for a single animation"
#endif


//ID DE LA PRIMERA IMAGEN de cada animacion
#define A_ID	1
#define B_ID	A_ID + A_IMGS 
#define C_ID	B_ID + B_IMGS
#define D_ID	C_ID + C_IMGS
#define E_ID	D_ID + D_IMGS
#define F_ID	E_ID + E_IMGS

#define N_IMGS	F_ID + F_IMGS


using namespace std;

int main (int argc,char *argv[])
{
	Graphic * g2 = NULL;
	Animation a [N_ANIMATIONS];

	{ //este bloque esta por separado para que las variables se destruyan una vez construido graphic y animations
		uint32_t nImages[N_ANIMATIONS] = {A_IMGS, B_IMGS, C_IMGS, D_IMGS, E_IMGS, F_IMGS};
		char * path[N_ANIMATIONS] = {A_PATH, B_PATH, C_PATH, D_PATH, E_PATH, F_PATH};
		uint32_t id[N_ANIMATIONS] = {A_ID, B_ID, C_ID, D_ID, E_ID, F_ID};
		double frameRate[N_ANIMATIONS] = {A_FR, B_FR, C_FR, D_FR, E_FR, F_FR};
		uint32_t moveInFrame[N_ANIMATIONS] = {A_STEP, B_STEP, C_STEP, D_STEP, E_STEP, F_STEP};
		uint32_t speed[N_ANIMATIONS] = {A_SPEED, B_SPEED, C_SPEED, D_SPEED, E_SPEED, F_SPEED};
		bool direction[N_ANIMATIONS] = {A_DIR, B_DIR, C_DIR, D_DIR, E_DIR, F_DIR};

		char imgPath[N_IMGS][MAX_STRLEN] = { BACKGROUND };
		char * imgFiles[N_IMGS] = { imgPath[0]};
		unsigned int k = 1;
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
		g2 = new Graphic(N_IMGS, imgFiles);
		} catch (bad_alloc& error) {
			cerr << "bad_alloc caught: " << error.what() << endl;
		}

		if (!g2->isValid())
			return EXIT_FAILURE;

		for(unsigned int i = 0; i < N_ANIMATIONS; i++) {
			a[i].setup(nImages[i], id[i], frameRate[i], moveInFrame[i], speed[i], direction[i], g2);
		}
	}

	for(unsigned int i = 0; i < N_ANIMATIONS; i++) {
		a[i].move(50, 50);
	}
	return EXIT_SUCCESS;
}


//#define N 4 //numero de compus, entre 1 y 255
//
//typedef char byte;
//
//
//byte buffer[N+2];
//
////buffer[0]= de 'A' a 'F' (ascii)
////buffer[1]= count, de 0 a N-1 (binario)
////buffer[2]-buffer[N]= de 1 a N, puerto de cada compu (binario)


//	a.	armar secuencia: que el usuario te diga que animacion y en que orden de las maquinas (solo una maquina por vez)
//	b.	escuchar: esperar un paquete
//	c.	pasar animacion
//	d.	mandar paquete


//el que habla: client; el que escucha: server
//el que escucha solo necesita el puerto, el que habla ip y puerto
//socket: "el puerto del ladode adentro". uno por maquina (como minimo, pero en nuestro caso uno). identificador de la conexion: conjuncion ip/puerto

//EL CLIENTE ES EL QUE MANDA EL YOU GO