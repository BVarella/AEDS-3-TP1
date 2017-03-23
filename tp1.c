#include "my_functions.h"

int main(){

	int library_size, max_books_in_memory, num_shelves, max_books_in_shelf, num_queries;
	read_input_parameters(&library_size, &max_books_in_memory, &num_shelves, &max_books_in_shelf, &num_queries);
	
	FILE *all_books = tmpfile();
	read_input_books(all_books, library_size);

	FILE *livros_ordenados = fopen("livros_ordenados", "w+");
	sort_all_books(all_books, livros_ordenados, max_books_in_memory, library_size);
	fclose(all_books);

	FILE **estante = (FILE **) malloc(num_shelves * sizeof(FILE *));
	int used_shelves = create_shelves(estante, livros_ordenados, num_shelves, max_books_in_shelf, library_size);
	fclose(livros_ordenados);

	FILE *indice = fopen("indice", "w+");
	create_index(estante, indice, used_shelves, num_shelves);

	answer_requests(num_queries, indice, estante);

	fclose(indice);
	free_shelves(estante, num_shelves);
	
	return 0;
}