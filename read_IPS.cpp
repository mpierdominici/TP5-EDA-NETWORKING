
#include "read_IPS.hpp"

using namespace std;

/* read_IPs
 * 
 * Funcion que lee el numero de linea deseado en un archivo de texto y almacena el contenido 
 * en el buffer que es recibido como parametro.
 *
 * Recibe: el buffer donde se almacena el contenido y el numero de linea que se desea leer 
 * de un archivo.
 *
 * Devuelve: true si no hubo error, false en caso contrario
 */
 

bool read_IPs(char * buffer, int n) {
 
    ifstream directions (ARCHIVO, std::ifstream::binary);
    if (!directions.is_open())
    {
        cerr << "Error opening file";
        return false;
    }
   
    
    char temp[20];
    if (directions)
    {
           if (n==1)
        {
            directions.getline(buffer, ' ');
            return true;
        }
        
        for (int i=1; i<n+1;i++)
        {
            directions.getline (temp, ' ');
        }
        
        directions.getline (buffer,' ');
    }
    
    directions.close();
    return true;
}


/* getNumberOfLines
 *
 * Funcion que cuenta la cantidad de lineas que tiene un archivo de texto.
 * En este caso seria igual al numero de direcciones IPs que hay.
 *
 * Recibe: void
 *
 * Devuelve: El numero de lineas del archivo
 */

int getNumberOfLines (void)
{
    char temp[20];
    unsigned int x=0;
    
    ifstream directions (ARCHIVO, std::ifstream::binary);
    while(!directions.eof())
    {
        directions.getline (temp, ' ');
        x++;
    }
    directions.close();
    return x;
}


