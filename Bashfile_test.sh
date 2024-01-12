echo Hola $USER
echo Tu directorio de trabajo actual es $PWD
cd /home
echo Nos movimos a $PWD
cd -
echo Volvimos a $PWD
echo Mostramos los archivos en el directorio
ls -lh
echo Lanzamos unos sleep en segundo plano
sleep 10 &
sleep 8 &
sleep 6 &
sleep 4 &
sleep 2 &
echo Mostramos los trabajos activos
jobs
echo $USER guarda este mensaje en el archivo "prueba" > prueba
cat < prueba
echo se copia del archivo prueba al archivo copia_prueba y lo mostramos
cat < prueba > copia_prueba
cat < copia_prueba
echo probamos una ejecucion en pipeline
ps aux | grep firefox
echo Dormimos por 5 segundos
sleep 5
echo Mientras dormiamos se ve como terminaron un par de trabajos en segundo plano
echo Esperamos a que finalicen los trabajos en segundo plano restantes y terminamos el programa
