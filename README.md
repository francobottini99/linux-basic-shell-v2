# MyShell - Interprete de Línea de Comandos Extendido

Este proyecto es una extensión del intérprete de línea de comandos [MyShell](https://github.com/francobottini99/LINUXSHELL1-2022.git). El objetivo principal de este proyecto es ampliar las funcionalidades de [MyShell](https://github.com/francobottini99/LINUXSHELL1-2022.git), incorporando el manejo de señales, tuberías (pipes) y redirección de entrada/salida.

## Características Principales

### 1. Comandos Internos
MyShell sigue soportando los comandos internos básicos, como `cd`, `clr`, `echo`, `quit` y agrega algunos nuevos.

- **cd \<directorio\>**: Cambia el directorio actual a \<directorio\>. Soporta la opción `cd -` para retornar al último directorio de trabajo.

- **clr**: Limpia la pantalla.

- **echo \<comentario\|env var\>**: Muestra \<comentario\> en la pantalla seguido por una línea nueva.

- **quit**: Cierra MyShell.

- **jobs**: Lista todos los trabajos (jobs) en ejecución.

- **kill \<job id\>**: Termina un trabajo o proceso especificado por su ID.

- **fg \<job id\>**: Reanuda la ejecución en primer plano de un trabajo especificado por su ID.

### 2. Signal Handling
Se ha implementado el manejo de señales para las combinaciones CTRL-C, CTRL-Z y CTRL-\ en la shell. Estas señales se envían al trabajo en ejecución de primer plano en lugar de a MyShell. Si no hay un trabajo en ejecución de primer plano, no se realiza ninguna acción.

### 3. Pipe (Tuberías)
MyShell ahora proporciona la funcionalidad de tuberías a través del operador **|**. Este operador conecta la salida estándar de un proceso con la entrada estándar del siguiente proceso.

Ejemplos:
```
$ last <username> | wc -l
$ ps aux | grep firefox
$ grep bash /etc/passwd | cut -d “:” -f 1 | sort -r
```

### 4. I/O Redirection (Redirección de Entrada/Salida)
MyShell ahora es capaz de manejar redirección de entrada/salida utilizando los operadores `<` y `>`. Por ejemplo:

```
program arg1 arg2 < inputfile > outputfile
```

Esto ejecuta el programa `program` con los argumentos `arg1` y `arg2`, utilizando `inputfile` como entrada estándar y `outputfile` como salida estándar. La redirección también funciona para el comando interno `echo`.

### 5. Comandos Externos
MyShell permite la ejecución de comandos externos. Puede ejecutar programas ubicados en el sistema de archivos utilizando paths relativos o absolutos.

## Compilación y Ejecución

Para compilar el proyecto, ejecutar:

```
make
```

Para ejecutar MyShell, utilizar:

```
./myshell [batchfile]
```

- Si se proporciona un batchfile como argumento, MyShell ejecutará los comandos desde el archivo y se cerrará al alcanzar el final del archivo.
- Si no se proporciona ningún argumento, MyShell mostrará el prompt y esperará comandos del usuario vía stdin.