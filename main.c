#include <stdio.h>
#include <stdlib.h>
#define maxArregloBeneficiarios 13
/*
SEBASTIAN CAHUANA
CRISTOBAL CAMERON
*/
struct SistemaBonos{
    struct ListaBeneficiarios *benef; //LSE
    struct NodoGenerico *abbBonos;
};
struct ListaBeneficiarios{
    struct NodoGenerico *headBeneficiarios;
};
struct NodoGenerico{
    struct DatoGenerico *dato;
    struct NodoGenerico *ref1, *ref2;
};
struct DatoGenerico{
    struct Bono* bono; //SOLO SE INGRESA POR EL LADO DEL ABB BONOS
    struct Beneficiario *beneficiario; //SOLO SE INGRESA POR EL LADO DE LA LISTA
};
struct Bono{
    int codigo; //con este codigo se ordena el arbol
    struct Beneficiario **beneficiarios; //array no compacto
};
struct Beneficiario{
    int id;
    char *nombre;
};

int buscarIdEnArregloBeneficiarios(struct Beneficiario **arregloBeneficiarios, int idPersona){
    int i ;
    for (i = 0 ; i < maxArregloBeneficiarios ; i++){
        if (arregloBeneficiarios[i] != NULL && arregloBeneficiarios[i]->id == idPersona)
            return 1;
    }
    return 0;
}

void calculaCantidadBonos(struct NodoGenerico *abbBonos,int idPersona,int *cantidadBonos){
    if (abbBonos){
        if (buscarIdEnArregloBeneficiarios(abbBonos->dato->bono->beneficiarios,idPersona) == 1)
            (*cantidadBonos)++;
        calculaCantidadBonos(abbBonos->ref1,idPersona,cantidadBonos);
        calculaCantidadBonos(abbBonos->ref2,idPersona,cantidadBonos);
    }
    return;
}

void calcularDatos(struct NodoGenerico *raiz, struct NodoGenerico *headBeneficiarios,double *cantidadBeneficiados, int *cantidadBonos) {
    while (headBeneficiarios){
        if (headBeneficiarios->dato->beneficiario){
            (*cantidadBeneficiados)++;
            calculaCantidadBonos(raiz,headBeneficiarios->dato->beneficiario->id,cantidadBonos);
        }
        headBeneficiarios=headBeneficiarios->ref1;
    }
}

void insertarEnAbbMasBeneficiados(struct NodoGenerico **abbMasBeneficiados, struct NodoGenerico *nodoInsertar){
    //el nodoInsertar viene del lado de la lista
    if (!(*abbMasBeneficiados)){ //si el nodo del abb nuevo es null
        (*abbMasBeneficiados) = (struct NodoGenerico *) malloc (sizeof(struct NodoGenerico));
        (*abbMasBeneficiados) = nodoInsertar;
        (*abbMasBeneficiados)->ref1 = (*abbMasBeneficiados)->ref2 = NULL;
    }else{
        if (nodoInsertar->dato->beneficiario->id  != (*abbMasBeneficiados)->dato->beneficiario->id){
            if (nodoInsertar->dato->beneficiario->id < (*abbMasBeneficiados)->dato->beneficiario->id)
                insertarEnAbbMasBeneficiados(&((*abbMasBeneficiados)->ref1), nodoInsertar);
            else
                insertarEnAbbMasBeneficiados(&((*abbMasBeneficiados)->ref2), nodoInsertar);
        }
    }
}

void agregarDatosAabbMasBeneficiados(struct NodoGenerico **abbMasBeneficiados,struct NodoGenerico *headBeneficiarios,struct NodoGenerico *AbbBonos, double promedio){
    int cantidadBonos;
    while (headBeneficiarios){
        //tiene la cantidad de bonos de ese beneficiario.
        cantidadBonos = 0 ;
        calculaCantidadBonos(AbbBonos,headBeneficiarios->dato->beneficiario->id,&cantidadBonos);
        if (cantidadBonos > promedio)
            insertarEnAbbMasBeneficiados(abbMasBeneficiados,headBeneficiarios);
        headBeneficiarios=headBeneficiarios->ref1;
    }
}
struct NodoGenerico *abbMasBeneficiados (struct SistemaBonos *SB){
    struct NodoGenerico *abbMasBeneficiados = NULL;
    double cantidadBeneficiados = 0 ;
    int cantidadBonos = 0;
    calcularDatos(SB->abbBonos,SB->benef->headBeneficiarios,&cantidadBeneficiados,&cantidadBonos);
    if (cantidadBeneficiados != 0)
        agregarDatosAabbMasBeneficiados(&abbMasBeneficiados,SB->benef->headBeneficiarios,SB->abbBonos,(double)cantidadBonos / cantidadBeneficiados);
    return abbMasBeneficiados;
}
int main() {
    return 11;
}
