#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "externalSort.h"

#define BOOK_SIZE 52
#define FILE_NAME_SIZE 16

/* Method: read_input_parameters
 * Parameters:
 * library_size - Total number of books
 * max_books_in_memory - Limit of books simultaneously in memory
 * num_shelves - Number of shelves in library
 * max_books_in_shelf - Maximum number of books in each shelf
 * num_queries - Number of student's requests
 * Return value:
 * none
 * Description:
 * Reads parameters from input.
 */

void read_input_parameters(int *library_size, int *max_books_in_memory, int *num_shelves,
						   int *max_books_in_shelf, int *num_queries);

/* Method: read_input_books
 * Parameters:
 * all_books - File with all books
 * library_size - Total number of books
 * Return value:
 * none
 * Description:
 * Reads books from input and writes to all_books.
 */

void read_input_books(FILE *all_books, int library_size);

/* Method: create_shelves
 * Parameters:
 * estante - Array of files (shelves)
 * livros_ordenados - Sorted books file (livros_ordenados)
 * num_shelves - Number of shelves in library
 * max_books_in_shelf - Maximum number of books in each shelf
 * library_size - Total number of books
 * Return value:
 * Returns the number of used shelves in library.
 * Description:
 * Reads livros_ordenados and creates each sorted shelf.
 */

int create_shelves(FILE **estante, FILE *livros_ordenados, int num_shelves,
				   int max_books_in_shelf, int library_size);

/* Method: create_index
 * Parameters:
 * estante - Array of files (shelves)
 * index - Index file (indice)
 * used_shelves - Number of shelves with books inside
 * num_shelves - Number of shelves in library
 * Return value:
 * none
 * Description:
 * Creates index file.
 */

void create_index(FILE **estante, FILE* index, int used_shelves, int num_shelves);

/* Method: search_index
 * Parameters:
 * index - Index file (indice)
 * search_word - Name of the book to be searched
 * Return value:
 * Returns the possible shelf to be searched;
 * If the book cannot be in library, returns -1.
 * Description:
 * Makes a linear search in index to find possible shelf.
 */

int search_index(FILE *index, char *search_word);

/* Method: binary_search_shelf
 * Parameters:
 * shelf - Single shelf to be searched
 * search_word - Name of the book to be searched
 * Return value:
 * Returns the position of the book, if it's available;
 * Returns -2 if the book exists but is unavailable;
 * Returns -1 if the book does not exist.
 * Description:
 * Makes a binary search in a shelf to find search_word.
 */

int binary_search_shelf(FILE *shelf, char *search_word);

/* Method: answer_requests
 * Parameters:
 * num_queries - Number of student's requests
 * index - Index file (indice) 
 * estante - Array of files (shelves)
 * Return value:
 * none
 * Description:
 * Prints output for each request.
 */

void answer_requests(int num_queries, FILE* index, FILE **estante);

/* Method: free_shelves
 * Parameters:
 * estante - Array of files (shelves)
 * num_shelves - Number of shelves in library
 * Return value:
 * none
 * Description:
 * Closes all shelves.
 */

void free_shelves(FILE **estante, int num_shelves);

#endif