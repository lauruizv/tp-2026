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

struct ComandaAux{
	char fecha[11];	//Registro ampliado para su trabajo en memoria
	int idMozo;
	int codigoProducto;
	int cantidad;
	float comision;
};

const char inventario[] = "datos/inventario.dat";
const char comandasHistoricas[] = "datos/comandas_historicas.dat";

const int NUM_CORRIMIENTO = 3;
const int MAX_MOZOS = 1000;

//-----------------------------|
//Prototipos de Funcion
template <typename T>
void crearArchivo(const char* nombre, T arr[], int len);

void verInventario();
void actualizarStock(const char* nombre, int claveBuscada, int cantidad);

void mostrarMozos();
void generarClave(Mozo& mozo);
int buscarMozo(Mozo arr[], int len, const char* nombre);
void agregarMozo(Mozo arr[], int& len, int max, Mozo nuevo);
void obtenerMozos(const char* nombre, Mozo arr[], int& len);

void mostrarComandas(const char* nombre);
void insertarComandaOrdenada(ComandaAux vec[], int &len, ComandaAux valor);
void insertarComanda(ComandaAux vec[], int &len, ComandaAux valor, int pos);
void obtenerComandas(const char* nombre, ComandaAux comandas[], Mozo mozos[], int lenMozos, int& lenComandas);
void generarArchivosComandas(ComandaAux comandas[], int cantComandas);
int contarComandasHistoricas(const char* nombre);


int main(int argc, char** argv){
	
	cout << "\n\n-------------NORMALIZADO-------------" << endl;
	Mozo mozos[MAX_MOZOS];
	int cantMozos = 0;
	obtenerMozos(comandasHistoricas, mozos, cantMozos);
	
	crearArchivo("datos/mozos.dat", mozos, cantMozos);
	
	//Cuento las comandas del historico y creo el array con ese tamaño exacto
        int totalHistoricas = contarComandasHistoricas(comandasHistoricas);
        ComandaAux* comandas = new ComandaAux[totalHistoricas];
    int cantComandas = 0;
    obtenerComandas(comandasHistoricas, comandas, mozos, cantMozos, cantComandas);

    //Crear archivos comandas_dd-mm-aaaa.dat y Actualizar inventario
        if (cantComandas > 0) {
                generarArchivosComandas(comandas, cantComandas);
        }

        delete[] comandas;      //Libero la memoria del array
	
	cout << "\n\n-------------NORMALIZADO-------------" << endl;
	
	return 0;
}

/*---------------------------------------------------------------------------------*/
//CREAR ARCHIVO: Genera desde cero un archivo: si ya existe, lo sobrescribe con el array normalizado.
template <typename T>
void crearArchivo(const char* nombre, T arr[], int len) {
    FILE* f = fopen(nombre, "wb");

    if (f == NULL) {
        cout << "No se pudo crear, ("<< nombre << ")."<< endl;
        return;
    }

    for (int i = 0; i < len; i++) {
        fwrite(&arr[i], sizeof(T), 1, f);
    }

    fclose(f);
    cout << "Archivo " << nombre << " creado." << endl;
}

/*---------------------------------------------------------------------------------*/

//INVENTARIO - Editar Registro in situ-----------------------------------------
//Descuenta en 'cantidad' el stock de un registro y lo sobreescribe
void actualizarStock(const char* nombre, int claveBuscada, int cantidad) {
	FILE* f = fopen(nombre, "rb+");
	if (f == NULL) return;
	Producto p;
	bool encontrado = false;
	while (!encontrado && fread(&p, sizeof(Producto), 1, f) == 1) {
		if (p.codigo == claveBuscada) {
			encontrado = true;
			p.stockActual -= cantidad;
			if (p.stockActual < 0){
				p.stockActual = 0;
			}
			// el puntero quedó en el SIGUIENTE registro -> retrocedo uno
			fseek(f, -(long)sizeof(Producto), SEEK_CUR);
			fwrite(&p, sizeof(Producto), 1, f);
			cout << "	Producto " << p.descripcion << " (" << p.codigo << ") Actualizado."<< endl;
		}
	}
	if (!encontrado) cout << "No se encontro el Producto." << endl;
	fclose(f);
 }


//------------COMANDAS AUXILIARES-----------------------
/*{ char fecha[11]; int idMozo; int codigoProducto; int cantidad; float comision; };*/

//Cuenta cuantas comandas tiene el archivo: tamaño total en bytes / tamaño de un registro
int contarComandasHistoricas(const char* nombre) {
        FILE* f = fopen(nombre, "rb");
        if (f == NULL) return 0;
        fseek(f, 0, SEEK_END);
        long totalBytes = ftell(f);
        fclose(f);
        return totalBytes / sizeof(ComandaHistorica);
}

