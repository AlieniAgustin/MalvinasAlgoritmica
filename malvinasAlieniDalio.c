//Comision 2
//Agustin Alieni, DNI: 46332866
//Damian Dalio, DNI: 45801187

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#define NMax 250
#define MinDNI 5000000

/*Definimos solo los registros Tfecha y TPersona como struct y el resto typedef struct, puesto que
en un principio todos eran typedef struct, pero de esa manera no se leia el archivo de prueba
mandado por ustedes. Por ello solo Tfecha y TPersona son struct.*/
struct Tfecha {
    int dia;
    int mes;
    int anio;
};

struct TPersona {
    int DNI;
    char nombre[20];
    char apellido[20];
    struct Tfecha nacimiento; 
    int edad;
    char ciudad[20]; 
    char ciudadAntes[20]; 
    char DP[30]; 
    char CE[20]; 
    char tel[15]; 
    int VF; 
    struct Tfecha fallecimiento; 
    char beneficio[20]; 
    char CP[10]; 
    char provincia[50]; 
    int fuerza; 
    char destino[50]; 
    char funcion[100]; 
    char grado[20]; 
    char secuelas[200]; 
};

typedef struct{
    char nombre[20],apellido[20],ciudad[20];
    int DNI;
    struct Tfecha fallecimiento;
}TPersonaFallecida;

typedef struct{
    char nombre[20],apellido[20],ciudad[20],DP[30],CE[20];
    struct Tfecha nacimiento;
    int edad, VF;
}TPersonaCiertoMes;

typedef struct{
    struct TPersona a[NMax];
    int cant;
}TData1;

typedef struct{
    TPersonaCiertoMes a[NMax];
    int cant;
}TData2;

typedef struct TNodo1{
    TPersonaFallecida info;
    struct TNodo1 *next;
}TNodo1;

typedef struct TNodo2{
    struct TPersona info;
    struct TNodo2 *next, *back;
}TNodo2;

FILE* archVeteranos;
TData1 asociacionVeteranos;
TData2 asociacionCiertoMes;
TNodo1 *listaFallecidosIni;
TNodo2 *listaFuerzaIni, *listaFuerzaFin;
struct Tfecha fechaActual;
int opcion, ciertoMes, DNI, i, j, valor, fuerzaListar;
struct TPersona persona;
TPersonaCiertoMes personaMes;
char apellidoBuscar[20];

int Comparar(struct TPersona a, struct TPersona b);
int VaciaLSE(TNodo1 *puntIni);
int VaciaLDE(TNodo2 *puntIni, TNodo2 *puntFin);
int DiaSegunMes(struct Tfecha fecha);
int EmpiezaMayusSigueMin(char *cadena, int i);
int ValidarComponente(int limiteInferior, int limiteSuperior, const char *mensaje);
char* MesesC(int mes);
void ValidarCadena(char *cadena, int tamaño, const char *mensaje);
void InicializarLSE(TNodo1 **puntIni);
void InicializarLDE(TNodo2 **puntIni, TNodo2 **puntFin);
void BusquedaSecuencialDNI(TData1 array, int dni, int *pos);
void BusquedaRecursivaApellido(TData1 array, char apellido[20], int *inf, int *sup);
void OrdenarInsercionApellido(TData1 *array);
void OrdenarBurbujaMes(TData2 *array);
void ArchivoAArreglo(FILE* arch, TData1 *array);
void ArregloAArchivo(TData1 array, FILE* *arch);
void CalcularEdad(struct Tfecha nacimiento, struct Tfecha diaActual, int *edad);
void CalcularFecha(struct Tfecha *fecha, struct Tfecha diaActual, int tipoFecha);
void ActualizarEdades(struct Tfecha diaActual, TData1 *array);
void TratamientoDatos(struct Tfecha diaActual, struct TPersona *persona, const char *mensaje);
void CargarPersona(TData1 array, struct Tfecha diaActual, struct TPersona *personax);
void InsertarEnOrden(struct TPersona personax, TData1 *array);
void SuprimirPersona(int dni, TData1 *array);
void MostrarPersonaCompleto(struct TPersona persona);
void ModificarPersona(struct Tfecha diaActual, int dni, TData1 *array);
void MostrarApellido(char apellido[20], TData1 array);
void InsertarCabezaLSE(TPersonaFallecida persona, TNodo1 **puntIni);
void CrearLSEFallecidos(TData1 array, TNodo1 **puntIni);
void MostrarLSE(TNodo1 *puntIni);
void LiberarLSE(TNodo1 **puntIni);
void InsertarColaLDE(struct TPersona persona, TNodo2 **puntIni, TNodo2 **puntFin);
void CrearLDEFuerza(TData1 array, int fuerza, TNodo2 **puntIni, TNodo2 **puntFin);
void MostrarLDE(TNodo2 *puntIni, TNodo2 *puntFin);
void LiberarLDE(TNodo2 **puntIni, TNodo2 **puntFin);
void CrearArregloCiertoMes(int mes, TData1 array1, TData2 *array2);
void MostrarPersonaCiertoMes(TPersonaCiertoMes persona);

