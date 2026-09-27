/*PROGRAMA CIERRE*/
/*Este programa lee archivos de tipo comandas_dd-mm-aaaa.dat*/

#include <iostream>

//REGISTRO COMANDA
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//Prototipos de Funcion
void calcularDias(char fechaHoy[], char nombresArchivos[7][30], int& cantidadArchivos);
void armarComanda(char fechaHoy[], char comandaBuscada[]);
void apareo(const char* nomA, const char* nomB, const char* nomC);

int main(int argc, char** argv) {
	
	char fechaHoy[11];
	cout << "Ingrese la fecha de hoy (DD-MM-AAAA): ";
	cin.getline(fechaHoy, 11);
	
	char comandaBuscada[30];
	armarComanda(fechaHoy, comandaBuscada);
	
	FILE* archivo = fopen(comandaBuscada, "rb");
	
	if (archivo == NULL)
	{
	    cout << "No se encontro el archivo de hoy. No se puede cerrar la semana." << endl;
	    cout << "Archivo buscado: " << comandaBuscada << endl;
	}
	else
	{
	    fclose(archivo);
	    
	    char nombresArchivos[7][30];
	    int cantidadArchivos = 0;
	    
	    // Guardar el archivo de hoy
	    int j = 0;
	    
	    while (comandaBuscada[j] != '\0')
	    {
	        nombresArchivos[cantidadArchivos][j] = comandaBuscada[j];
	        j++;
	    }
	    
	    nombresArchivos[cantidadArchivos][j] = '\0';
	    cantidadArchivos++;
	    
	    // Buscar los 6 dias anteriores
	    calcularDias(fechaHoy, nombresArchivos, cantidadArchivos);
	    
	    //COMBINAR ARCHIVOS... (pendiente)
	}
	return 0;
}

//Desarrollo de Funcion armarComanda()...
void armarComanda(char fechaHoy[], char comandaBuscada[]){
    
	int i = 0;
    comandaBuscada[i++] = 'c';
    comandaBuscada[i++] = 'o';
    comandaBuscada[i++] = 'm';
    comandaBuscada[i++] = 'a';
    comandaBuscada[i++] = 'n';
    comandaBuscada[i++] = 'd';
    comandaBuscada[i++] = 'a';
    comandaBuscada[i++] = 's';
    comandaBuscada[i++] = '_';

    for (int j = 0; j < 10; j++)
    {
        comandaBuscada[i++] = fechaHoy[j];
    }

    comandaBuscada[i++] = '.';
    comandaBuscada[i++] = 'd';
    comandaBuscada[i++] = 'a';
    comandaBuscada[i++] = 't';
    comandaBuscada[i] = '\0';
}

//Desarrollo de Funcion calcularDias()...
void calcularDias(char fechaHoy[], char nombresArchivos[7][30], int& cantidadArchivos)
{
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

        if (archivo != NULL)
        {
            int j = 0;

            while (comandaBuscada[j] != '\0')
            {
                nombresArchivos[cantidadArchivos][j] = comandaBuscada[j];
                j++;
            }

            nombresArchivos[cantidadArchivos][j] = '\0';

            cantidadArchivos++;

            fclose(archivo);
        }
    }
}

//Desarrollo de funcion...(concepto)
void apareo(const char* nomA, const char* nomB, const char* nomC) {
    
	FILE* a = fopen(nomA, "rb");
    FILE* b = fopen(nomB, "rb");
    FILE* c = fopen(nomC, "wb");

    if (a == NULL || b == NULL || c == NULL) {
        cout << "Error al abrir los archivos." << endl;
        return;
    }

    Comanda ca, cb;

    int la = fread(&ca, sizeof(Comanda), 1, a);
    int lb = fread(&cb, sizeof(Comanda), 1, b);

    while (la == 1 && lb == 1) {

        if (ca.idMozo < cb.idMozo) {
            fwrite(&ca, sizeof(Comanda), 1, c);
            la = fread(&ca, sizeof(Comanda), 1, a);
        }
        else {
            fwrite(&cb, sizeof(Comanda), 1, c);
            lb = fread(&cb, sizeof(Comanda), 1, b);
        }
    }

    while (la == 1) {
        fwrite(&ca, sizeof(Comanda), 1, c);
        la = fread(&ca, sizeof(Comanda), 1, a);
    }

    while (lb == 1) {
        fwrite(&cb, sizeof(Comanda), 1, c);
        lb = fread(&cb, sizeof(Comanda), 1, b);
    }

    fclose(a);
    fclose(b);
    fclose(c);
}
