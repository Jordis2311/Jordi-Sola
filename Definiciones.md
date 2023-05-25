# Procesos
## Vida de un proceso
+ Nuevo: se solicitó al SO la creación de un proceso cuyos recursos y
estructuras están siendo creadas
+ Listo: está listo para iniciar la ejecución pero el SO no le asignó ningún
procesador
+ En Ejecución: el proceso está siendo ejecutado en éste momento.
+ Bloqueado: en espera de un evento para poder continuar su ejecución
+ Terminado: el proceso terminó de ejecutarse y sus estructuras están a la
espera de ser eliminadas por el SO.
+ Zombie: el programa ha terminado su ejecución pero el SO todavía debe
realizar algunas operaciones de limpieza para poder eliminarlo.
## Progamas Concurrentes
Es la descripción de un conjunto de máquinas de estados que cooperan mediante un medio de comunicación. Dos procesos se ejecutan concurrentemente si comparten el tiempo de vida.
## Garantías que asumimos del Sistema Operativo
En el caso que tengamos menos procesadores físicos que procesos tendremos que seleccionar que procesos se ejecutarán en cada
momento. Eso lo hace el Sistema Operativo, y está totalmente oculto a los procesos.
+ Scheduler Justo: el scheduler del Sistema Operativo es el encargado de seleccionar qué procesos, dentro del conjunto de procesos listos, comenzarán la ejecución. Nosotros asumimos que el scheduler es justo en el sentido que todo proceso listo eventualmente se ejecutará.
+ Procesos Confiables: los procesos harán lo que el programa les dice que hacer, y podemos confiar que su ejecución es fiel.
+ Procesos Asíncronos: no asumimos ninguna restricción de tiempo sobre la ejecución de las operaciones de cada uno de los procesos. En particular, no podemos asumir ningún orden de ejecución entre procesos concurrentes.
## File Descriptors
Un File Descriptor es un unsigned integer utilizado por un proceso para identificar un archivo abierto. Para cada proceso en nuestro sistema operativo, hay un Process Control Block (PCB). PCB realiza un seguimiento del contexto del proceso. Entonces, uno de los campos dentro de esto es un array llamado File Descriptor Table.

La File Descriptor Table es la colección de índices de enteros File Descriptors en los que los elementos son punteros a elementos de la File Table. Para cada proceso, se proporciona una File Descriptor Table única en el sistema operativo.

La File Table es una estructura de datos residente en el kernel, que contiene detalles de todos los archivos abiertos. Un elemento en la File Table es una estructura en memoria para un archivo/recurso abierto, que se crea cuando se procesa una solicitud para abrir un archivo/recurso y estas entradas mantienen
la posición del archivo/recurso. Los archivos/recursos podrían ser:
+ File
+ E/S de terminal
+ Pipes
+ Socket (para el canal de comunicación entre computadoras)
+ Cualquier dispositivo (CD-ROM, USB, etc)

Cada proceso en ejecución comienza con tres archivos ya abiertos:
| Nombre del File Descriptor | Nombre corto | Número del File Descriptor | Descripción                  |
| -------------------------- | ------------ | -------------------------- | ---------------------------- |
| Entrada estándar           | stdin        | 0                          |Entrada desde el teclado      |
| Salida estándar            | stdout       | 1                          | Salida a la consola          |
| Salida de error estándar   | stderr       | 2                          | Salida de error a la consola |
# Inter-Process Communication (IPC)
## Señales
Las señales son interrupciones producidas por un proceso o por el Sistema Operativo (Kernel) y sirven para identificar que algo ha ocurrido. Por ejemplo, CTRL+C/D, etc, producen señales a los procesos que se están ejecutando en una terminal. Aunque también se pueden producir por fallos de hardware (objetivo por el cual fueron creadas). Se dice que son generadas en el momento que se producen y son consideradas entregadas cuando el proceso que la recibe actúa sobre ellas.

Hay 5 tipos de comportamientos por defecto que puede generar una señal.
+ Term: la acción por defecto es terminar el proceso
+ Ign: la acción por defecto es de ignorar la señal
+ Core: la acción por defecto es terminar el proceso y generar un core dump
+ Stop: la acción por defecto es la de frenar el proceso
+ Cont: la acción por defecto es la de continuar un proceso frenado

