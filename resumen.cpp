#include <iostream>
using namespace std;
int main(){
  
struct Comanda{
  int idMozo;
int codigoProducto;
int cantidad;
float comision;
};
  
int TotProd=0;
int ProdXmozo=0;
float ComiXmozo=0;

FILE* ArchiComSem = fopen(NomArchiComSem, "rb");
if(ArchiComSem==NULL){

  cout<<"ERROR, no se pudo abrir el archivo "<<endl;
}

  Comanda ComSemanal;
  
while(fread(&ComSemanal, sizeof(Comanda), 1, ArchiComSem) ==1){
  bool MismoMozo=true;
  
  if(MismoMozo==true){
    ProdXmozo+=ComSemana.cantidad;
    ComiXmozo+=ComSemana.comision;
  }







return 0;}
