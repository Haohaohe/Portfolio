#include "indices.h"
/*Ejemplo de un buscador de palabras en un documento.

Este script no esta pensado para funcionar por si mismo, se presenta unicamente para mostrar las habilidades de programacion en c++

*/

string eliminarSigPuntuacion(string &s){
		unsigned int i = s.length() - 1;
		while (ispunct(s[i])){
			i--;
		}
		s.erase(i + 1, s.length() - i);
	return s;
}

string eliminarCorchetes(string &s){
	if (s[0] == '<') s.erase(0, 1);
	return eliminarSigPuntuacion(s);
}

void inicializarListas(tListaCadenas & lista){
	lista.cont = 0;
}

string leerLaFrase(ifstream & ar){
	string x;
	getline(ar, x);
	transform(x.begin(), x.end(),x.begin(), tolower);
	return x;
}

int buscarFichero(tListaCadenas t, string pa){
	int i = 0;
	while (t.cad[i] != pa) i++;
	return i;
}

void crearTabla(tIndicePalabras & tabla, tListaCadenas & totales, const string & archivoInicial, tMatriz &L){
	tListaCadenas visitados, no_visitados;
	ifstream archivo;
	string frase, palabra, archivoActual;
	int i_frase;
	int pos_j;

	inicializarListas(visitados);
	inicializarListas(no_visitados);
	inicializarListas(totales);

	insertar(no_visitados, archivoInicial);
	insertar(totales, archivoInicial);

	while (no_visitados.cont > 0){
		archivoActual = getCadena(no_visitados, 0);
		insertar(visitados, archivoActual);
		eliminar(no_visitados, 0);

		//¿?duda
		pos_j = buscarFichero(totales, archivoActual);

		archivo.open(archivoActual);
		while (!archivo.fail()){
			frase = leerLaFrase(archivo);
			//Inicia y reinicia el contador a 0
			i_frase = 0;
			while (i_frase < frase.size()){
				//Reinicia la palabra a NULL
				palabra = "";
				//Separa cada palabra (se puede hacer una funcion)
				while (i_frase < frase.size() && !isspace(frase[i_frase]) && !ispunct(frase[i_frase])){
					palabra += frase[i_frase];
					i_frase++;
				}

				//Funcion para recorrer las enlaces (se puede hacer una funcion)
				if (frase[i_frase] == '<'){
					while (frase[i_frase] != '>'){
						palabra += frase[i_frase];
						i_frase++;
					}
					i_frase++;
				}
				//Saltar un corchete o signos de puntuacion
				if (isspace(frase[i_frase]) || ispunct(frase[i_frase]))i_frase++;

				if (palabra != "" && palabra[0] != '<') insertar(tabla, eliminarCorchetes(palabra), archivoActual);
				else if (palabra[0] == '<'){
					if (buscar(totales, eliminarCorchetes(palabra))){
						L.datos[buscarFichero(totales, palabra)][pos_j] = 1;
					}
					else{
						insertar(no_visitados, palabra);
						insertar(totales, palabra);
						//Ampliar
						L.filas++; L.columnas++;
						for (int i = 0; i < L.filas; i++){
							for (int j = 0; j < L.columnas; j++){
								if (L.datos[i][j] != 1) L.datos[i][j] = 0;
							}
						}
						L.datos[getTamanno(totales) - 1][pos_j] = 1;

					}
					
				}

			}
		}
		archivo.close();
	}
}