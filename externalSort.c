#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "externalSort.h"

static int cmp(const void *a, const void *b){
	return strcmp((*(book *) a).title,
				  (*(book *) b).title);
}

void create_sorted_temp_files(FILE **temp, FILE *input_file, int max_books_in_memory, int library_size){

	book *array_of_books;
	array_of_books = (book *) malloc(max_books_in_memory * sizeof(book));

	int num_output_file = 0;
	int i;
	for(int j = 0; j < library_size; ++j){
		for(i = 0; i < max_books_in_memory; ++i){
			if(fread(array_of_books[i].title, sizeof(*array_of_books[i].title), TITLE_SIZE, input_file)
				!= TITLE_SIZE){
					break;
			}
			if(fread(&array_of_books[i].available, sizeof(array_of_books[i].available), 1, input_file)
				!= sizeof(array_of_books[i].available)){
					exit(EXIT_FAILURE);
			}
		}

		qsort(array_of_books, i, sizeof(book), cmp);

		for (int k = 0; k < i; ++k){
			fwrite(array_of_books[k].title, sizeof(*array_of_books[k].title), TITLE_SIZE, temp[num_output_file]);
			fwrite(&array_of_books[k].available, sizeof(array_of_books[k].available), 1, temp[num_output_file]);
		}
		num_output_file++;
	}

	free(array_of_books);
}

void merge_sorted_files(FILE **temp, FILE *output, int num_temp_files, int library_size){

	smallest_book current;

	book temp_book;

	for(int i = 0; i < num_temp_files; ++i){
		rewind(temp[i]);
	}

	int sorted_books = 0;
	int *smallest_from = (int *) calloc(num_temp_files, sizeof(int));

	while(sorted_books < library_size){

		strcpy(current.smallest.title, "~");
	
		for(int i = 0; i < num_temp_files; ++i){

			if(fread(temp_book.title, sizeof(*temp_book.title), TITLE_SIZE, temp[i])
				== (sizeof(*temp_book.title) * TITLE_SIZE)){
					if(fread(&temp_book.available, sizeof(temp_book.available), 1, temp[i])
						!= sizeof(temp_book.available)){
							exit(EXIT_FAILURE);
					}
					if(strcmp(current.smallest.title, temp_book.title) >= 0){
						strcpy(current.smallest.title, temp_book.title);
						current.smallest.available = temp_book.available;
						current.position = i;
					}
			}
		}
		
		++smallest_from[current.position];

		fprintf(output, "%s %c\n", current.smallest.title, current.smallest.available);
		
		for(int i = 0; i < num_temp_files; ++i){
			fseek(temp[i], smallest_from[i] * sizeof(book), SEEK_SET);
		}

		++sorted_books;
	}
	free(smallest_from);

}

void sort_all_books(FILE *input_file, FILE *output, int max_books_in_memory, int library_size){

	rewind(input_file);

	int num_temp_files;
	if(library_size % max_books_in_memory == 0){
		num_temp_files = (library_size / max_books_in_memory);
	}
	else num_temp_files = (library_size / max_books_in_memory) + 1;

	FILE **temp;
	temp = (FILE **) malloc(num_temp_files * sizeof(FILE *));
	
	for(int i = 0; i < num_temp_files; ++i){
		temp[i] = tmpfile();
	}

	create_sorted_temp_files(temp, input_file, max_books_in_memory, library_size);
	
	merge_sorted_files(temp, output, num_temp_files, library_size);

	for(int i = 0; i < num_temp_files; ++i){
		fclose(temp[i]);
	}
	free(temp);
}