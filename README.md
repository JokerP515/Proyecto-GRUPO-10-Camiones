Proyecto Grupo 10
Nombres:
- José David Díaz Quevedo
- Gabriela Ramírez Perdomo
- Melissa Vela Martínez
- Alisson Giselle Vargas

Funciones del programa:
1. Registrar Camión (Matricula, Consumo (gal/km), Capacidad y Carga actual)
2. Consultar datos de camión por ID
3. Cargar camión
4. Descargar camión
5. Mostrar todos los camiones
6. Mostrar todas las cargas que hay en 1 camión
7. Consultar carga por ID

Requerimientos
- Para que el código funcione, es necesario tener instalada la librería "Connector/C++" ofrecida por Oracle (https://dev.mysql.com/downloads/connector/cpp/), se sugiere instalarla en Visual Studio 2022 (tutorial: https://youtu.be/a_W4zt5sR1M?si=FKyMExWbdLe1o_KK)
- El programa necesita ejecutar previamente en el Workbench de MySQL el script llamado "TABLAS CAMIONES", de lo contrario no va a funcionar el programa creado en C++
- En el código de C++, tener en cuenta el modificar la línea 362 con la IP, usuario y contraseña que tenga el usuario que quiere ejecutar el programa, la IP por defecto ya está, sin embargo, el usuario y la contraseña es la misma que usa el usuario en el Workbench de MySQL

De ser posible, este Git se actualizará si fué posible integrar este proyecto en un contenedor, aún así, se adjunta el siguiente vídeo para observar la ejecución el programa: https://drive.google.com/file/d/1BzOAvAKLZfpqlI15jWaGACcCU1EuASl_/view?usp=sharing
