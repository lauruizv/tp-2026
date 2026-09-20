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

  char fechaDia[11];
  char nomArchi[50]="comandas_XXXXXXXXXX.dat";
  

  cout<<" Ingrese la fecha de hoy (DD-MM-AAAA): "; cin>>fechaDia;

//condicion para cargar el nombre del archivo y asegurarse de no meter un caracter nulo de la fecha al nombre del archivo
  for(int i=0;i<10 && fechaDia[i]!='\0';i++){
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
     
//cierre archivo mozos
    fclose(ArchiMozos);

    if(!mozEnc){
      cout<<"Numero de mozo no existente "<<endl;}

    else if(!clavCorr){cout<<"La clave ingresada es incorrecta "<<endl;}
      
    else{
      //apertura archivo inventario.dat
      Producto UnProd;
      
cout<<"--SESION INCIADA--"<<endl;
  
int codProdIng;
int cantIng;
Comanda UnaComanda;
  
  cout<<"Ingrese el codigo del producto: (0 para finalizar el ingreso de ventas) ";
  cin >> codProdIng;
  
  while(codProdIng!=0){
    bool ProdEncontrado=false;
    FILE* ArchiInv = fopen("inventario.dat", "rb+");
    if(ArchiInv==NULL){
      cout<<"Error, no se pudo abrir el archivo de inventario"<<endl;
      break;
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
      }

     fclose(ArchiInv);

        if(!ProdEncontrado){
          cout<<"ERROR, PRODUCTO INEXISTENTE"<<endl;
      }
        cout<<"Ingrese otro codigo de producto (0 para finalizar el ingreso de ventas)";
          cin>>codProdIng;
  }
  cout<<" --SESION CERRADA-- "<<endl;
}
      cout<<"Ingrese el ID del mozo: (Ingrese 0 para terminar el dia) "; cin>>IdMozo;
    }
  fclose(ArchiDia);

  //Pasamos los datos de la comanda del dia a un array de struct para ordenarlos y volverlos a meter al archivo de comanda dia
  Comanda VecComDia[500];
  int cantVentas=0;
  ArchiDia = fopen(nomArchi, "rb");
  if(ArchiDia==NULL){
    cout<<"ERROR, el archivo nose pudo abrir"<<endl; 
    return 1;
  }
  while (fread(&VecComDia[cantVentas], sizeof(Comanda), 1, ArchiDia) == 1) {
        cantVentas++; 
    }
  fclose(ArchiDia);
  //VEC de struct ya copiado
  //ORDENAR por IdMozo
for(int i=0;i<cantVentas-1;i++){
  int posMenor=i;
  for(int j=i+1;j<cantVentas;j++){
    if(VecComDia[j].idMozo<VecComDia[posMenor].idMozo){
      posMenor=j;}
  }
  if (posMenor != i) {
        Comanda aux = VecComDia[i];
        VecComDia[i] = VecComDia[posMenor];
        VecComDia[posMenor] = aux;
    }
}
  //CARGAR VEC DE STRUCTS ORDENADO EN ARCHIVO COMANDAS DIA
  ArchiDia = fopen(nomArchi, "wb");
  fwrite(VecComDia, sizeof(Comanda), cantVentas, ArchiDia);
  fclose(ArchiDia);
  
return 0;}

