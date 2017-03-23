#include "my_functions.h"

//Return the number of books in a shelf
static int books_in_shelf(FILE* shelf){
	
	fseek(shelf, 0, SEEK_END);
	int final_position = ftell(shelf);
	return (final_position / BOOK_SIZE);
}

void read_input_parameters(int *library_size, int *max_books_in_memory, int *num_shelves,
						   int *max_books_in_shelf, int *num_queries){

	if(scanf("%d %d %d %d %d", library_size, max_books_in_memory,
								num_shelves, max_books_in_shelf,
								num_queries) != 5){
		exit(EXIT_FAILURE);
	}
}

void read_input_books(FILE *all_books, int library_size){

	book temp_book;

	for(int i = 0; i < library_size; ++i){
		memset (temp_book.title, '\0', 51);
		if(scanf("%s %c", temp_book.title, &temp_book.available) != 2){
			exit(EXIT_FAILURE);
		}
		fwrite(&temp_book.title, sizeof(temp_book.title), 1, all_books);
		fwrite(&temp_book.available, sizeof(temp_book.available), 1, all_books);
	}
}

int create_shelves(FILE **estante, FILE *livros_ordenados, int num_shelves,
				   int max_books_in_shelf, int library_size){

	char file_name[FILE_NAME_SIZE];
	for(int i = 0; i < num_shelves; ++i){
		snprintf(file_name, (FILE_NAME_SIZE * sizeof(char)), "estante%d", i);
		estante[i] = fopen(file_name, "w+b");
	}

	book temp_book;

	rewind(livros_ordenados);
	int used_shelves = 0;
	int organized_books = 0;
	for(int i = 0; i < num_shelves; ++i){
		if(organized_books == library_size)
			break;
		++used_shelves;
		for(int j = 0; j < max_books_in_shelf; ++j){
			memset (temp_book.title, '\0', TITLE_SIZE);

			if(fscanf(livros_ordenados, "%s", temp_book.title) != 1)
				break;
			if(fscanf(livros_ordenados, " %c", &temp_book.available) != 1)
				exit(EXIT_FAILURE);
			
			fwrite(temp_book.title, sizeof(*temp_book.title), TITLE_SIZE, estante[i]);
			fwrite(&temp_book.available, sizeof(temp_book.available), 1, estante[i]);
			++organized_books;
		}
	}

	return used_shelves;
}

void create_index(FILE **estante, FILE* index, int used_shelves, int num_shelves){
	
	int unused_shelves = num_shelves-used_shelves;
	book temp_book;

	for(int i = 0; i < used_shelves; ++i){
		rewind(estante[i]);
		if(fread(temp_book.title, sizeof(*temp_book.title), TITLE_SIZE, estante[i])
			!= sizeof(*temp_book.title)*TITLE_SIZE){
				exit(EXIT_FAILURE);
		}
		fprintf(index, "%s ", temp_book.title);
		fseek(estante[i], -BOOK_SIZE, SEEK_END);
		if(fread(temp_book.title, sizeof(*temp_book.title), TITLE_SIZE, estante[i])
			!= sizeof(*temp_book.title)*TITLE_SIZE){
				exit(EXIT_FAILURE);
		}
		fprintf(index, "%s\n", temp_book.title);
	}
	for(int i = 0; i < unused_shelves; ++i){
		fprintf(index, "#\n");
	}
}

int search_index(FILE *index, char *search_word){

	int current_shelf = 0;
	char word_0[TITLE_SIZE];
	char word_1[TITLE_SIZE];
	rewind(index);

	while(fscanf(index, "%s %s", word_0, word_1) == 2){
		if(strcmp(search_word, word_0) >= 0 && strcmp(search_word, word_1) <= 0){
			return current_shelf;
		}
		else if(strcmp(search_word, word_0) > 0 && strcmp(search_word, word_1) > 0){
			++current_shelf;
		}
	}
	return -1;
}

int binary_search_shelf(FILE *shelf, char *search_word){

	book current_book;
	int l = 0;
	int r = books_in_shelf(shelf);

	while(l <= r){

		int m = l + (r-l)/2;

		fseek(shelf, m * sizeof(book), SEEK_SET);
		if(fread(current_book.title, sizeof(*current_book.title), TITLE_SIZE, shelf)
			!= sizeof(*current_book.title)*TITLE_SIZE){
				exit(EXIT_FAILURE);
		}

		if(strcmp(current_book.title, search_word) == 0){
			if(fread(&current_book.available, sizeof(current_book.available), 1, shelf)
				!= sizeof(current_book.available)){
					exit(EXIT_FAILURE);
			}

			if((int) current_book.available == '1'){
				return m;	
			}
			else return -2;
		}
		else if(strcmp(current_book.title, search_word) < 0){
			l = m + 1;
		}
		else r = m - 1;
	}

	return -1;
}

void answer_requests(int num_queries, FILE* index, FILE **estante){
	
	char search_word[51];

	for (int i = 0; i < num_queries; ++i){
		if(scanf("%s", search_word) != 1){
			exit (EXIT_FAILURE);
		}

		int shelf = search_index(index, search_word);

		if(shelf < 0){
			printf("livro nao encontrado\n");
		}
		else{
			int book_status = binary_search_shelf(estante[shelf], search_word);

			if(book_status == -2){
				printf("emprestado\n");
			}
			else if(book_status == -1){
				printf("livro nao encontrado\n");
			}
			else printf("disponivel na posicao %d na estante %d\n", book_status, shelf);
		}
	}
}

void free_shelves(FILE **estante, int num_shelves){

	for(int i = 0; i < num_shelves; ++i){
		fclose(estante[i]);
	}
	free(estante);
}