int main(){
    ArchivoAArreglo(archVeteranos,&asociacionVeteranos);
    //Obtiene la fecha actual desde el sistema operativo
    time_t t = time(NULL);
    struct tm* tiempo = localtime(&t);
    fechaActual.dia = tiempo->tm_mday;
    fechaActual.mes = tiempo->tm_mon + 1;
    fechaActual.anio = tiempo->tm_year + 1900;
    //Nos aseguramos que esta sea la fecha actual
    printf("¿Esta es la fecha actual? (No = 0 / Si = 1): \n");
    printf("Fecha Actual: %d/%d/%d \n", fechaActual.dia, fechaActual.mes, fechaActual.anio);
    scanf("%d", &opcion);
    if(!opcion){
        printf("\nIngrese la fecha actual \n");
        CalcularFecha(&fechaActual, fechaActual, 1); 
    }
    ActualizarEdades(fechaActual,&asociacionVeteranos);
    do{
        printf("\n-------Menú de Opciones-------\n");
        printf("1. Insertar Persona \n");
        printf("2. Eliminar Persona \n");
        printf("3. Modificar datos de persona \n");
        printf("4. Listar todas las personas \n");
        printf("5. Listar todas las personas según el apellido \n");
        printf("6. Listar todas las personas fallecidas \n");
        printf("7. Listar todas las personas de una fuerza elegida \n");
        printf("8. Listar todas las personas que cumplen en un mes elegido \n");
        printf("9. Guardar información cargada hasta el momento \n"); //se agrega esta opcion por si luego de llevar un rato cargando elementos, por alguna razon se rompe el programa. Asi no se pierde todo el progreso
        printf("10. Salir \n");
        scanf("%d", &opcion);
        if(opcion == 1){
            if(asociacionVeteranos.cant == NMax){   
                printf("\nSe alcanzó el límite de datos que se pueden almacenar \n");
            }else{
                CargarPersona(asociacionVeteranos, fechaActual, &persona);
                InsertarEnOrden(persona,&asociacionVeteranos);
            }   
        }else if(opcion == 2){
            if(asociacionVeteranos.cant == 0){
                printf("\nNo hay datos cargados para eliminar \n");
            }else{
                DNI = ValidarComponente(MinDNI, 100000000, "\nIngrese el DNI que desea eliminar: ");
                SuprimirPersona(DNI, &asociacionVeteranos); 
            }
        } else if(opcion == 3){
            if(asociacionVeteranos.cant == 0){
                printf("\nNo hay datos cargados para modificar \n");
            }else{
                DNI = ValidarComponente(MinDNI, 100000000, "\nIngrese el DNI de la persona que desea modificar: ");
                ModificarPersona(fechaActual, DNI, &asociacionVeteranos);
            }
        } else if(opcion == 4){
            if(asociacionVeteranos.cant == 0){
                printf("\nNo hay datos cargados para mostrar \n");
            }else{
                printf("\n          Listado Completo de los Integrantes     \n");
                for(i = 0; i < asociacionVeteranos.cant ; i++){
                    persona = asociacionVeteranos.a[i];
                    MostrarPersonaCompleto(persona);
                }
            }
        } else if(opcion == 5){
            if(asociacionVeteranos.cant == 0){
                printf("\nNo hay datos cargados para mostrar \n");
            }else{
                i = 0;
                while (getchar() != '\n');
                ValidarCadena(apellidoBuscar,sizeof(apellidoBuscar),"\nIngresa el apellido de la/s persona/s que desea mostrar: ");
                MostrarApellido(apellidoBuscar, asociacionVeteranos);
            }
        } else if(opcion == 6){
            if(asociacionVeteranos.cant == 0){
                printf("\nNo hay datos cargados para mostrar \n");
            }else{
                InicializarLSE(&listaFallecidosIni);
                CrearLSEFallecidos(asociacionVeteranos, &listaFallecidosIni);
                MostrarLSE(listaFallecidosIni);
                LiberarLSE(&listaFallecidosIni);
            }
        } else if(opcion == 7){
            if(asociacionVeteranos.cant == 0){
                printf("\nNo hay datos cargados para mostrar \n");
            }else{
                printf("\n-------Ingrese la Fuerza que desea listar------- \n");
                do{
                    printf("1. Marina \n");
                    printf("2. Gendarmería \n");
                    printf("3. Ejército \n");
                    printf("4. Aereonáutica \n");
                    printf("5. Desconocido \n");
                    scanf("%d", &fuerzaListar);
                    if(fuerzaListar < 1 || fuerzaListar > 5){
                        printf("\nValor incorrecto. Ingrese nuevamente la fuerza \n");
                    }
                }while(!(fuerzaListar >= 1 && fuerzaListar <= 5));
                InicializarLDE(&listaFuerzaIni, &listaFuerzaFin);
                CrearLDEFuerza(asociacionVeteranos, fuerzaListar, &listaFuerzaIni, &listaFuerzaFin);
                MostrarLDE(listaFuerzaIni, listaFuerzaFin);
                LiberarLDE(&listaFuerzaIni, &listaFuerzaFin);
            }
        } else if(opcion == 8){
            if(asociacionVeteranos.cant == 0){
                printf("\nNo hay datos cargados para mostrar \n");
            }else{
                ciertoMes = ValidarComponente(1,12,"\nIngrese el mes de cumpleaños de las personas que desea mostrar: " );
                CrearArregloCiertoMes(ciertoMes, asociacionVeteranos, &asociacionCiertoMes);
                OrdenarBurbujaMes(&asociacionCiertoMes);
                if(asociacionCiertoMes.cant == 0){
                    printf("\nNo hay nadie registrado que cumpla con el mes indicado \n");
                }else{
                    printf("\n          Listado de personas que cumplen en el mes de %s\n ", MesesC(ciertoMes)); //hago una función para que devuelva en palabras??
                    for(i = 0 ;  i < asociacionCiertoMes.cant ;  i++){
                        personaMes = asociacionCiertoMes.a[i];
                        MostrarPersonaCiertoMes(personaMes);
                    }
                }
            }
        } else if(opcion == 9 || opcion == 10){
            ArregloAArchivo(asociacionVeteranos,&archVeteranos);
        }

        if(opcion < 1 || opcion > 10){
            printf("Valor incorrecto. Ingrese nuevamente la opción \n");
        }
    }while(!(opcion == 10));
    return 0;
}

