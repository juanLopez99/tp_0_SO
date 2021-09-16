#include "client.h"
int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	config = iniciar_config();

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	valor = config_get_string_value(config,"CLAVE");
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"Puerto");
	log_info(logger, valor);

	//lee linea de consol
	//char* valorALoggear = readline("");
	//log_info(logger, valorALoggear);

	//Loggear valor de config

	leer_consola(logger);



	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor
	log_info(logger, "Creando conexion");
	conexion = crear_conexion(ip, puerto);

	if(conexion == -1){
		log_error(logger, "Conexion fallida");
		exit(-1);
	}
	log_info(logger, "Conexion exitosa");
	//enviar CLAVE al servirdor
	enviar_mensaje(valor, conexion);

	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("/home/utnso/tp0/client/tp0.config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	//El primero te lo dejo de yapa
	leido = readline(">");
	// Acá la idea es que imprimas por el log lo que recibis de la consola.
	while(!strcmp(leido, "")==0){
		log_info(logger, leido);
		leido = readline(">");
	}



}

void paquete(int conexion)
{
	//Ahora toca lo divertido!

	char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();

	leido = readline("ingrese texto a enviar (Ingrese vacio para salir) >");

	while(!strcmp(leido,"\0")==0){

		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline(">");
	}

	enviar_paquete(paquete,conexion);
	eliminar_paquete(paquete);
	free(leido);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	free(1);
	log_info(logger, "Terminando programa (cierre de conexion, log y config)");
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
