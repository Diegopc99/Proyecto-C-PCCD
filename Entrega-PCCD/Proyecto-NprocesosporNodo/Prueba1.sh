
_term(){

    echo "Capturada Control+C"
        killall nprocesos-nodo
}

trap _term SIGINT

echo Prueba 1:

gcc -Wall inicializador.c -o inicializador

./inicializador 1; id_nodo1=$?

./inicializador 2; id_nodo2=$?

./inicializador 3; id_nodo3=$?


echo $id_nodo1
echo $id_nodo2
echo $id_nodo3

gcc -Wall -pthread nprocesos-nodo.c -o nprocesos-nodo

./nprocesos-nodo $id_nodo1 3 1000 300 1500 200 600 650 $id_nodo2 $id_nodo3 &

child1=$!

./nprocesos-nodo $id_nodo2 3 1500 200 2500 100 475 700 $id_nodo1 $id_nodo3 &

child2=$!

./nprocesos-nodo $id_nodo3 3 1500 100 1500 250 800 900 $id_nodo2 $id_nodo1 

child3=$!