int Comparar(struct TPersona a, struct TPersona b){
    /*Funcion que devuelve 1 si la persona a esta antes que la persona b en el
    orden lexicografico: apellido-nombre-DNI. Otro en caso contrario*/

    return((strcmp(a.apellido,b.apellido)>0)||((strcmp(a.apellido,b.apellido)==0)&&(strcmp(a.nombre,b.nombre)>0))||((strcmp(a.apellido,b.apellido)==0)&&(strcmp(a.nombre,b.nombre)==0)&&(a.DNI > b.DNI)));
}

int VaciaLSE(TNodo1 *puntIni){
    /*Funcion que devuelve 1 si la lista simple esta vacia(solo posee al ficticio). Otro en caso contrario*/

    return(puntIni->next == NULL);
}

int VaciaLDE(TNodo2 *puntIni, TNodo2 *puntFin){
    /*Funcion que devuelve 1 si la lista doble esta vacia(solo posee a los ficticios). Otro en caso contrario*/

    return(puntIni->next == puntFin);
}

int DiaSegunMes(struct Tfecha fecha){
    /*Funcion que devuelve cuantos dias debe tener cada mes dependiendo si es bisiesto o no*/

    if(fecha.mes == 1 || fecha.mes == 3 || fecha.mes  == 5 || fecha.mes  == 7 || fecha.mes == 8 || fecha.mes == 10 || fecha.mes == 12){
        return(31);
    }else if((fecha.mes == 2) && ((fecha.anio % 4 == 0 && fecha.anio % 100 != 0) || (fecha.anio % 400 == 0))){ //(si el año es divisible por 4 y no es divisible por 100) o el año es divisible por 400
        return(29);                                                                                            //entonces es año bisiesto
    }else if((fecha.mes == 2) && ((fecha.anio % 4 != 0) || ((fecha.anio % 100 == 0) && (fecha.anio % 400 != 0)))){ //(si el año no es divisible por 4) o es divisible por 100 pero no divisible por 400 
        return(28);                                                                                                //entonces no es bisiesto
    } else if(fecha.mes == 4 || fecha.mes == 6 || fecha.mes == 9 || fecha.mes == 11){
        return(30);
    }
}

char* MesesC(int mes){
    /*Mejora la legibilidad de la opcion Listar todas las personas segun apellido.
    Se pasan los numeros de los meses a texto.*/

    if(mes == 1){
        return("Enero");
    }else if(mes == 2){
        return("Febrero");
    }else if(mes == 3){
        return("Marzo");
    }else if(mes == 4){
        return("Abril");
    }else if(mes == 5){
        return("Mayo");
    }else if(mes == 6){
        return("Junio");
    }else if(mes == 7){
        return("Julio");
    }else if(mes == 8){
        return("Agosto");
    }else if(mes == 9){
        return("Septiembre");
    }else if(mes == 10){
        return("Octubre");
    }else if(mes == 11){
        return("Noviembre");
    }else if(mes == 12){
        return("Diciembre");
    }
}

int EmpiezaMayusSigueMin(char *cadena, int i){
    /*Evalua si una frase empieza sus palabras con Mayúsculas.
    Se utiliza para nombres propios*/

    if (cadena[i] != '\0' && isupper(cadena[i])) { 
        i++;
        while (cadena[i] != '\0' && cadena[i] != ' ') {
            if (!islower(cadena[i])) {
                return 0;
            }
            i++;
        }
        if(cadena[i] == ' '){
            i++;
            return EmpiezaMayusSigueMin(cadena, i); 
        }else{
            return 1;   
        }  
    }
    return 0;  
}

int ValidarComponente(int limiteInferior, int limiteSuperior, const char *mensaje) {
    /*Una estructura modularizada para validar el año, el mes, y el dia de una fecha.
    Las restricciones de cada fecha en particular se pasan como parámetro*/
    int componente;

    do {
        printf("%s", mensaje);
        scanf("%d", &componente);
        if (componente < limiteInferior || componente > limiteSuperior) {
            printf("Valor incorrecto. Ingrese nuevamente.\n");
        }
    } while (!(componente >= limiteInferior && componente <= limiteSuperior));
    return componente;
}

void ValidarCadena(char *cadena, int tamaño, const char *mensaje){
    /*Una estructura modularizada para validar que un nombre propio esté correctamente escrito*/
    int i;

    i = 0;
    do{
        printf("%s", mensaje);
        fgets(cadena, tamaño, stdin);
        cadena[strcspn(cadena, "\n")] = '\0'; 
        if(!EmpiezaMayusSigueMin(cadena, i)){
            printf("Valor incorrecto. Ingrese las palabras correctamente (primer letra mayúscula). \n");
        }
    }while(!EmpiezaMayusSigueMin(cadena, i));  
}

void InicializarLSE(TNodo1 **puntIni){
    /*Crea la lista simple con ficticio*/
    
    (*puntIni) = (struct TNodo1 *) malloc (sizeof(struct TNodo1));
    (*puntIni)->next = NULL;
}

void InicializarLDE(TNodo2 **puntIni, TNodo2 **puntFin){
    /*Crea la lista doble con los dos ficticios*/

    (*puntIni) = (struct TNodo2 *) malloc (sizeof(struct TNodo2));
    (*puntFin) = (struct TNodo2 *) malloc (sizeof(struct TNodo2));
    (*puntIni)->next = *puntFin;
    (*puntIni)->back = NULL;
    (*puntFin)->next = NULL;
    (*puntFin)->back = *puntIni;
}

