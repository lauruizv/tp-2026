#include <iostream>
#include <cstring>
using namespace std;

//Declaracion de structs
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

struct Producto{
int codigo;
char descripcion[50];
float precio;
int stockActual;
};

int main(){

  char fechDia[11];
  char nomArchi[50]="comandas_XXXXXXXXXX.dat";
  

  cout<<" Ingrese la fecha de hoy (DD-MM-AAAA): "; cin>>fechDia;

//condicion para cargar el nombre del archivo y asegurarse de no meter un caracter nulo de la fecha al nombre del archivo
  for(int i=0;i<10 && fechDia[i]!='\0';i++){
    nomArchi[9+i]=fechaDia[i];
  }
//Apertura archivo/crear en caso de no existir
FILE* ArchiDia = fopen(nomArchi, "ab");
    if (ArchiDia == NULL) {
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
    for(int i=0; claveIngresada[i]!='\0'; i++){
      claveIngresada[i]=claveIngresada[i]+numCorrimiento;
    }
    //Apertura de archivo mozos
    FILE* ArchiMozos = fopen("mozos.dat", "rb");
    if(ArchiMozos==NULL){
      cout<<"Error, no se pudo abrir el archivo de mozos"<<endl;
      return 1;
    }
    bool mozEnc=false;
    bool clavCorr=false;
    Mozo unMozo;
    //recorrido con while fread - verificacion de clave
    while (fread(&unMozo, sizeof(Mozo), 1, ArchiMozos) == 1){

      if(IdMozo==unMozo.idMozo){
        mozEnc=true;
      
      if (strcmp(claveIngresada, unMozo.password) == 0) {
                    clavCorr = true;
                }
        
        break;
      }
      }
      else{ cout<<"Id del mozo incorrecto"<<endl;
      }
    
//cierre archivo mozos
    fclose(ArchiMozos);

    if(!mozEnc){
      cout<<"Numero de mozo no existente "<<endl;}

    else if(!clavCorr){cout<<"La clave ingresada es incorrecta "<<endl;}
      
    else{
      //apertura archivo inventario.dat
      Producto UnProd;
      bool ProdEncontrado=false;
cout<<"--SESION INCIADA--"<<endl;
  
int codProdIng;
int cantIng;
Comanda UnaComanda;
  
  cout<<"Ingrese el codigo del producto: (0 para finalizar el ingreso de ventas) ";
  cin >> codProdIng;
  
  while(codProdIng!=0){
    
    FILE* ArchiInv = fopen("inventario.dat", "rb+");
    if(ArchiInv==NULL){
      cout<<"Error, no se pudo abrir el archivo de mozos"<<endl;
      return 1;
    }
    
  cout<<"Ingrese la cantidad del producto: ";
    cin >> cantIng; 
      //recorrido para buscar prod
      while (fread(&UnProd, sizeof(Producto), 1,ArchiInv) == 1){
        if(UnProd.codigo==codProdIng){
          ProdEncontrado=true;
          if(UnProd.stockActual>=cantIng){
            
          UnaComanda.idMozo = IdMozo;
          UnaComanda.codigoProducto = codProdIng;
          UnaComanda.cantidad = cantIng;
          UnaComanda.comision = (UnProd.precio * cantIng) * 0.10;
             
          fwrite(&UnaComanda, sizeof(Comanda), 1, ArchiDia);

            UnProd.stockActual-=cantIng; 

          fseek(ArchiInv, -sizeof(Producto), SEEK_CUR);
            
          fwrite(&UnProd, sizeof(Producto), 1, ArchiInv);

          break;
          }
          else {cout<<" --ERROR STOCK INSUFICIENTE-- "<<endl;}
      }

      fclose(ArchiInv); }
   
cout << "Ingrese otro codigo de producto (0 para finalizar ventas del mozo): ";
    cin >> codProdIngresado;
  }
  cout<<" --SESION CERRADA-- "<<endl;
}
      cout<<"Ingrese el ID del mozo: (Ingrese 0 para terminar el dia) "; cin>>IdMozo;
    }
return 0;}

