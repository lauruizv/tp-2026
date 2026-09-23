/*PROGRAMA CIERRE*/
/*Este programa lee archivos de tipo comandas_dd-mm-aaaa.dat*/

#include <iostream>

//REGISTRO COMANDA
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

//Prototipos de Funcion
void apareo(const char* nomA, const char* nomB, const char* nomC);

int main(int argc, char** argv) {
	return 0;
}

//Desarrollo de funcion
void apareo(const char* nomA, const char* nomB, const char* nomC) {
    
	FILE* a = fopen(nomA, "rb");
    FILE* b = fopen(nomB, "rb");
    FILE* c = fopen(nomC, "wb");

    if (a == NULL || b == NULL || c == NULL) {
        cout << "Error al abrir los archivos." << endl;
        return;
    }

    Comanda ca, cb;

    int la = fread(&ca, sizeof(Comanda), 1, a);
    int lb = fread(&cb, sizeof(Comanda), 1, b);

    while (la == 1 && lb == 1) {

        if (ca.idMozo < cb.idMozo) {
            fwrite(&ca, sizeof(Comanda), 1, c);
            la = fread(&ca, sizeof(Comanda), 1, a);
        }
        else {
            fwrite(&cb, sizeof(Comanda), 1, c);
            lb = fread(&cb, sizeof(Comanda), 1, b);
        }
    }

    while (la == 1) {
        fwrite(&ca, sizeof(Comanda), 1, c);
        la = fread(&ca, sizeof(Comanda), 1, a);
    }

    while (lb == 1) {
        fwrite(&cb, sizeof(Comanda), 1, c);
        lb = fread(&cb, sizeof(Comanda), 1, b);
    }

    fclose(a);
    fclose(b);
    fclose(c);
}