SIGKILL y SIGSTOP son señales que no pueden ser capturadas o ignoradas. Siempre tienen efecto.
+ SIGKILL: La señal SIGKILL se envía a un proceso para que finalice inmediatamente. A diferencia de SIGTERM y SIGINT, esta señal no se puede capturar ni ignorar, y el proceso de recepción no puede realizar ninguna limpieza al recibir esta señal.
+ SIGSTOP: La señal SIGSTOP le indica al sistema operativo que detenga un proceso para su posterior reanudación.
## Pipes
Un pipe es un canal unidireccional (en una dirección), que permite escribir de un extremo, y leer del otro. Aunque se puede acceder a un pipe como a un archivo ordinario, el sistema en realidad lo gestiona como una cola FIFO. Es importante que el proceso que escribe, cierre su extremo de lectura del pipe y el proceso que lee, cierre su extremo de escritura del pipe. Cuando se crea un pipe, se le asigna un tamaño fijo en bytes. Cuando un proceso intenta escribir en el pipe, la solicitud de escritura se ejecuta inmediatamente si el pipe no está lleno. Si el pipe está lleno, el proceso se bloquea hasta que cambia el estado del pipe. Si el pipe está vacío, un proceso de lectura se bloquea; de lo contrario, se ejecuta el proceso de lectura. Solo un proceso puede acceder al pipe a la vez.
## Socket
Un socket es un canal generalizado para la comunicación entre procesos. Al igual que un pipe, un socket se representa como un File Descriptor. A diferencia de los pipes, los sockets permiten la comunicación entre procesos no relacionados (es decir, por ejemplo, no hace falta que sean Parent y Child) e incluso entre procesos que se ejecutan en diferentes computadoras en una misma red. Los sockets son el principal medio de comunicación con otras máquinas; telnet, rlogin, ftp, talk y otros programas de red conocidos utilizan sockets.
## Socket Orientado a Datagramas
El protocolo UDP, SOCK_DGRAM, se usa para enviar paquetes individuales a una dirección dada de manera no confiable. Cada vez que se escriben datos en un socket tipo SOCK_DGRAM, esos datos se convierten en un paquete. Dado que los sockets SOCK_DGRAM no trabajan con una conexión, se debe especificar la dirección del destinatario con cada paquete. La única garantía que ofrece el sistema sobre las solicitudes de transmisión de datos es que hará todo lo posible para entregar cada paquete que se envíe.
## Socket de tipo Stream
Los sockets de tipo stream orientan la conexión a establecer un canal de comunicación entre dos procesos o hilos. Es decir, se busca establecer una conexión estable y luego comenzar con el envío de mensajes. Esto garantiza la conexión, el envío y recepción de los mensajes, como a su vez el
orden en el que fueron enviados. En particular garantizan las siguientes propiedades:
+ Conexión Orientada a Stream: Cuando se quieren transmitir grandes volúmenes de datos,
estos datos se dividen en bytes (8-bits), y se crea una flujo (stream) de bytes. El flujo se recibe
en el orden en que fue enviada. Al ser un flujo de bytes, no se conservan límites en los datos.
+ Conexión Virtual de Circuitos: primero se establece una conexión estable. Luego, si la
comunicación falla por alguna razón (por ejemplo, algún nodo de la red se cae), ambas
computadoras detectan el fallo y lo comunican al proceso correspondiente.
+ Full Duplex: la conexión es bidireccional.
# Locks
## Problema del Jardín Ornamental
Un gran jardín ornamental se abre al público para que todos puedan apreciar sus fantásticas rosas, arbustos y plantas acuáticas. Por supuesto, se cobra una módica suma de dinero a la entrada para lo cual se colocan dos molinetes, uno en cada una de sus dos entradas. Se desea conocer cuánta gente ha ingresado al jardín así que se instala una computadora conectada a ambos molinetes: estos envían una señal cuando una persona ingresa al jardín.

Puede darse la siguiente secuencia de eventos durante la ejecución de estas instrucciones:
1. cuenta = 0
2. torniquete1: LEER (resultado: rax de p1 = 0, cuenta = 0)
3. torniquete1: INC (resultado: rax de p1 = 1, cuenta = 0)
4. El sistema operativo decide cambiar de tarea, suspende torniquete1 y
continúa con torniquete2.
5. torniquete2: LEER (resultado: rax de p2 = 0, cuenta = 0)
6. torniquete2: INC (resultado: rax de p2 = 1, cuenta = 0)
7. torniquete2: GUARDAR (resultado: rax de p2 = 1, cuenta = 1)
8. El sistema operativo decide cambiar de tarea, suspende torniquete2 y
continua con torniquete1.
9. torniquete1: GUARDAR (resultado: rax de p1 = 1, cuenta = 1)

Ambos procesos ejecutaron sus instrucciones para incrementar en 1 el contador. Sin embargo, en este caso cuenta tiene el valor 1. A este problema también se lo conoce como problema de las actualizaciones múltiples. Una instrucción aparentemente simple, como cuenta = cuenta + 1 habitualmente implica varias operaciones de más bajo nivel:
+ LEER: leer cuenta desde la memoria (p. ej. mov $cuenta,%rax).
+ INC: incrementar el registro (p. ej. add $1,%rax).
+ GUARDAR: guardar el resultado nuevamente en memoria (p. ej. mov%rax, $cuenta).

