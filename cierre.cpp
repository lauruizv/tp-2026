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
const int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//Prototipos de Funcion
void mostrarComandas(const char* nombre);

bool esFinDeSemana(char fechaHoy[]);
void armarSemana(char* cadena, const char* fecha);
void calcularDias(char fechaHoy[], char nombresArchivos[7][30], int& cantidadArchivos);
void armarComanda(char fecha[], char comanda[]);

void copiarArchivo(const char* origen, const char* destino);
void apareo(const char* nomA, const char* nomB, const char* nomC);

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
	    
		char archivoSemanal[32] = "datos/comandas_semana_s";
	    armarSemana(archivoSemanal, fechaHoy);
	    
		/*
			A + B ? temporal1
			temporal1 + C ? temporal2
			temporal2 + D ? temporal1
			temporal1 + E ? temporal2
		*/
		
		char temporal1[20] = "datos/temporal1.dat";
		char temporal2[20] = "datos/temporal2.dat";
		
		remove(archivoSemanal);
		if (cantidadArchivos == 1){
			//	Si solo existe la planilla cierre
			copiarArchivo(nombresArchivos[0], archivoSemanal);
		}
		else{
			// Varios Archivos
			apareo(nombresArchivos[0], nombresArchivos[1], temporal1);
		
			char resultado[20];
			strcpy(resultado, temporal1);
			
			for (int i = 2; i < cantidadArchivos; i++)
			{
			    if (i % 2 == 0)
			    {
			        remove(temporal2);
			        apareo(temporal1, nombresArchivos[i], temporal2);
			        strcpy(resultado, temporal2);
			    }
			    else
			    {
			        remove(temporal1);
			        apareo(temporal2, nombresArchivos[i], temporal1);
			        strcpy(resultado, temporal1);
			    }
			}
			
			rename(resultado, archivoSemanal);	
		}
		
		remove(temporal1);
		remove(temporal2);
		cout << "\nArchivo Creado: " << archivoSemanal << "\n" << endl;
		
	    //mostrarComandas(archivoSemanal);
	    
	    cout << "\n\n---------------CIERRE---------------" << endl;
	    
	}
	return 0;
}

// APAREO
void apareo(const char* nomA, const char* nomB, const char* nomC) {
	
	FILE* a = fopen(nomA, "rb");
	FILE* b = fopen(nomB, "rb");
	FILE* c = fopen(nomC, "wb");
	
	Comanda ra, rb;
	
	int la = fread(&ra, sizeof(Comanda), 1, a);
	int lb = fread(&rb, sizeof(Comanda), 1, b);
	
	while (la == 1 && lb == 1) { // mientras haya en AMBOS
		if (ra.idMozo < rb.idMozo) {
			fwrite(&ra, sizeof(Comanda), 1, c);
			la = fread(&ra, sizeof(Comanda), 1, a); // avanzo A
		} else {
			fwrite(&rb, sizeof(Comanda), 1, c);
			lb = fread(&rb, sizeof(Comanda), 1, b); // avanzo B
		}
	}
	while (la == 1) { // agoto A (si fue el que sobró)
		fwrite(&ra, sizeof(Comanda), 1, c);
		la = fread(&ra, sizeof(Comanda), 1, a);
	}
	while (lb == 1) { // agoto B
		fwrite(&rb, sizeof(Comanda), 1, c);
		lb = fread(&rb, sizeof(Comanda), 1, b);
	}
	fclose(a); fclose(b); fclose(c);
}

//COPIAR ARCHIVO: Recorrer
void copiarArchivo(const char* origen, const char* destino) {
    FILE* a = fopen(origen, "rb");

    if (a == NULL) {
        cout << "Error al abrir el archivo de origen." << endl;
        return;
    }

    FILE* b = fopen(destino, "wb");

    if (b == NULL) {
        cout << "Error al crear el archivo de destino." << endl;
        fclose(a);
        return;
    }

    Comanda r;

    while (fread(&r, sizeof(Comanda), 1, a) == 1) {
        fwrite(&r, sizeof(Comanda), 1, b);
    }

    fclose(a);
    fclose(b);
}
//----------FUNCIONES AUXILIARES----------

//Desarrollo de Funcion armarComanda()...
void armarComanda(char fecha[], char comanda[]){
    
	strcpy(comanda, "datos/comandas_");
	strcat(comanda, fecha);
	strcat(comanda, ".dat");
}

//Desarrollo de Funcion armarSemana()...
void armarSemana(char* cadena, const char* fecha){
	
	char semana;
	
	int i = 0;
	while(cadena[i] != '\0'){
		i++;
	}
	
	int dia = (fecha[0] - '0') * 10 + (fecha[1] - '0');

    if (dia >= 1 && dia <= 7) semana =  '1';
    else if (dia <= 14) semana = '2';
    else if (dia <= 21) semana = '3';
    else if (dia <= 28) semana = '4';
    else semana = '5';
	
	cadena[i] = semana;
	cadena[i + 1] = '\0';
	
	strcat(cadena, "-");
	strncat(cadena, fecha + 3, 2);
	strcat(cadena, ".dat");
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

    int diasABuscar = 6;

	if (dia > 28) {
	    diasABuscar = diasMes[mes - 1] - 29;
	}
	
	for (int i = 0; i < diasABuscar; i++) {
    	
		dia--;
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
		cout << "\t" << r.idMozo << " - " << r.codigoProducto << " - " << r.cantidad << " - " << r.comision << endl;
	}
	fclose(archivoComandas);
}
