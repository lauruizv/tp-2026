#include <iostream>
#include <cstdio>
using namespace std;

void CorteControl(FILE* ArchSemana);

struct Comanda{
  int idMozo;
int codigoProducto;
int cantidad;
float comision;
};

int main(){
  

char seguir;
do {
char NomArchi[50] = "datos/comandas_semana_sX-mm.dat";
char sem;
char mes[3];

cout<<"--RESUMEN SEMANAL--"<<endl;
cout<<"Ingrese el numero de semana (1, 2, 3, 4): "; cin>>sem;
cout<<"Ingrese el mes (01 - 12): "; cin>>mes;

NomArchi[23] = sem;
NomArchi[25] = mes[0];
NomArchi[26] = mes[1];


FILE* ArchSemana = fopen(NomArchi, "rb");
if(ArchSemana==NULL){

  cout<< " ERROR, no se pudo abrir el archivo "<<NomArchi<<endl;
  cout<< " Verifique que la semana ya fue procesada por el programa de cierre " <<endl;
}
else{

  CorteControl(ArchSemana);
  fclose(ArchSemana);

}
  cout<< "Desea consultar otra semana? (S/N): "; 
  cin >> seguir;

} 
  while ( seguir == 'S' || seguir == 's' );
  

return 0;}
void CorteControl(FILE* ArchSemana){
Comanda UnaComanda;
int BuenLec = fread(&UnaComanda, sizeof(Comanda), 1, ArchSemana);
int TotBuf = 0;
while(BuenLec == 1){

int MozAct = UnaComanda.idMozo;
int TotProd = 0;
float TotCom = 0;

while( BuenLec == 1 && UnaComanda.idMozo == MozAct){

TotProd += UnaComanda.cantidad;
TotCom += UnaComanda.comision;

  BuenLec = fread(&UnaComanda, sizeof(Comanda), 1, ArchSemana);
}
TotBuf += TotProd;
  
cout<<"Para el mozo " <<MozAct<<endl;

cout<<" El total de productos vendidos fue: " <<TotProd<<endl;

cout<<" La comision que el corresponde es de: " <<TotCom<<endl;

}
  
  cout<<" --CIERRE SEMANAL-- "<<endl;
  cout<<" El total de productos vendidos por el buffet fue: " <<TotBuf;

}
