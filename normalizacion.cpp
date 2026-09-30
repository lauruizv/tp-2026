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

//La Comision de cada venta es del 10% de lo vendido
const float TASA_COMISION = 0.10f;

//-----------------------------|


int main(int argc, char** argv){
	
	return 0;
}
