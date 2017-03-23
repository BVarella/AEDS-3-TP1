#ifndef EXTERNALSORT_H
#define EXTERNALSORT_H

#define TITLE_SIZE 51

typedef struct book{
	char title[TITLE_SIZE];
	char available;
} book;

typedef struct smallest_book{
		book smallest;
		int position;
} smallest_book;

/* Method: create_sorted_temp_files
 * Parameters:
 * temp - Array of temporary files
 * input_file - File with all the books
 * max_books_in_memory - Limit of books simultaneously in memory
 * library_size - Total number of books
 * Return value:
 * none
 * Description:
 * Creates an array of books that fits the memory, sort them internally
 * and writes to temporary files.
 */

void create_sorted_temp_files(FILE **temp, FILE *input_file, int max_books_in_memory, int library_size);

/* Method: merge_sorted_files
 * Parameters:
 * temp - Array of temporary files
 * output - Output file (livros_ordenados)
 * num_temp_files - Number of temporary files
 * library_size - Total number of books
 * Return value:
 * none
 * Description:
 * Merge temporary files to sorted output.
 */

void merge_sorted_files(FILE **temp, FILE *output, int num_temp_files, int library_size);

/* Method: sort_all_books
 * Parameters:
 * input_file - File with all the books
 * output - Output file (livros_ordenados)
 * max_books_in_memory - Limit of books simultaneously in memory
 * library_size - Total number of books
 * Return value:
 * none
 * Description:
 * Auxiliary function that creates temporary files and 
 * calls both create_sorted_temp_files and merge_sorted_files.
 */

void sort_all_books(FILE *input_file, FILE *output, int max_books_in_memory, int library_size);

#endif