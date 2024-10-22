/**
 * @file git.c
 * @brief Simulador básico de control de versiones tipo Git.
 * 
 * Este archivo contiene las implementaciones para las operaciones básicas
 * de un sistema de control de versiones simulado, como agregar archivos, 
 * crear commits, listar el historial de commits y cambiar entre versiones.
 * 
 * @authors 
 * Benjamin Sanhueza (bsanhuez@umag.cl)
 * Nicolas Mancilla (nicmanci@umag.cl)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "git.h"

/// Puntero al inicio de la lista de archivos en el área de preparación.
static FileNode *file_list = NULL; 

/// Puntero al inicio de la lista de commits.
static commitGit *commit_list = NULL; 

/// Puntero al inicio de la lista de versiones (no usado actualmente).
static versionGit *version_list = NULL; 

/// Indicador de si el repositorio ha sido inicializado.
static int is_repo_initialized = 0; 

/**
 * @brief Inicializa el repositorio.
 * 
 * Si el repositorio ya ha sido inicializado, no se realiza ninguna acción.
 * 
 * @return 0 en caso de éxito, 1 si ya estaba inicializado.
 */
int init_repo() 
{ 
    if (is_repo_initialized) 
    {
        printf("El repositorio ya está inicializado.\n");
        return 0;
    }

    is_repo_initialized = 1;
    return 0;
}

/**
 * @brief Verifica si el repositorio ha sido inicializado.
 * 
 * Si no está inicializado, muestra un mensaje de error.
 * 
 * @return 1 si está inicializado, 0 en caso contrario.
 */
int check_repo_initialized() 
{ 
    if (!is_repo_initialized) 
    {
        printf("Error: El repositorio no ha sido inicializado. Ejecuta 'init' primero.\n");
        return 0;
    }
    return 1;
}

/**
 * @brief Agrega un archivo al área de preparación.
 * 
 * Si el archivo ya existe, lo reemplaza.
 * 
 * @param filename Nombre del archivo a agregar.
 * @return 0 en caso de éxito, -1 si ocurre un error.
 */
int add_file(const char *filename)  
{
    if (!check_repo_initialized()) return -1;

    FileNode *current = file_list;
    while (current != NULL) 
    {    
        if (strcmp(current->filename, filename) == 0) 
        { 
            printf("El archivo %s ya existe. Reemplazando el archivo.\n", filename);
            strncpy(current->filename, filename, MAX_ARG_LENGTH);
            current->filename[MAX_ARG_LENGTH - 1] = '\0';
            return 0;
        }
        current = current->next;
    }

    FileNode *new_node = (FileNode *)malloc(sizeof(FileNode));  
    if (!new_node) 
    {
        perror("Error al asignar memoria");
        return -1;
    }
    
    strncpy(new_node->filename, filename, MAX_ARG_LENGTH);
    new_node->filename[MAX_ARG_LENGTH - 1] = '\0'; 
    new_node->next = file_list;
    file_list = new_node;

    printf("Archivo %s agregado al área de preparación.\n", filename);
    return 0;
}

/**
 * @brief Elimina un archivo del área de preparación.
 * 
 * @param filename Nombre del archivo a eliminar.
 * @return 0 en caso de éxito, -1 si ocurre un error.
 */
int remove_file(const char *filename) 
{ 
    if (!check_repo_initialized()) return -1;

    FileNode *current = file_list;
    FileNode *previous = NULL;

    while (current != NULL && strcmp(current->filename, filename) != 0) 
    { 
        previous = current;
        current = current->next;
    }

    if (current == NULL) 
    { 
        printf("Archivo no encontrado: %s\n", filename);
        return -1;
    }

    if (previous == NULL) 
    {
        file_list = current->next;
    } 
    else 
    {
        previous->next = current->next;
    }

    free(current);
    printf("Archivo %s eliminado.\n", filename);
    return 0;
}

