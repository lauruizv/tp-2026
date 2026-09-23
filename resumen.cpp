#include <iostream>
#include <cstdio>
using namespace std;
void CorteControl(FILE*, archSemana);
int main(){
  
struct Comanda{
  int idMozo;
int codigoProducto;
int cantidad;
float comision;
};
char seguir;
do {
char NomArchi[50] = "comandas_semana_sX-mm.dat";
char sem;
char mes[3];

cout<<"--RESUMEN SEMANAL--"<<endl;
cout<<"Ingrese el numero de semana (1, 2, 3, 4): "; cin>>sem;
cout<<"Ingrese el mes (01 - 12): "; cin>>mes;

NomArchi[17] = sem;
NomArchi[19] = mes[0];
NomArchi[20] = mes[1];


FILE* ArchSemana = fopen(NomArchi, "rb");
if(ArchiComSem==NULL){

  cout<<"ERROR, no se pudo abrir el archivo "<<NomArchi<<endl;
  cout<<"Verifique que la semana ya fue procesada por el programa de cierre " <endl;
}
else{
//funciion y fclose de NomArchi

}

} while (//cond seguir





return 0;}
