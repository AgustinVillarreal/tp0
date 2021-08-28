#include "client.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;
	t_paquete* paquete;

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	config = iniciar_config();

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "VALOR");

	//Loggear valor de config
	log_info(logger, ip);
	log_info(logger, puerto);
	log_info(logger, valor);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor

	log_info(logger, "Aca llegue");

	conexion = crear_conexion(ip, puerto);
	handshake(conexion);

	//enviar CLAVE al servidor

	enviar_mensaje(valor, conexion);

	paquete = crear_paquete();

	leer_consola(logger, paquete);

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente



	return EXIT_SUCCESS;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	if((nuevo_logger = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO)) == NULL){
		printf("Error al crear log");
		exit(1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("tp0.config");
	return nuevo_config;
}

void leer_consola(t_log* logger, t_paquete* paquete)
{
	char* leido;

	//El primero te lo dejo de yapa
	leido = readline(">");
	while(strcmp(leido, "")){
	// Acá la idea es que imprimas por el log lo que recibis de la consola.
		log_info(logger, leido);
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
	//El readline retorna NULL cuando la cadena es vacia, por lo que no hay que hacer un free
		leido = readline(">");
	}

}
/*
t_paquete* paquete(int conexion)
{
	//Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();

	return paquete;
}*/

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