void BusquedaSecuencialDNI(TData1 array, int dni, int *pos){
    /*Busqueda secuencial por DNI. Se devuelve la posicion donde se encontro a la persona,
    o -1 si no se encontro*/

    if(array.cant == 0){
        *pos = -1;
    }else{
        *pos = 0;
        while((*pos < array.cant) && (array.a[*pos].DNI != dni)){
            (*pos)++; 
        }
        if(*pos == array.cant){ 
            *pos = -1;
        }
    }
}

void BusquedaRecursivaApellido(TData1 array, char apellido[20], int *inf, int *sup){
    /*Busqueda recursiva por apellido. Se devuelve la posicion donde se encontro a la persona,
    o -1 si no se encontro*/
    int k;

    if(strcmp(apellido, array.a[0].apellido) < 0 || strcmp(apellido, array.a[array.cant-1].apellido) > 0){
        (*sup) = -1;
    } else if (*inf == *sup) {
        if (strcmp(array.a[*sup].apellido, apellido) != 0) {
            (*sup) = -1;
        }
    } else if (strcmp(array.a[0].apellido, apellido) <= 0 && strcmp(apellido, array.a[array.cant-1].apellido) <= 0) {
        k = ((*inf) + (*sup)) / 2;
        if (strcmp(apellido, array.a[k].apellido) > 0) {
            *inf = k + 1;
        } else if (strcmp(apellido, array.a[k].apellido) <= 0) {
            *sup = k;
        }
        BusquedaRecursivaApellido(array, apellido, &(*inf), &(*sup));
    }
}

void OrdenarInsercionApellido(TData1 *array){
    /*Ordena el arreglo por orden lexicografico utilizando el metodo de insercion*/
    int i,j;
    struct TPersona aux;

    i = 1;
    while(i < array->cant){
        aux = array->a[i];
        j = i-1;
        while(j > -1 && Comparar(array->a[j],aux)){
            array->a[j+1] = array->a[j];
            j--;
        }
        array->a[j+1] = aux;
        i++;
    }
}

void OrdenarBurbujaMes(TData2 *array){
    /*Ordena el arreglo de las personas que nacieron en cierto mes segun el dia en que nacieron*/
    int i, j;
    TPersonaCiertoMes personAux;

    i = array->cant - 1;
    while(i > 0){
        j = 0;
        while(j < i){
            if (array->a[j].nacimiento.dia > array->a[j+1].nacimiento.dia){
                personAux = array->a[j];
                array->a[j] = array->a[j+1];
                array->a[j+1] = personAux;
            }
            j++;
        }
        i--;
    }
}

void ArchivoAArreglo(FILE *arch, TData1 *array){
    /*Pasa todos los elementos del archivo al arreglo
    Aviso! Tuvimos que hacer esta accion de esta forma distinta
    a la que presentamos en notacion algoritmica, puesto que 
    esta fue la unica manera de que se lea correctamente el archivo
    de prueba que mandaron los profesores*/

    arch = fopen("malvinas.dat", "rb");
    if (arch == NULL) {
        printf("\n¡No hay ningún archivo creado aun! Se creará automáticamente\n"); 
        return;
    }
    fseek(arch, 0, SEEK_END); //se coloca el puntero de posicion al final del archivo
    long fileSize = ftell(arch); //se obtiene la posicion del puntero, que es el tamaño del archivo en bytes
    array->cant = fileSize / sizeof(struct TPersona); //se calcula la cantidad de elementos dividiendo el tamaño del archivo por el tamaño de TPersona
    fseek(arch, 0, SEEK_SET); //se vuelve al principio del archivo
    fread(array->a, sizeof(struct TPersona), array->cant, arch); //lee los datos desde el archivo al arreglo
    //tuvimos que usar ese fread, ya que leyendo con el bucle no se guardaban correctamente los elementos del archivo de prueba
    fclose(arch);
    if(array->cant == 0){
        printf("\n¡Aviso! ¡Aún no hay datos cargados! \n");        
    }else{
        OrdenarInsercionApellido(&*array);
    }
}

void ArregloAArchivo(TData1 array, FILE* *arch) {
    /*Se pasan los elementos del arreglo al archivo*/
    int i;
    struct TPersona persona;

    *arch = fopen("malvinas.dat", "wb");  
    if (*arch == NULL) {
        printf("\nError al abrir el archivo para escribir \n");
    }
    if (array.cant == 0) {
        printf("\nNo quedará ninguna persona guardada \n");
    }else{
        for (i = 0; i < array.cant; i++) {
            persona = array.a[i];
            fwrite(&persona, sizeof(persona), 1, *arch);
        }
    }
    fclose(*arch);
}

void CalcularEdad(struct Tfecha nacimiento, struct Tfecha diaActual, int *edad){
    /*Calcula la edad de una persona sii se conoce su fecha de nacimiento.
    Como por defecto las personas indeterminadas estan con nacimiento 1/1/1,
    entonces si el año de nacimiento es distinto de 1, alli se hace el calculo 
    correspondiente.*/

    if(nacimiento.anio == 1){
        *edad = -1;
    }else{
        *edad = diaActual.anio - nacimiento.anio;
        if((nacimiento.mes > diaActual.mes)||((nacimiento.mes == diaActual.mes)&&(nacimiento.dia > diaActual.dia))){
            (*edad)--;
        }
    }
}

