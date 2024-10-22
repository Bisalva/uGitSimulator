/**
 * @file main.c
 * @brief Implementación principal del sistema de control de versiones uGit.
 * 
 * Este archivo contiene la lógica principal de interacción con el usuario para el sistema de control de versiones uGit.
 * Permite ejecutar comandos como `init`, `add`, `rm`, `commit`, `log`, `checkout`, `ls` y `exit` a través de un prompt interactivo.
 * 
 * @authors
 * - Benjamin Sanhueza (bsanhuez@umag.cl)
 * - Nicolas Mancilla (nicmanci@umag.cl)
 */

#include <stdio.h>
#include <string.h>
#include "git.h"

/**
 * @brief Función principal que ejecuta el sistema uGit.
 * 
 * Esta función inicializa el prompt interactivo que permite a los usuarios ejecutar
 * los comandos de uGit para administrar un repositorio simulado.
 * 
 * @return 0 si el programa finaliza correctamente.
 */
int main() {
    char command[MAX_COMMAND_LENGTH]; ///< Almacena el comando introducido por el usuario.
    char *result;

    printf("Bienvenido a uGit\n");

    while (1) // Bucle infinito para el prompt de la consola
    {
        printf("ugit> "); // Prompt para el usuario
        
        result = fgets(command, MAX_COMMAND_LENGTH, stdin); // Leer el comando y verificar si se ha leído correctamente
        if (result == NULL) 
        {
            printf("Error al leer el comando.\n"); // Warning del inicio del programa
            continue;
        }

        command[strcspn(command, "\n")] = 0; // Remover el salto de línea al final de la entrada

        char *token = strtok(command, " "); // Parsear el comando y argumentos
        if (token == NULL) continue;

        if (strcmp(token, "init") == 0) // Inicializa desde el prompt
        {
            if (init_repo() == 0) 
            {
                printf("Repositorio inicializado correctamente.\n");
            } 
            else
            {
                printf("Error al inicializar el repositorio.\n"); // Warning de la inicialización
            }
        } 
        else if (strcmp(token, "add") == 0) // Añade archivo desde el prompt
        {
            char *filename = strtok(NULL, " ");
            if (filename != NULL) 
            {
                add_file(filename);
            } 
            else
            {
                printf("Error: nombre del archivo no proporcionado.\n"); // Warning de creación de archivos
            }
        } 
        else if (strcmp(token, "rm") == 0) // Borra archivo desde el prompt
        {
            char *filename = strtok(NULL, " ");
            if (filename != NULL) 
            {
                remove_file(filename);
            } 
            else 
            {
                printf("Error: nombre del archivo no proporcionado.\n"); // Warning del remove de archivos
            }
        } 
        else if (strcmp(token, "commit") == 0) // Genera un commit desde el prompt
        {
            char *message = strtok(NULL, "");
            if (message != NULL) 
            {
                commit(message);
            } 
            else 
            {
                printf("Error: mensaje de commit no proporcionado.\n"); // Warning de los commits
            }
        } 
        else if (strcmp(token, "log") == 0) // Genera el historial desde el prompt
        {
            log_commits();
        } 
        else if (strcmp(token, "checkout") == 0) // Cambia las versiones desde el prompt
        {
            char *commit_id = strtok(NULL, " ");
            if (commit_id != NULL) 
            {
                checkout_commit(commit_id);
            } 
            else 
            {
                printf("Error: ID del commit no proporcionado.\n"); // Warning de las versiones
            }
        } 
        else if (strcmp(token, "ls") == 0) // Genera una lista de archivos de la versión desde el prompt
        {
            list_files();
        } 
        else if (strcmp(token, "exit") == 0) // Finaliza el programa desde el prompt
        {
            printf("Saliendo de uGit.\n");
            break;
        } 
        else 
        {
            printf("Comando no reconocido: %s\n", command); // Warning en caso de comando no reconocido
        }
    }

    return 0;
}
