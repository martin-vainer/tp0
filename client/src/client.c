#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char *ip;
	char *puerto;
	char *valor;

	t_log *logger;
	t_config *config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger,"Hola, soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */
  
	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// la CLAVE, IP, y PUERTO son clas claves de mi archivo cliente.config que funciona
	// como nuestro diccionario y tienen nuestros valores, 
	// aca las almacenamos en las variables ya predefinidas arriba.
	valor = config_get_string_value(config, "CLAVE");	
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config

	log_info(logger,"VALOR leido de la config: %s", valor);// *funciona como un printf

	/* ---------------- LEER DE CONSOLA ---------------- */

	// * vamos a loggear algo que entre por consola: 

	leer_consola(logger);

	

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);


	// Armamos y enviamos el paquete
	paquete(conexion);


	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente

	printf("\n CLIENTE CERRADO !!!!!!!");//8
}

t_log *iniciar_logger(void)
{	
	
	t_log *nuevo_logger = log_create("cliente.log", "CL_LOG", 1, LOG_LEVEL_INFO );
	
	// *manejo de errores: 
	if(nuevo_logger == NULL){
			perror("error con el log");
			exit(EXIT_FAILURE);
		}
	return nuevo_logger; // *devuelve logger o NULL en caso de error
}

t_config *iniciar_config(void)
{
	t_config *nuevo_config = config_create("/home/utnso/Documents/tp0/client/cliente.config");
	if(nuevo_config == NULL){
		perror("error en el config");
		exit(EXIT_FAILURE);
	}

	return nuevo_config;
}

void leer_consola(t_log *logger)
{
	char *leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger, leido);

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío:

	while( strcmp(leido, "") != 0 ){ // *funcion para comparar Strings, se podria comparar sin la funcion haciendo leido != '\0'
		
	// *mientras que comparando lo leido con un string vacio sea diferente a falso, entonces.. 
		free(leido);
		leido = readline("> ");
		log_info(logger, leido);
	}

	
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char *leido = NULL;
	t_paquete *paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

	leido = readline("> ");
	while( strcmp(leido, "") != 0 ){ // *funcion para comparar Strings, se podria comparar sin la funcion haciendo leido != '\0'
		
	// *mientras que comparando lo leido con un string vacio sea diferente a falso, entonces.. 
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);//*
		free(leido);
		leido = readline("> ");
	}


	//ENVIAR PAQUETE *
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);



}

void terminar_programa(int conexion, t_log *logger, t_config *config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  log_destroy(logger);
	  config_destroy(config);
	  liberar_conexion(conexion);
}
