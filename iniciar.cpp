
#include "iniciar.h"
using namespace std;
/* iniciar
 *
 * Funcion que se encarga de recibir y verificar los parametros del usuario. 
 *
 * Recibe: el numero de direcciones IP y un puntero al arreglo donde se almacenara la data necesaria
 * para realizar la simulacion.
 * 
 * Devuelve: true si los datos ingresados son correctos o false si hubo algun error.
 */


bool iniciar (unsigned int i, char* data)
{
    
    //string s = std::to_string(i);
	if (i == 0)
		return false;

	char * tmp_arr;
	//char * temp;

	try {
		tmp_arr = new char [i];// donde se almacenaran los datos del usuario temporalmente hasta quelos mismos esten verificados.
	}catch (bad_alloc& error) {
		cerr << "bad_alloc caught: " << error.what() << endl;
	}
	
    
    //Para elegir la animacion
    cout << "Bienvenido! Porfavor ingrese la letra correspondiente a la animacion que desea reproducir" <<"\n"<< "A. Cat Running"<<"\n"<< "B. Explotion 1"<<"\n"<< "C. Explotion 2 "<<"\n"<< "D. Homer Dance"<<"\n"<< "E. Super Mario"<<"\n"<< "F. Sonic" <<"\n"<<"Si desea salir ingrese 'Q'."<<"\n"<<endl;
    
    string Letra;//se guardara letra ingresada
    cin >> Letra; //Se lee la letra ingresada
    
    
    /*
    //Convierte string a arreglo char*
    try {
		temp = new char[Letra.size() + 1];
	}catch (bad_alloc& error) {
		cerr << "bad_alloc caught: " << error.what() << endl;
	}
	
	
	copy(Letra.begin(), Letra.end(), temp);// copia lo que es ingresado por el usuario a un char *
    temp[Letra.size()] = '\0'; // terminador
    */
    
    if (Letra.size()>1) //chequea que el usuario haya ingresado una sola letra
    {
        cerr<<"Error: opcion no valida. Valores posibles A, B, C, D, E y F, o Q en caso de que desee salir."<< "\n"<< endl;
        return false;
    }
     
    if ((Letra[0]>='A'&& Letra[0]<='F') || (Letra[0]<= 'f'&& Letra [0]>='a')) //chequea que la letra elegida este dentro de las opciones
    {
        data [0]= tolower(Letra[0]); // si es correcta, la almacena en el primer elemento de data.
	data [1]= 0; //Count = 0
    }
    else if (Letra[0] == 'Q' || Letra[0]== 'q') // si el usuario desea salir
    {
        data [0]= 0; //Letra = 0
        return true;
    }
    
    else // si es incorrecta, devuelve false
    {
        cerr << "Error: opcion no valida. Valores posibles A, B, C, D, E y F, o Q en caso de que desee salir."<< endl;
        return false;
    }
    
    //delete [] temp;
	
    //Para  elegir el orden de las maquinas en el que aparecera la animacion
    cout << "Porfavor elija el orden de las maquinas en que desea que se reproduzca la animacion" <<"\n"<<"El total de maquinas conectadas es de: "<< i <<"\n"<<"Asegurese de colocar todos los numeros del 1 al "<< i<<" sin repetir\n"<< endl;
    
    for (int n=0; n<i; n++)
    {
        string num;//En esta variable estará almacenado el numero ingresado.
        cin >> num; //Se lee el numero
        
        int tmp = stoi(num); // convirto el string a int para verificarlo
        
        if (tmp> i || tmp<1) // chequeo que el numero este dentro del rango permitido ( de 1 a la cantidad de maquinas que haya)
        {
            cerr<<"Error: el numero "<< tmp << " no pertenece al rango permitido (de 1 hasta "<<i <<")\n"<<endl;
            return false; // si no esta dentro del rango devuelve false
        }
        
        else // si el numero ingresado es correcto, se almacena en el arreglo temporal
        {
            tmp_arr[n]=tmp;
        }
        
    }
    
    // Una vez que el usuario ingreso todos los numeros, chequeo que esten todos y sin repetir.
    
    bool flag= false; // para indicar si un numero fue ingresado por el usuario o no.
    
    
    for (int j=1; j<=i;j++)
    {
        for (int n=0; n<i; n++)
        {
            if (tmp_arr[n]==j && flag== false)
                flag = true;// encontre el numero
        }
        
        if (flag== false)// falta un numero
        {
            cerr<<"Error: Falto el numero "<<j<<endl;
            return false;
        }
        
        data [j+1]= tmp_arr[j-1];// guardo el numero data
        flag=false;
        
    }
    delete [] tmp_arr;
    return true;
}