void CalcularFecha(struct Tfecha *fecha, struct Tfecha diaActual, int tipoFecha) {
    /*Ayuda a modularizar el ingreso de una fecha.
    Hay 3 tipos de fechas: de nacimiento, fallecimiento y actual.
    actual=1,nacimiento=2,fallecimiento=3. 
    Dependiendo el tipo de fecha, habra un limite superior e inferior para dicha fecha.*/
    printf("\n¡Aviso! Ingrese la fecha en números, no con palabras \n");
    if(tipoFecha == 1){
        fecha->anio = ValidarComponente(1900, 2100, "Año: ");
        fecha->mes = ValidarComponente(1, 12, "Mes (1-12): ");
        fecha->dia = ValidarComponente(1, DiaSegunMes((*fecha)), "Día: ");
    } else if(tipoFecha == 2){
        fecha->anio = ValidarComponente(1900, 1965, "Año (hasta 1965): "); //porque el veterano tendría como mínimo 18 años en el momento de la guerra
        fecha->mes = ValidarComponente(1, 12, "Mes (1-12): ");
        fecha->dia = ValidarComponente(1, DiaSegunMes((*fecha)), "Día: ");
    } else if(tipoFecha == 3){
        fecha->anio = ValidarComponente(1982, diaActual.anio, "Año: "); //puede fallecer después de participar, porque sino no es veterano
        if((fecha->anio == diaActual.anio)){                               //no puede fallecer en un año posterior al actual 
            fecha->mes = ValidarComponente(1, diaActual.mes, "Mes (1-12): ");
            if((fecha->mes == diaActual.mes)){ 
                fecha->dia = ValidarComponente(1, diaActual.dia, "Día: ");
            } else{
                fecha->dia = ValidarComponente(1, DiaSegunMes(*fecha), "Día: ");
            }
        } else{
            fecha->mes = ValidarComponente(1, 12, "Mes (1-12): ");
            fecha->dia = ValidarComponente(1, DiaSegunMes(*fecha), "Día: ");
        }     
    }   
}

void ActualizarEdades(struct Tfecha diaActual, TData1 *array){
    /*Recorre el arreglo y va actualizando las edades en funcion de 
    la informacion ya cargada, y la fecha actual*/
    int i;

    for(i = 0; i < array->cant; i++){
        CalcularEdad(array->a[i].nacimiento,diaActual,&(array->a[i].edad));
    }
}

void TratamientoDatos(struct Tfecha diaActual, struct TPersona *persona, const char *mensaje){
    /*Permite la modificacion de todos los campos excepto el DNI de una persona*/
    int opcion, boolMuerte;

    do{
        printf("%s", mensaje);
        printf("1. Apellido \n");
        printf("2. Nombre \n");
        printf("3. Fecha de nacimiento \n");
        printf("4. Ciudad de residencia \n");
        printf("5. Ciudad de residencia en el momento de la guerra \n");
        printf("6. Dirección postal de su vivienda \n");
        printf("7. Correo electrónico \n");
        printf("8. Teléfono celular \n");
        printf("9. Número de beneficio nacional \n");
        printf("10. Código Postal \n");
        printf("11. Provincia donde nació \n");
        printf("12. Fuerza a la que perteneció \n");
        printf("13. Destino en Malvinas \n");
        printf("14. Función \n");
        printf("15. Grado \n");
        printf("16. Secuelas \n");
        printf("17. ¿Está vivo? (No = 0 / Sí = 1)\n");
        printf("18. Salir \n");
        scanf("%d",&opcion);
        printf("\n");
        if(opcion == 1){
            while (getchar() != '\n');
            ValidarCadena(persona->apellido, sizeof(persona->apellido), "Apellido: ");
        }else if(opcion == 2){
            while (getchar() != '\n');
            ValidarCadena(persona->nombre, sizeof(persona->nombre), "Nombre: ");
        }else if(opcion == 3){
            printf("\n  ¡Aviso! No puede ingresar una persona después de 1965 \n");
            CalcularFecha(&persona->nacimiento, diaActual, 2);
        }else if(opcion == 4){
            while (getchar() != '\n');
            ValidarCadena(persona->ciudad, sizeof(persona->ciudad), "Ciudad de residencia: ");
        }else if(opcion == 5){
            while (getchar() != '\n');
            ValidarCadena(persona->ciudadAntes, sizeof(persona->ciudadAntes), "Ciudad de residencia en el momento de la guerra: ");
        }else if(opcion == 6){
            while (getchar() != '\n');
            printf("Dirección postal: ");
            fgets(persona->DP, sizeof(persona->DP), stdin);
            persona->DP[strcspn(persona->DP, "\n")] = '\0';
        }else if(opcion == 7){
            while (getchar() != '\n');
            printf("Correo electrónico: ");
            fgets(persona->CE, sizeof(persona->CE), stdin);
            persona->CE[strcspn(persona->CE, "\n")] = '\0';
        }else if(opcion == 8){
            while (getchar() != '\n');
            printf("Teléfono celular: ");
            fgets(persona->tel, sizeof(persona->tel), stdin);
            persona->tel[strcspn(persona->tel, "\n")] = '\0';
        }else if(opcion == 9){
            while (getchar() != '\n');
            printf("Número de beneficio social: ");
            fgets(persona->beneficio, sizeof(persona->beneficio), stdin);
            persona->beneficio[strcspn(persona->beneficio, "\n")] = '\0';
        }else if(opcion == 10){
            while (getchar() != '\n');
            printf("Código postal: ");
            fgets(persona->CP, sizeof(persona->CP), stdin);
            persona->CP[strcspn(persona->CP, "\n")] = '\0';
        }else if(opcion == 11){
            while (getchar() != '\n');
            ValidarCadena(persona->provincia, sizeof(persona->provincia), "Provincia: ");
        }else if(opcion == 12){
            do{
                printf("\n-------Fuerza a la que perteneció-------\n");
                printf("1. Marina \n");
                printf("2. Gendarmería \n");
                printf("3. Ejército \n");
                printf("4. Aereonáutica \n");
                printf("5. Desconocido \n");
                scanf("%d", &persona->fuerza);
                if( persona->fuerza < 1 || persona->fuerza > 5){
                    printf("Valor incorrecto. Ingrese nuevamente la fuerza \n");
                }
            }while(!(persona->fuerza >= 1 && persona->fuerza <= 5));
        }else if(opcion == 13){
            while (getchar() != '\n');
            ValidarCadena(persona->destino, sizeof(persona->destino), "Destino en Malvinas: ");
        }else if(opcion == 14){
            while (getchar() != '\n');
            printf("Función: "); 
            fgets(persona->funcion, sizeof(persona->funcion), stdin);
            persona->funcion[strcspn(persona->funcion, "\n")] = '\0';
        }else if(opcion == 15){
            while (getchar() != '\n');
            printf("Grado: ");
            fgets(persona->grado, sizeof(persona->grado), stdin);
            persona->grado[strcspn(persona->grado, "\n")] = '\0';
        }else if(opcion == 16){
            while (getchar() != '\n');
            printf("Secuelas: ");
            fgets(persona->secuelas, sizeof(persona->secuelas), stdin);
            persona->secuelas[strcspn(persona->secuelas, "\n")] = '\0';
        }else if(opcion == 17){
            printf("¿Está vivo? (No = 0 / Sí = 1): ");
            scanf("%d", &persona->VF);
            if(!(persona->VF)){
                printf("¿Tiene conocimiento de la fecha de fallecimiento? (No = 0 / Sí = 1): ");
                scanf("%d",&boolMuerte);
                if(boolMuerte){
                    printf("Ingrese la fecha de fallecimiento: \n");
                    CalcularFecha(&persona->fallecimiento,diaActual, 3);
                }
            }
        }
        if(opcion < 1 || opcion > 18){
            printf("Valor incorrecto. Ingrese nuevamente la opción \n");
        }
    }while(!(opcion == 18)); 
    CalcularEdad(persona->nacimiento,diaActual,&(persona->edad));
}