En el problema hay claramente un recurso compartido que es la cuenta, por tanto, el código que modifica la cuenta constituye una sección crítica y la operación cuenta = cuenta + 1 debe ser una operación atómica. La secuencia de eventos que se mostró es una condición de carrera.
## Propiedad Safety
Propiedad Mutex: A lo sumo un proceso accede a la Sección Crítica.
## Propiedad Liveness
Ausencia de Deadlock: Si hay procesos intentado tomar/soltar a un lock, algún proceso va a tomar/soltar el lock.
## Algoritmo de Peterson
Código de P1
```
f1 = true
turn = 2
while (f2 && turn == 2)
    /* esperar */;
CRIT;
f1 = false
```
Código de P2
```
f2 = true
turn = 1
while (f1 && turn == 1)
    /* esperar */;
CRIT;
f2 = false
```
## Consistencia secuencial y fences
Asumimos que haya una consistencia secuencial
1. Las operaciones de cada procesador se realizan en el orden especificado, y
2. Los stores son inmediatamente visibles al otro procesador. 

Esto no es cierto en procesadores modernos. Estos implementan ejecución fuera de orden y store buffering.
+ Ejecución fuera de orden: puede ocurrir que algunas instrucciones se hagan antes de lo estipulado en el código si la CPU se encuentra bloqueada para seguir el pipeline de instrucciones secuencialmente
+ Store buffering: los sistemas multiprocesador no garantizan que sus cachés son consistentes. Cuando un procesador escribe en una dirección de
memoria, esto no es inmediatamente visible a los otros procesadores.

Debido a esto, en computadoras modernas, incluso la versión correcta del algoritmo de Peterson puede fallar y dejar a ambos procesadores entrar a su RC, una forma de paliar estos efectos es usar un fence o barrera de memoria, un fence (instrucción mfence) causa que la CPU:
+ No reordene instrucciones a través del mismo
+ Garantice que todas las escrituras previas al fence son visibles a todos los procesadores antes de continuar

Código P1
```
f1 = true
turn = 2
asm("mfence");
while (f2 && turn == 2)
    /* esperar */;
CRIT;
f1 = false
``` 
Código P2
```
f2 = true
turn = 1
asm("mfence");
while (f1 && turn == 1)
    /* esperar */;
CRIT;
f2 = false
``` 
## Optimizaciones del Compilador
Otro problema independiente son las optimizaciones del compilador. En general, el compilador asume que las variables no cambian espóntaneamente. Es decir, si tenemos el fragmento:
```
v = 1;
/* ... mucho código que no modifica a ‘v‘ */
x = v;
```
El compilador está autorizado a optimizar la última asignación a x=1. Estas optimizaciones ocurren en tiempo de compilación, antes de correr el programa. Puede inspeccionarse el código assembly generado para determinar si fueron hechas o no Para evitar esto, se usa en general la palabra clave volatile. Usada en declaraciones de variables le dice al compilador que la variable puede cambiar espontáneamente, sin acción del programa.
## Deadlock
El término deadlock se utiliza cuando un programa concurrente entra en un estado donde ningún proceso puede progresar. Se da en general cuando los procesos compiten por varios recursos.
## Livelock
El término livelock se utiliza cuando dos o más procesos (activamente) realizan pasos que previenen el progreso de los otros procesos.
## Algoritmo de la Panadería
```
/* Variables globales */
Número: vector [1..N] de enteros = {todos a 0};
Eligiendo: vector [1..N] de booleanos = {todos a false};

/* Código del hilo i */
lock(i)
{
    /* Calcula el número de turno */
    Eligiendo[i] = true;
    Número[i] = 1 + max(Número[1],..., Número[N]);
    Eligiendo[i] = false;

    /* Compara con todos los hilos */
    for j in 1..N
    {
        /* Si el hilo j está calculando su número, espera a que termine */
        while ( Eligiendo[j] ) { /* busy waiting */ }

        /* Si el hilo j tiene más prioridad, espera a que ponga su número a cero */
        /* j tiene más prioridad si su número de turno es más bajo que el de i, */
        /* o bien si es el mismo número y además j es menor que i */
        while ((Número[j] != 0) &&
            ((Número[j] < Número[i]) || ((Número[j] == Número[i]) && (j < i)))) { /* busy waiting */ }
    }
}

/* Sección crítica */

unlock(i)
{
    Número[i] = 0;
}
```
## Variables de Condición
## Semáforos