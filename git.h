/**
 * @file git.h
 * @brief Archivo de cabecera para un sistema básico de control de versiones tipo Git.
 * 
 * Este archivo contiene las definiciones de las estructuras de datos y los prototipos
 * de funciones para un sistema simple tipo Git. Incluye funcionalidad para inicializar 
 * un repositorio, agregar/eliminar archivos, hacer commits, listar archivos y restaurar commits previos.
 * 
 * @authors
 * - Benjamin Sanhueza (bsanhuez@umag.cl)
 * - Nicolas Mancilla (nicmanci@umag.cl)
 */

#define GIT_H

#define MAX_ARG_LENGTH 50 ///< Número máximo de caracteres para nombres de archivos y mensajes de commit.
#define MAX_COMMAND_LENGTH 100 ///< Número máximo de caracteres para la entrada de comandos.
#define MAX_FILES 10 ///< Número máximo de archivos permitidos por commit.
#define MAX_COMMIT 15 ///< Número máximo de commits por versión.

/**
 * @brief Estructura que representa un nodo de archivo en el repositorio.
 * 
 * Esta estructura contiene el nombre del archivo y un puntero al siguiente nodo de archivo.
 */
typedef struct FileNode 
{
    char filename[MAX_ARG_LENGTH]; ///< Nombre del archivo.
    struct FileNode *next; ///< Puntero al siguiente nodo de archivo.
} FileNode;

/**
 * @brief Estructura que representa un commit en el sistema de control de versiones.
 * 
 * Un commit contiene un arreglo de nodos de archivo y un mensaje asociado con el commit.
 */
typedef struct commitGit 
{
    FileNode archivos[MAX_FILES]; ///< Lista de archivos incluidos en el commit.
    char mensaje[MAX_ARG_LENGTH]; ///< Mensaje del commit.
    struct commitGit *next; ///< Puntero al siguiente commit en la historia.
} commitGit;

/**
 * @brief Estructura que representa una versión en el sistema de control de versiones.
 * 
 * Una versión contiene un número y un arreglo de commits.
 */
typedef struct versionGit 
{
    int num_version; ///< Número de versión.
    commitGit commit[MAX_ARG_LENGTH]; ///< Lista de commits en la versión.
    struct versionGit *next; ///< Puntero a la siguiente versión.
} versionGit;

/**
 * @brief Inicializa el repositorio.
 * 
 * Esta función configura el estado inicial del repositorio y lo marca como inicializado.
 * 
 * @return 0 en caso de éxito, o 1 si el repositorio ya está inicializado.
 */
int init_repo();

/**
 * @brief Agrega un archivo al área de preparación.
 * 
 * Esta función agrega un archivo al área de preparación para el siguiente commit.
 * 
 * @param filename El nombre del archivo que se agregará.
 * @return 0 en caso de éxito, -1 si ocurrió un error.
 */
int add_file(const char *filename);

/**
 * @brief Crea un commit con los archivos en preparación.
 * 
 * Esta función realiza el commit de los archivos en preparación en el repositorio con un mensaje dado.
 * 
 * @param mensaje El mensaje del commit.
 * @return 0 en caso de éxito, -1 si ocurrió un error.
 */
int commit(const char *mensaje);

/**
 * @brief Muestra el historial de commits.
 * 
 * Esta función imprime la lista de commits realizados en el repositorio.
 * 
 * @return 0 en caso de éxito, -1 si ocurrió un error.
 */
int log_commits();

/**
 * @brief Cambia a un commit anterior.
 * 
 * Esta función restaura el estado del repositorio al commit especificado.
 * 
 * @param commit_id El mensaje o ID del commit al que se desea cambiar.
 * @return 0 en caso de éxito, -1 si no se encuentra el commit o ocurrió un error.
 */
int checkout_commit(const char *commit_id);

/**
 * @brief Lista los archivos en el área de preparación.
 * 
 * Esta función imprime la lista de archivos que están actualmente en el área de preparación para commit.
 * 
 * @return 0 en caso de éxito, -1 si ocurrió un error o no hay archivos en preparación.
 */
int list_files();

/**
 * @brief Elimina un archivo del área de preparación.
 * 
 * Esta función elimina un archivo específico del área de preparación.
 * 
 * @param filename El nombre del archivo que se desea eliminar.
 * @return 0 en caso de éxito, -1 si no se encuentra el archivo o si ocurrió un error.
 */
int remove_file(const char *filename);