void CargarPersona(TData1 array, struct Tfecha diaActual, struct TPersona *personax){
    /*Carga una persona que aun no este guardada en la base de datos.
    Obligatoriamente se debe cargar la edad, nombre y apellido de cada persona,
    puesto que ello se usara para mantener el orden, el resto de datos son
    opcionales.*/
    int pos, i;

    i = 0;
    printf("\n¡Aviso! El DNI es el único dato que no se podrá modificar. \n");
    printf("Si desea hacerlo, debe eliminar a la persona con el DNI incorrecto e ingresar los datos nuevamente. \n");
    printf("\n");
    do{
        personax->DNI = ValidarComponente(MinDNI, 100000000, "DNI: ");
        BusquedaSecuencialDNI(array,personax->DNI,&pos);
        if(pos != -1){
            printf("Ya hay una persona guardada con ese DNI \n");
        }
    }while(pos != -1); 
    while (getchar() != '\n');
    ValidarCadena(personax->apellido, sizeof(personax->apellido), "Apellido: ");
    ValidarCadena(personax->nombre, sizeof(personax->nombre),"Nombre: ");
    strcpy(personax->destino, "-");
    strcpy(personax->ciudad, "-");
    strcpy(personax->ciudadAntes, "-");
    strcpy(personax->DP, "-");
    strcpy(personax->CE, "-");
    strcpy(personax->CP, "-");
    strcpy(personax->tel, "-");
    strcpy(personax->beneficio, "-");
    strcpy(personax->provincia, "-");
    strcpy(personax->grado, "-");
    strcpy(personax->funcion, "-");
    strcpy(personax->secuelas, "-");
    personax->edad = -1;
    personax->nacimiento.dia = 1;
    personax->nacimiento.mes = 1;
    personax->nacimiento.anio = 1;
    personax->fallecimiento.dia = 1;
    personax->fallecimiento.mes = 1;
    personax->fallecimiento.anio = 1;
    personax->VF = 1;
    personax->fuerza = 5;
    TratamientoDatos(diaActual,&*personax,"\n---------Seleccione qué dato acerca de la persona desea almacenar--------- \n");
}

void InsertarEnOrden(struct TPersona personax, TData1 *array){
    /*Busca el lugar donde se debe ingresar la persona, y la mete alli.*/
    int i,j;

    i = 0;
    while((i < array->cant)&&(Comparar(personax,array->a[i]))){
        i++;
    }
    j = array->cant;
    while(j > i){
        array->a[j] = array->a[j-1];
        j--;
    }
    array->a[i] = personax;
    (array->cant)++;
}

void SuprimirPersona(int dni, TData1 *array){
    /*Borra una persona existente.*/
    int pos, i;

    BusquedaSecuencialDNI(*array,dni,&pos);
    if(pos == -1){
        printf("\nNo se encontró ninguna persona con ese DNI! \n");
    }else{
        i = pos;    
        while(i < array->cant-1){
            array->a[i] = array->a[i+1];
            i++;
        }
        (array->cant)--;
        printf("\nLa persona con el DNI %d fue eliminada correctamente. \n", dni);
    }
}

