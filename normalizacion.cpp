#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

//LEE-------------------------->
struct ComandaHistorica{
	char fecha[11];
	char nombreMozo[50];
	int codigoProducto;
	int cantidad;
	float comision;
};

struct Producto{
	int codigo;
	char descripcion[50];
	float precio;
	int stockActual;
};

//GENERA----------------------->
struct Mozo{
	int idMozo;
	char nombre[50];
	char password[20];
	float totalComision;
};

struct Comanda{
	int idMozo;
	int codigoProducto;
	int cantidad;
	float comision;
};

const char inventario[] = "datos/inventario.dat";
const char comandasHistoricas[] = "datos/comandas_historicas.dat";

const int NUM_CORRIMIENTO = 3;
const float TASA_COMISION = 0.10f;	//La Comision de cada venta es del 10% de lo vendido
const int MAX_MOZOS = 30;

//-----------------------------|

//Prototipos de Funcion
void generarClave(Mozo& mozo);
int buscarMozo(Mozo vec[], int len, const char* nombre);
void agregarMozo(Mozo arr[], int& len, int max, Mozo nuevo);
void obtenerMozos(const char* nombre, Mozo arr[], int& len);

template <typename T>
void crearArchivo(const char* nombre, T arr[], int len);

int main(int argc, char** argv){

	Mozo mozos[MAX_MOZOS];
	int cantMozos = 0;
	
	obtenerMozos(comandasHistoricas, mozos, cantMozos);
	crearArchivo("datos/mozos.dat", mozos, cantMozos);
	
	return 0;
}

//Genera desde cero un archivo: si ya existe, lo sobrescribe con la nueva normalizacion.
template <typename T>
void crearArchivo(const char* nombre, T arr[], int len) {
    FILE* f = fopen(nombre, "wb");

    if (f == NULL) {
        cout << "No se pudo crear." << endl;
        return;
    }

    for (int i = 0; i < len; i++) {
        fwrite(&arr[i], sizeof(T), 1, f);
    }

    fclose(f);
    cout << "Archivo creado." << endl;
}


//ARRAY mozos: BUSQUEDA SECUENCIAL
int buscarMozo(Mozo vec[], int len, const char* nombre) {
	int i = 0;
	while (i < len && strcmp(vec[i].nombre, nombre) != 0) {
		i++;
	}
	if (i == len) {
		return -1; // causa 1: no encontrado
	} else {
		return i; // causa 2: encontrado en posición i
	}
}


//ARRAY mozos: AGREGAR
void agregarMozo(Mozo arr[], int& len, int max, Mozo nuevo){
	if(len < max){
		arr[len] = nuevo;
		len++;
	}
	else{
		cout << "Array de Mozos: LLENO." << endl;
	}
}
	



//Recorre comandasHistoricas.dat y carga el Array de Mozos
void obtenerMozos(const char* nombre, Mozo arr[], int& len) {
	FILE* f = fopen(nombre, "rb");
	if (f == NULL) return;
	ComandaHistorica r;
	while (fread(&r, sizeof(ComandaHistorica), 1, f) == 1) {
		
		int mozoEncontrado = buscarMozo(arr, len, r.nombreMozo);
		if(mozoEncontrado == -1){
			
			//Nuevo Mozo
			Mozo nuevoMozo;
			
			nuevoMozo.idMozo = len + 1;
		    strcpy(nuevoMozo.nombre, r.nombreMozo);
		
		    // generar password a partir del ID
		    generarClave(nuevoMozo);
	
		    nuevoMozo.totalComision = r.comision;
			
			agregarMozo(arr, len, MAX_MOZOS, nuevoMozo); // No Existe: Crear Mozo
			
		}
		else{
			arr[mozoEncontrado].totalComision += r.comision; // Existe: Actualizo Comision
		}	
	}
	fclose(f);
}


//ARRAY mozos (aux): Construye la clave de un mozo que ya existía
void generarClave(Mozo& mozo) {
    char clave[20];

    sprintf(clave, "%d", mozo.idMozo);

    for (int i = 0; clave[i] != '\0'; i++) {
        clave[i] = clave[i] + NUM_CORRIMIENTO;
    }

    strcpy(mozo.password, clave);
}
