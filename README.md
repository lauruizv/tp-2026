Grupo Numero 6: Laureano Ruiz github: lauruizv. | Mariano Delgado github: Marian4d.

Importante: todos los archivos .dat deben ubicarse en la carpeta datos/. Los programas deben ejecutarse desde la carpeta raíz del proyecto para que las rutas relativas a esos archivos funcionen correctamente.

1. Normalización: Delgado, Mariano (Marian4d). Procesa los archivos históricos y el inventario para generar los archivos normalizados necesarios para el funcionamiento del sistema. Se ejecuta primero de todo y una única vez.

Partes Clave:
-Generación de claves: se genera una contraseña para cada mozo a partir de su 'id_Mozo', aplicando un desplazamiento de caracteres.
-Cálculo de comisiones: se acumulan las comisiones correspondientes a cada mozo a partir de las comandas históricas.
-Separación de la información: se utilizan estructuras auxiliares en memoria para conservar temporalmente los datos necesarios, agrupar las comandas por fecha y generar los archivos diarios con la información normalizada.

2. Ventas: Ruiz, Laureano (lauruizv). Simula la jornada laboral del buffet. Permite iniciar sesión, registrar las ventas del día, descontar el stock en tiempo real y generar el archivo diario de comandas. Se ejecuta diariamente para cargar operaciones.

Partes Clave:
​-Autenticación de mozos: se verifica la identidad mediante el ingreso del ID y una clave. A la clave ingresada se le aplica un corrimiento matemático acordado en grupo para validarla en vivo contra el archivo maestro de mozos.
​-Gestión dinámica de inventario y ventas: se registran los productos despachados y se actualiza el stock disponible sobreescribiendo directamente el archivo binario, validando en el proceso que exista cantidad suficiente para cubrir la venta.
​-Ordenamiento directo en disco: se implementa un algoritmo de burbuja operando directamente sobre la memoria física del archivo generado (mediante fseek, fread y fwrite). Esto agrupa las comandas por identificador de mozo sin depender de arreglos estáticos, respetando la consigna de no limitar el uso de memoria RAM.


3. Cierre:  Delgado, Mariano (Marian4d). Permite cerrar la semana siempre que exista la planilla del día ingresado como cierre de semana. Se espera que se ejecute al final de la semana, tras muchas ejecuciones de   ventas.cpp. De todas formas, la reitirada ejecucion de cierre (caso anormal), Actualiza el archivo semanal sin dejar ducplicados.

Partes Clave:
-Validación de la fecha: se comprueba que la fecha ingresada corresponda a un cierre permitido, se comprueba que exista el archivo de comandas correspondiente a la fecha ingresada. Si no existe, no se genera el cierre.
-Selección de archivos: se buscan los archivos correspondientes, tomando únicamente aquellos que existen. Se contemplan los cierres de fin de mes para no buscar fechas pertenecientes a otra semana.
-Generación del nombre del archivo semanal: se construye el nombre a partir del número de semana dentro del mes y del mes correspondiente.
-Apareo de comandas: se combinan los archivos diarios mediante un apareo secuencial, aprovechando que sus registros están ordenados por identificador de mozo. El resultado se escribe en archivos temporales hasta completar la combinación. Una vez finalizado el proceso, el archivo resultante se renombra con el nombre semanal correspondiente y se eliminan los archivos temporales utilizados.

4.  Resumen: Ruiz, Laureano (lauruizv). Procesa el archivo semanal unificado generado por el módulo de Cierre para emitir un reporte detallado del desempeño del personal y del local. Se ejecuta a demanda para consultar períodos ya finalizados.

Partes Clave:
​-Corte de control: se recorre el archivo secuencialmente agrupando los registros por identificador de mozo. Esto permite emitir los subtotales de productos vendidos y calcular la comisión correspondiente para cada empleado de forma secuencial y eficiente.
​-Totalización global: mediante la optimización de las lecturas, se calcula y expone la sumatoria general de todos los productos despachados por el buffet durante esa semana.
​-Consulta iterativa y validación: se provee una interfaz interactiva que permite al usuario ingresar el número de semana y mes para buscar el archivo correspondiente, reportando errores si la semana aún no fue procesada y permitiendo consultar múltiples reportes sin reiniciar el sistema.