void MostrarPersonaCompleto(struct TPersona persona){
    /*Muestra todos los datos de una persona.*/

    printf("-------------------------------------------------------------- \n");
    printf("Apellido: %s \n",persona.apellido);
    printf("Nombre: %s \n",persona.nombre);
    printf("DNI: %d \n",persona.DNI);
    printf("Ciudad de residencia: %s \n",persona.ciudad);
    printf("Direccion postal de su vivienda: %s \n",persona.DP);
    printf("Correo electronico: %s \n",persona.CE);
    printf("Telefono celular: %s \n",persona.tel);
    printf("Numero de beneficio social: %s \n",persona.beneficio);
    printf("Codigo postal: %s \n",persona.CP);
    printf("Provincia donde nacio: %s \n",persona.provincia);
    printf("Ciudad de residencia en el momento de la guerra: %s \n",persona.ciudadAntes);
    printf("Destino en Malvinas: %s \n",persona.destino);
    printf("Funcion: %s \n",persona.funcion);
    printf("Grado: %s \n",persona.grado);
    printf("Secuelas: %s \n",persona.secuelas);
    if(persona.nacimiento.anio != 1){
        printf("Fecha de nacimiento: %d/%d/%d \n",persona.nacimiento.dia,persona.nacimiento.mes,persona.nacimiento.anio);
    }else{
        printf("Se desconoce su fecha de nacimiento. \n");
    }
    if(persona.VF && persona.nacimiento.anio != 1){
        printf("Está vivo. \n");
        printf("Su edad actual es de %d años. \n",persona.edad);
    }else if(persona.VF && persona.nacimiento.anio == 1){
        printf("Está vivo. \n");
        printf("Se desconoce su edad. \n");
    }else if(!(persona.VF) && persona.nacimiento.anio != 1 ){
        printf("Está muerto. \n");
        printf("Su edad seria: %d \n",persona.edad);
    }else if(!(persona.VF) && persona.nacimiento.anio == 1){
        printf("Está muerto. \n");
        printf("Se desconoce su edad. \n");
    }
    if(!(persona.VF) && persona.fallecimiento.anio != 1){
        printf("Fecha de fallecimiento: %d/%d/%d \n",persona.fallecimiento.dia,persona.fallecimiento.mes,persona.fallecimiento.anio);
    }else if (!(persona.VF) && persona.fallecimiento.anio == 1){
        printf("Se desconoce su fecha de fallecimiento \n");
    }
    if(persona.fuerza == 1){
        printf("Pertenecía a la marina. \n");
    }else if(persona.fuerza == 2){
        printf("Pertenecía a gendarmería. \n");
    }else if(persona.fuerza == 3){
        printf("Pertenecía al ejército. \n");
    }else if(persona.fuerza == 4){
        printf("Pertenecía a aeronáutica. \n");
    }else if(persona.fuerza == 5){
        printf("No se sabe a que fuerza perteneció. \n");
    }
    printf("-------------------------------------------------------------- \n");
}

void ModificarPersona(struct Tfecha diaActual, int dni, TData1 *array){
    /*Permite modificar todos los datos de una persona existente, 
    incluso aquellos donde no se cargo nada y quedo por defecto
    alguna marca especial.*/
    int pos;
    struct TPersona persona;
    char apellidoTemporal[20], nombreTemporal[20];

    BusquedaSecuencialDNI((*array), dni, &pos);
    if(pos == -1){
        printf("\n¡No se encontró ninguna persona con ese DNI! \n");
    }else{
        persona = array->a[pos];
        strcpy(apellidoTemporal, persona.apellido);
        strcpy(nombreTemporal, persona.nombre);
        printf("\n¡Aviso! El DNI es el único dato que no se podrá modificar. \n");
        printf("Si desea hacerlo, debe eliminar a la persona con el DNI incorrecto e ingresar los datos nuevamente. \n");
        TratamientoDatos(diaActual, &persona, "\n---------Seleccione el/los dato/s que desea modificar---------\n");
        printf("\n          Datos modificados de la persona \n");
        MostrarPersonaCompleto(persona);
        array->a[pos] = persona;
        if(strcmp(nombreTemporal,persona.nombre) != 0 || (strcmp(apellidoTemporal,persona.apellido) != 0) ){
            OrdenarInsercionApellido(&(*array));
        }
    }
}

void MostrarApellido(char apellido[20], TData1 array){
    /*Muestra todas las personas existentes que tienen cierto apellido*/
    int limInf, limSup, pos, i;
    struct TPersona persona;

    limInf = 0;
    limSup = array.cant-1;
    BusquedaRecursivaApellido(array,apellido,&limInf,&limSup);
    pos = limSup;
    if(pos == -1){
        printf("\n¡No se encontró ninguna persona con ese apellido! \n");
    }else{  
        i = pos;
        while(i > 0 && strcmp(array.a[i-1].apellido, apellido) == 0){
            i--;
        }
        printf("\n          Listado de personas con el apellido %s\n", apellido);
        while(i < array.cant && strcmp(array.a[i].apellido, apellido) == 0){
            persona = array.a[i];
            MostrarPersonaCompleto(persona);
            i++;
        }
    }
}

void InsertarCabezaLSE(TPersonaFallecida persona, TNodo1 **puntIni){
    /*Inserta a la cabeza en lista simple con ficticio*/
    TNodo1 *aux;

    aux = (struct TNodo1 *) malloc (sizeof(struct TNodo1));
    aux->next = (*puntIni)->next;
    aux->info = persona;
    (*puntIni)->next = aux;
}

void CrearLSEFallecidos(TData1 array, TNodo1 **puntIni){
    /*Crea lista simple de personas fallecidas*/
    int i;
    TPersonaFallecida persona;

    i = array.cant-1;
    while(i >= 0){
        if(!(array.a[i].VF)){
            strcpy(persona.apellido,array.a[i].apellido);
            strcpy(persona.nombre,array.a[i].nombre);
            strcpy(persona.ciudad,array.a[i].ciudad);
            persona.DNI = array.a[i].DNI;
            persona.fallecimiento = array.a[i].fallecimiento;
            InsertarCabezaLSE(persona, &(*puntIni));
        }
        i--;
    }
}

