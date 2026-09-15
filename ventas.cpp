#include <iostream>
using namespace std;
int main(){

  char fechDia[11];
  char nomArchi[50]="comandas_XXXXXXXXXX.dat";
  

  cout<<" Ingrese la fecha de hoy (DD-MM-AAAA): "; cin>>fechDia;

//condicion para cargar el nombre del archivo y asegurarse de no meter un caracter nulo de la fecha al nombre del archivo
  for(int i=0;i<10 && fechDia[i]!='\0';i++){
    nomArchi[9+i]=fechaDia[i];
  }
//Apertura archivo/crear en caso de no existir
FILE* archivoDia = fopen(nomArchi, "ab");
    if (archivoDia == NULL) {
        cout << "Error al crear/abrir el archivo." << endl;
        return 1;
    }





return 0;
}
