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
//ciclo de carga de ventas
  int IdMozo;
  cout<<"Ingrese el ID del mozo: (Ingrese 0 para terminar el dia) "; cin>>IdMozo;
  while(IdMozo!=0){
    char claveIngresada[20];
    cout<<"Ingrese la clave del mozo: "; cin>>claveIngresada;
    //Aplicar corrimiento a la clave 
    const  int numCorrimiento=3;
    for(int i=0;i<3;i++){
      claveIngresada[i]=claveIngresada[i]+numCorrimiento;
    }
    //Apertura de archivo mozos
    FILE* archMozos = fopen("mozos.dat", "rb");
    if(archMozos==NULL){
      cout<<"Error, no se pudo abrir el archivo de mozos"<<endl;
      return 1;
    }
    bool mozEnc=false;
    bool clavCorr=false;
    //recorrido con while fread 




return 0;
}