void MostrarLSE(TNodo1 *puntIni){
    /*Muestra todos los elementos de la lista simple.*/
    TNodo1 *aux;

    aux = puntIni;
    if(VaciaLSE(puntIni)){
        printf("\n¡No hay ningún fallecido registrado! \n");
    }else{
        printf("\n          Listado de personas Fallecidas      \n");
        while(aux->next != NULL){
            aux = aux->next;
            printf("-------------------------------------------------------------- \n");
            printf("Apellido: %s \n",aux->info.apellido);
            printf("Nombre: %s \n",aux->info.nombre);
            printf("DNI: %d \n",aux->info.DNI);
            printf("Ciudad de residencia actual: %s \n",aux->info.ciudad);
            if(aux->info.fallecimiento.anio != 1){
                printf("Fecha de fallecimiento: %d/%d/%d \n", aux->info.fallecimiento.dia, aux->info.fallecimiento.mes, aux->info.fallecimiento.anio);
            }else{
                printf("Se desconoce la fecha de fallecimiento \n");
            }
            printf("-------------------------------------------------------------- \n");
        }
    }
}

void LiberarLSE(TNodo1 **puntIni){
    /*Libera la lista simple.*/
    TNodo1 *aux;

    aux = (*puntIni);
    while((*puntIni) != NULL){
        (*puntIni) = aux->next;
        free(aux);
        aux = (*puntIni);
    }
}

void InsertarColaLDE(struct TPersona persona, TNodo2 **puntIni, TNodo2 **puntFin){
    /*Inserta a la cola en lista doble con ficticios*/
    TNodo2 *aux;

    aux = (struct TNodo2 *) malloc (sizeof(struct TNodo2));
    aux->info = persona;
    aux->next = *puntFin;
    ((*puntFin)->back)->next = aux;
    aux->back = (*puntFin)->back;
    (*puntFin)->back = aux;
}

void CrearLDEFuerza(TData1 array, int fuerza, TNodo2 **puntIni, TNodo2 **puntFin){
    /*Crea lista doble de personas que pertenecen a cierta fuerza*/
    int i;

    i = 0;
    while(i < array.cant){
        if(array.a[i].fuerza == fuerza){
            InsertarColaLDE(array.a[i],&(*puntIni),&(*puntFin));
        }
        i++;
    }
}

void MostrarLDE(TNodo2 *puntIni, TNodo2 *puntFin){
    /*Muestra todos los elementos de la lista doble.*/
    TNodo2 *aux;

    if(VaciaLDE(puntIni,puntFin)){
        printf("\n¡No hay ninguna persona registrada de la fuerza elegida! \n");
    }else{
        aux = puntIni;
        if((aux->next)->info.fuerza == 1){
            printf("\n          Listado de personas que han pertenecido a la Marina  \n"); 
        }else if((aux->next)->info.fuerza == 2) {
            printf("\n          Listado de personas que han pertenecido a la Gendarmería   \n");
        }else if((aux->next)->info.fuerza == 3) {
            printf("\n          Listado de personas que han pertenecido al Ejército  \n");
        }else if((aux->next)->info.fuerza == 4){
            printf("\n          Listado de personas que han pertenecido a la Areonáutica  \n");
        }else{
            printf("\n          Listado de personas que han pertenecido a una fuerza Deconocida   \n");
        }
        while(aux->next != puntFin){
            aux = aux->next;
            MostrarPersonaCompleto(aux->info);
        }
    }
}

void LiberarLDE(TNodo2 **puntIni, TNodo2 **puntFin){
    /*Libera la lista doble.*/
    TNodo2 *aux;

    aux = *puntIni;
    while(aux != NULL){
        *puntIni = aux->next;
        free(aux);
        aux = *puntIni;
    }
    *puntFin = NULL;
}

void CrearArregloCiertoMes(int mes, TData1 array1, TData2 *array2){
    /*Crea arreglo de las personas existentes que cumplen en cierto mes*/
    int i, j;

    array2->cant = 0;
    i = 0;
    j = 0;
    for ( i = 0; i < array1.cant; i++){
        if((array1.a[i].nacimiento.anio != 1) && (array1.a[i].nacimiento.mes == mes)){
            strcpy(array2->a[j].apellido, array1.a[i].apellido);
            strcpy(array2->a[j].nombre, array1.a[i].nombre);
            array2->a[j].nacimiento = array1.a[i].nacimiento;
            array2->a[j].edad = array1.a[i].edad;
            strcpy(array2->a[j].ciudad, array1.a[i].ciudad);
            strcpy(array2->a[j].DP, array1.a[i].DP);
            strcpy(array2->a[j].CE, array1.a[i].CE);
            array2->a[j].VF = array1.a[i].VF;
            j++;
            array2->cant++;
        }
    }
}

void MostrarPersonaCiertoMes(TPersonaCiertoMes persona){
    /*Muestra los elementos del arreglo de las personas existentes que nacieron en cierto mes*/

    printf("-------------------------------------------------------------- \n");
    printf("Apellido: %s \n", persona.apellido);
    printf("Nombre: %s \n", persona.nombre);
    printf("Fecha de nacimiento: %d/%d/%d \n", persona.nacimiento.dia, persona.nacimiento.mes, persona.nacimiento.anio);
    printf("Ciudad de residencia actual: %s \n", persona.ciudad);
    printf("Dirección postal de su vivienda: %s \n", persona.DP);
    printf("Correo electrónico: %s \n", persona.CE);
    if(persona.VF){
        printf("Está vivo. \n");
        printf("Su edad actual es de %d años. \n", persona.edad);
    }else{
        printf("Está muerto. \n");
        printf("Su edad actualmente sería de %d años. \n", persona.edad);
    }
    printf("-------------------------------------------------------------- \n");
}