/**
 * @brief Crea un commit con los archivos en el área de preparación.
 * 
 * Copia los archivos actuales en el área de preparación y los guarda en un nuevo commit.
 * 
 * @param mensaje Mensaje descriptivo del commit.
 * @return 0 en caso de éxito, -1 si ocurre un error.
 */
int commit(const char *mensaje)
{ 
    if (!check_repo_initialized()) return -1;

    commitGit *new_commit = (commitGit *)malloc(sizeof(commitGit));
    if (!new_commit) 
    {
        perror("Error al asignar memoria para el commit");
        return -1;
    }

    FileNode *current_file = file_list;
    int index = 0;

    while (current_file != NULL && index < MAX_FILES) 
    {
        strncpy(new_commit->archivos[index].filename, current_file->filename, MAX_ARG_LENGTH);
        new_commit->archivos[index].filename[MAX_ARG_LENGTH - 1] = '\0';
        current_file = current_file->next;
        index++;
    }

    strncpy(new_commit->mensaje, mensaje, MAX_ARG_LENGTH);
    new_commit->mensaje[MAX_ARG_LENGTH - 1] = '\0';

    new_commit->next = commit_list;
    commit_list = new_commit;

    printf("Commit creado con éxito: %s\n", mensaje);
    return 0;
}

/**
 * @brief Muestra el historial de commits.
 * 
 * Lista todos los commits realizados hasta el momento.
 * 
 * @return 0 en caso de éxito, -1 si ocurre un error.
 */
int log_commits() 
{
    if (!check_repo_initialized()) return -1;

    printf("==Historial de Commits==\n");
    commitGit *current_commit = commit_list;

    if (!current_commit) 
    {
        printf("No hay commits.\n");
        return 0;
    }
    
    while(current_commit)
    {
        printf("%s\n",current_commit->mensaje);
        current_commit = current_commit->next;
    }
    
    return 0;
}

/**
 * @brief Cambia a una versión anterior (commit) basada en su ID.
 * 
 * Limpia el área de preparación y restaura los archivos del commit seleccionado.
 * 
 * @param commit_id ID o mensaje del commit al que se quiere cambiar.
 * @return 0 en caso de éxito, -1 si ocurre un error.
 */
int checkout_commit(const char *commit_id)
{
    if (!check_repo_initialized()) return -1;

    commitGit *current_commit = commit_list;
    while (current_commit != NULL && strcmp(current_commit->mensaje, commit_id) != 0) 
    {
        current_commit = current_commit->next;
    }

    if (current_commit == NULL) 
    {
        printf("Error: Commit con ID '%s' no encontrado.\n", commit_id);
        return -1;
    }

    FileNode *current_file = file_list;
    while (current_file != NULL) 
    {
        FileNode *temp = current_file;
        current_file = current_file->next;
        free(temp);
    }
    file_list = NULL;

    for (int i = 0; i < MAX_FILES; i++) 
    {
        if (strlen(current_commit->archivos[i].filename) > 0) 
        {
            FileNode *new_file = (FileNode *)malloc(sizeof(FileNode));
            strncpy(new_file->filename, current_commit->archivos[i].filename, MAX_ARG_LENGTH);
            new_file->filename[MAX_ARG_LENGTH - 1] = '\0';
            new_file->next = file_list;
            file_list = new_file;
        }
    }

    printf("Restaurado al commit: %s\n", commit_id);
    return 0;
}

/**
 * @brief Lista los archivos en el área de preparación.
 * 
 * Muestra todos los archivos actualmente añadidos para commit.
 * 
 * @return 0 en caso de éxito, -1 si ocurre un error.
 */
int list_files()
{
    if (!check_repo_initialized()) return -1;

    FileNode *current = file_list;
    if (!current) 
    {
        printf("No hay archivos en el área de preparación.\n");
        return 0;
    }
    
    printf("Archivos en el área de preparación:\n");
    while (current) 
    {
        printf("%s\n", current->filename);
        current = current->next;
    }
    return 0;
}
