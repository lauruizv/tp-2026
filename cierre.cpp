#include <iostream>
#include <cstring>

using namespace std;

/*PROGRAMA CIERRE:
Este programa lee archivos de tipo comandas_dd-mm-aaaa.dat
y cierra la semana si y solo si el archivo de la fecha de hoy existe, tomando los MAX_DIAS previos y omitiendo los faltantes*/

//LEE-------------------------->
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

//Constantes-----------------------
const int MAX_COMANDAS = 100;

const int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//Prototipos de Funcion
void mostrarComandas(const char* nombre);
void ordenarComandas(Comanda vec[], int len);
void obtenerComandas(const char* nombreArchivo, Comanda comandas[], int& cantidadComandas);

bool esFinDeSemana(char fechaHoy[]);
void calcularDias(char fechaHoy[], char nombresArchivos[7][30], int& cantidadArchivos);
void armarComanda(char fecha[], char comanda[]);

int main(int argc, char** argv) {
	
	cout << "---------------CIERRE---------------" << endl;
	
	char fechaHoy[11];
	cout << "Ingrese la fecha de CIERRE (DD-MM-AAAA): ";
	cin.getline(fechaHoy, 11);
	
	if (!esFinDeSemana(fechaHoy)) {
	    cout << "La fecha ingresada no corresponde al ultimo dia de una semana." << endl;
	    return 0;
	}
	
	char comandaBuscada[30];
	
	armarComanda(fechaHoy, comandaBuscada);
	
	FILE* archivo = fopen(comandaBuscada, "rb");
	
	if (archivo == NULL)
	{
	    cout << "No se encontro el archivo. No se puede cerrar la semana." << endl;
	    cout << "Archivo buscado: " << comandaBuscada << endl;
	}
	else
	{
	    fclose(archivo);
	    
	    char nombresArchivos[7][30];
	    int cantidadArchivos = 0;
	    
	    // Guardar el nombre del archivo de hoy
	    strcpy(nombresArchivos[cantidadArchivos], comandaBuscada);
	    cantidadArchivos++;
	    
	    // Buscar los MAX_DIAS anteriores. Los guarda en nombresArchivos[][]
	    calcularDias(fechaHoy, nombresArchivos, cantidadArchivos);
	    
	    //JUNTAR PLANILLAS... 
	    Comanda comandas[MAX_COMANDAS];
	    int cantComandas = 0;
		
		for (int i = cantidadArchivos-1; i >= 0; i--){
		    obtenerComandas(nombresArchivos[i], comandas, cantComandas);
		}
		
		ordenarComandas(comandas, cantComandas);
		
		//GRABAR datos/comandas_semana_sX-mm.dat
		
	    
	    cout << "\n\n---------------CIERRE---------------" << endl;
	    
	}
	return 0;
}

//COMANDAS: Recorrer
void obtenerComandas(const char* nombre, Comanda arr[], int& len){
	
	FILE* archivoComandas = fopen(nombre, "rb");
	if (archivoComandas == NULL) return;
	
	Comanda r;
	
	while (fread(&r, sizeof(Comanda), 1, archivoComandas) == 1) {
		arr[len] = r;
		len++;
	}
	fclose(archivoComandas);
}

//COMANDAS: Ordenamiento por Insercion
void ordenarComandas(Comanda vec[], int len) {
	for (int i = 1; i < len; i++) {
		Comanda clave = vec[i];	// elemento a insertar en la parte ordenada
		int j = i - 1;
		// Desplazar a la derecha todos los elementos mayores que clave
		while (j >= 0 && vec[j].idMozo > clave.idMozo) {
			vec[j + 1] = vec[j];
			j--;
		}
		vec[j + 1] = clave; // insertar clave en su posición correcta
	}
}

//----------FUNCIONES AUXILIARES----------

//Desarrollo de Funcion armarComanda()...
void armarComanda(char fecha[], char comanda[]){
    
	strcpy(comanda, "datos/comandas_");
	strcat(comanda, fecha);
	strcat(comanda, ".dat");
}

//Desarrollo de Funcion esFinDeSemana()...
bool esFinDeSemana(char fechaHoy[]) {

    int dia = (fechaHoy[0] - '0') * 10 + (fechaHoy[1] - '0');
    int mes = (fechaHoy[3] - '0') * 10 + (fechaHoy[4] - '0');

    if (dia % 7 == 0)
        return true;

    if (dia == diasMes[mes - 1])
        return true;

    return false;
}

//Desarrollo de Funcion calcularDias()...
void calcularDias(char fechaHoy[], char nombresArchivos[7][30], int& cantidadArchivos){
    
	int dia = (fechaHoy[0] - '0') * 10 + (fechaHoy[1] - '0');

    int mes = (fechaHoy[3] - '0') * 10 + (fechaHoy[4] - '0');

    int anio = (fechaHoy[6] - '0') * 1000
             + (fechaHoy[7] - '0') * 100
             + (fechaHoy[8] - '0') * 10
             + (fechaHoy[9] - '0');

    for (int i = 0; i < 6; i++)
    {
        dia--;

        if (dia == 0)
        {
            mes--;

            if (mes == 0)
            {
                mes = 12;
                anio--;
            }

            dia = diasMes[mes - 1];
        }

        char fechaAnterior[11];

        fechaAnterior[0] = dia / 10 + '0';
        fechaAnterior[1] = dia % 10 + '0';
        fechaAnterior[2] = '-';

        fechaAnterior[3] = mes / 10 + '0';
        fechaAnterior[4] = mes % 10 + '0';
        fechaAnterior[5] = '-';

        fechaAnterior[6] = anio / 1000 + '0';
        fechaAnterior[7] = (anio / 100) % 10 + '0';
        fechaAnterior[8] = (anio / 10) % 10 + '0';
        fechaAnterior[9] = anio % 10 + '0';

        fechaAnterior[10] = '\0';

        char comandaBuscada[30];

        armarComanda(fechaAnterior, comandaBuscada);

        FILE* archivo = fopen(comandaBuscada, "rb");

        // Si un archivo no existe, lo omite
		if (archivo != NULL)
        {
            strcpy(nombresArchivos[cantidadArchivos], comandaBuscada);
            cantidadArchivos++;

            fclose(archivo);
        }
    }
}

//PRUEBAS----------------------------------------------------------------------
//Recorrer comandas_dd-mm-aaaa.dat
void mostrarComandas(const char* nombre){
	FILE* archivoComandas = fopen(nombre, "rb");
	if (archivoComandas == NULL) return;
	Comanda r;
	while (fread(&r, sizeof(Comanda), 1, archivoComandas) == 1) {
		cout << r.idMozo << " - " << r.codigoProducto << " - " << r.cantidad << " - " << r.comision << endl;
	}
	fclose(archivoComandas);
}
