Grupo Numero 6: Laureano Ruiz github: lauruizv. | Mariano Delgado github: Marian4d.



1. Normalización: Delgado, Mariano (Marian4d). Procesa los archivos históricos y el inventario para generar los archivos normalizados necesarios para el funcionamiento del sistema. Se ejecuta una única vez.
   Partes Clave:

-Generación de claves: se genera una contraseña para cada mozo a partir de su nombre, aplicando un desplazamiento de caracteres.

-Cálculo de comisiones: se acumulan las comisiones correspondientes a cada mozo a partir de las comandas históricas.

-Separación de la información: se utilizan estructuras auxiliares en memoria para conservar temporalmente los datos necesarios, agrupar las comandas por fecha y generar los archivos diarios con la información normalizada.

3. Cierre:  Delgado, Mariano (Marian4d). Permite cerrar la semana siempre que exista la planilla del día ingresado como cierre de semana. Se espera que se ejecute al final de la semana, tras muchas ejecuciones de   ventas.cpp. De todas formas, la reitirada ejecucion de cierre (que no es lo ideal), Actualiza el archivo semanal sin dejar ducplicados.
   Partes Clave:
 
-Validación de la fecha: se comprueba que la fecha ingresada corresponda a un cierre permitido, se comprueba que exista el archivo de comandas correspondiente a la fecha ingresada. Si no existe, no se genera el cierre.

-Selección de archivos: se buscan los archivos correspondientes, tomando únicamente aquellos que existen. Se contemplan los cierres de fin de mes para no buscar fechas pertenecientes a otra semana.

-Generación del nombre del archivo semanal: se construye el nombre a partir del número de semana dentro del mes y del mes correspondiente.

-Apareo de comandas: se combinan los archivos diarios mediante un apareo secuencial, aprovechando que sus registros están ordenados por identificador de mozo. El resultado se escribe en archivos temporales hasta completar la combinación. Una vez finalizado el proceso, el archivo resultante se renombra con el nombre semanal correspondiente y se eliminan los archivos temporales utilizados.