void generarArchivosComandas(ComandaAux comandas[], int cantComandas){
	char fechaAnterior[11];
	strcpy(fechaAnterior, comandas[0].fecha);
	
	FILE* f = NULL;
	char nombreArchivo[30];
	
	for (int i = 0; i < cantComandas; i++) {
	
	    // Cambia la fecha
	    if (strcmp(fechaAnterior, comandas[i].fecha) != 0) fclose(f);
	
	    // Si es la primera comanda o cambia la fecha
	    if (i == 0 || strcmp(fechaAnterior, comandas[i].fecha) != 0) {
	
	        strcpy(nombreArchivo, "datos/comandas_");
	        strcat(nombreArchivo, comandas[i].fecha);
	        strcat(nombreArchivo, ".dat");
	
	        f = fopen(nombreArchivo, "wb");
	
	        if (f == NULL) {
	            cout << "No se pudo crear, ("<< nombreArchivo << ")."<< endl;
	            return;
	        }
	
	        cout << "\nArchivo " << nombreArchivo << " creado." << endl;
	        
	    }
	
	    Comanda c;
	    c.idMozo = comandas[i].idMozo;
	    c.codigoProducto = comandas[i].codigoProducto;
	    c.cantidad = comandas[i].cantidad;
	    c.comision = comandas[i].comision;
		
		//Escribir Comanda
		fwrite(&c, sizeof(Comanda), 1, f);
		
		//Actualizar Inventario
		actualizarStock(inventario, c.codigoProducto, c.cantidad);
		
	    strcpy(fechaAnterior, comandas[i].fecha);
	    
	}

	// Cerrar el último archivo
	if (f != NULL) {
	    fclose(f);
	}
}

//Recorre comandasHistoricas.dat e inserta las Comandas Auxiliares ordenadas por fecha e idMozo
//Actualiza Inventario
void obtenerComandas(const char* nombre, ComandaAux comandas[], Mozo mozos[], int lenMozos, int& lenComandas) {
    FILE* f = fopen(nombre, "rb");
    if (f == NULL) return;

    ComandaHistorica r;
    ComandaAux c;

    while (fread(&r, sizeof(ComandaHistorica), 1, f) == 1) {

        int pos = buscarMozo(mozos, lenMozos, r.nombreMozo);

		if (pos != -1) {
			strcpy(c.fecha, r.fecha);
		    c.idMozo = mozos[pos].idMozo;
		    c.codigoProducto = r.codigoProducto;
		    c.cantidad = r.cantidad;
		    c.comision = r.comision;
			
			insertarComandaOrdenada(comandas, lenComandas, c);
			 
		}
		else {
		    cout << "Mozo, " << r.nombreMozo << ", no encontrado al buscar Comanda..." << endl;
		    continue;
		}

    }

    fclose(f);
}

//ARRAY comandas: INSERTAR ORDENADO
void insertarComandaOrdenada(ComandaAux vec[], int& len, ComandaAux valor) {
    int i = 0;

    // Avanzar hasta encontrar la posición correcta
	while (i < len) {
        int compararFechas = strcmp(vec[i].fecha, valor.fecha);

        if (compararFechas > 0) {	// nueva fecha es menor, paro ahi
            break;
        }

        if (compararFechas == 0 && vec[i].idMozo > valor.idMozo) { 	// nueva fecha es igual, paro ahi
            break;
        }

        i++;
    }

    if (i == len) {	// valor es mayor que todos: insertar al final
        vec[len] = valor;
        len++;
    }
    else {	// insertar en posición i desplazando el resto
        insertarComanda(vec, len, valor, i);
    }
}

//ARRAY comandas: INSERTAR
void insertarComanda(ComandaAux vec[], int &len, ComandaAux valor, int pos) {
	// Desplazar hacia la derecha desde el final hasta pos
	for (int i = len; i > pos; i--) {
		vec[i] = vec[i - 1];
	}
	vec[pos] = valor; // colocar el nuevo valor
	len++; // el vector tiene un elemento más
}

//-------------------------------------------

//ARRAY mozos: BUSQUEDA SECUENCIAL
int buscarMozo(Mozo arr[], int len, const char* nombre) {
	int i = 0;
	while (i < len && strcmp(arr[i].nombre, nombre) != 0) {
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

void generarClave(Mozo& mozo) {
    char clave[20];

    sprintf(clave, "%d", mozo.idMozo);

    for (int i = 0; clave[i] != '\0'; i++) {
        clave[i] = clave[i] + NUM_CORRIMIENTO;
    }

    strcpy(mozo.password, clave);
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

//Recorrer mozos.dat
void mostrarMozos() {
	FILE* f = fopen("datos/mozos.dat", "rb");
	if (f == NULL) return;
	Mozo r;
	while (fread(&r, sizeof(Mozo), 1, f) == 1) {
		cout << r.idMozo << " - " << r.nombre << " - " << r.password << " - " << r.totalComision << endl;
	}
	fclose(f);
}

//Recorrer inventario.dat
void verInventario() {
	FILE* f = fopen("datos/inventario.dat", "rb");
	if (f == NULL) return;
	Producto r;
	while (fread(&r, sizeof(Producto), 1, f) == 1) {
		cout << r.codigo << " - " << r.descripcion << " - " << r.precio << " - " << r.stockActual << endl;
	}
	fclose(f);
}
