/*

    Nombre:tarea 13.c
    Autores: Torres Ramos, Juan Luis
    Sistemas Operativos

    Ejercicio 2: comprende el codigo. bloque de multiples archivos

*/

/*

    ./tarea13.c datos.txt informacion.txt

    Si datos.txt está bloqueado por otro proceso, el programa imprimirá información sobre el bloqueo existente y continuará con informacion.txt.
    Si el bloqueo tiene éxito en datos.txt, imprimirá "procesando el archivo".
    Luego, liberará el bloqueo de datos.txt y repetirá el proceso para informacion.txt.

    interbloqueo
        Proceso A intenta bloquear archivo 1
        Proceso B intenta bloquear archivo 2
        bloquea
    
    algunos SO solucionan esste problema 


*/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char * argv[]) 
{

	struct flock cerrojo;
	int fd;
	while (--argc > 0 ) {
		if ((fd=open(*++argv, O_RDWR)) == -1 ){
			perror("open fallo");
			continue;
		}
	cerrojo.l_type=F_WRLCK;
	cerrojo.l_whence=SEEK_SET;
	cerrojo.l_start=0;
	cerrojo.l_len=0;
	
	/* intentamos un bloqueo de escritura del archivo completo */

	while (fcntl (fd, F_SETLK, &cerrojo) == -1) {
		/* si el cerrojo falla, vemos quien lo bloquea */
		while (fcntl (fd, F_SETLK, &cerrojo) == -1 && cerrojo.l_type != F_UNLCK) {
			printf ("%s bloqueado por %d desde %lld hasta %lld para %c", *argv, cerrojo.l_pid, cerrojo.l_start, cerrojo.l_len, cerrojo.l_type==F_WRLCK ? 'w':'r');
		if (!cerrojo.l_len) break;
		cerrojo.l_start += cerrojo.l_len;
		cerrojo.l_len = 0;
		} /*mientras existan cerrojos de otros procesos */
	} /*mientras el bloqueo no tenga exito */

	/* ahora el bloqueo tiene exito y podemos procesar el archivo*/
	printf ("procesando el archivo\n");
	/* una vez finalizado el trabajo, desbloqueamos el archivo entero*/
	cerrojo.l_type=F_UNLCK;
	cerrojo.l_whence=SEEK_SET;
	cerrojo.l_start=0;
	cerrojo.l_len=0;
	if (fcntl (fd, F_SETLKW, &cerrojo) == -1) perror ("Desbloqueo");
	}
	return 0;
}