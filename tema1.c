#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 20000

typedef struct Elem {		//Un element dintr-un bucket

	int key;
	char *val;
	struct Elem *next;

} elem;

typedef struct Hash {		//Hash-ul propriu-zis

	long hash_size;
	elem **buckets;

} my_hash;

my_hash *hash_init(long hash_size)
{		//Initialixarea hash-ului si alocarea memoriei
	int i;
	my_hash *ht = NULL;

	if (hash_size <= 0)
		return NULL;

	ht = (my_hash *)malloc(sizeof(my_hash));
	ht->hash_size = hash_size;
	ht->buckets = (elem **)malloc(hash_size * sizeof(elem *));

	for (i = 0 ; i < hash_size ; i++) {
		ht->buckets[i] = (elem *)malloc(sizeof(elem));
		ht->buckets[i]->next = NULL;
	}

	return ht;
}

void hash_destroy(my_hash *ht)
{
	int i;

	for (i = 0 ; i < ht->hash_size ; i++)
		free(ht->buckets[i]);

	free(ht->buckets);
	free(ht);
}

int exists(elem *bucket, char word[])
{
	elem *head = bucket;

	while (head->next != NULL) {
		head = head->next;
		if (strcmp(head->val, word) == 0)
			return 1;
	}

	return 0;
}

void hash_add(my_hash *ht, char word[])
{
	int index, i;
	elem *new;
	elem *head = NULL;
	elem *curr = NULL;

	new = (elem *)malloc(sizeof(elem));
	new->val = (char *)malloc((strlen(word) + 1) * sizeof(char));

	index = hash(word, ht->hash_size);
	strcpy(new->val, word);
	new->key = index;
	new->next = NULL;

	head = ht->buckets[index];

	curr = head;

	if (strcmp(word, "") == 0) {
		perror("Invalid argument!\n");
		hash_destroy(ht);
		exit(-1);
	}

	if (exists(head, word) == 0) {

		if (head->next == NULL) {
			head->next = new;
		} else {
			while (curr->next != NULL) {
				curr = curr->next;
			}
			curr->next = new;
		}
	}
}

void hash_remove(my_hash *ht, char word[])
{
	int index;

	elem *curr = NULL;
	elem *prev = NULL;

	index = hash(word, ht->hash_size);

	prev = ht->buckets[index];
	curr = prev->next;

	if (exists(prev, word) == 1) {

		while (curr != NULL) {
			if (strcmp(curr->val, word) == 0) {
				prev->next = curr->next;
				free(curr->val);
				free(curr);
			}

			curr = curr->next;
			prev = prev->next;
		}
	}
}

void hash_find(my_hash *ht, char buffer[])
{
	int index, rez;
	char *ch;
	char *word;
	char *file_name;

	ch = strtok(buffer, " \n");
	word = strtok(NULL, " \n");
	file_name = strtok(NULL, " \n");

	index = hash(word, ht->hash_size);
	rez = exists(ht->buckets[index], word);

	if (file_name == NULL) {
		if (rez == 1)
			printf("True\n");
		else
			printf("False\n");
	} else {
		FILE *f = fopen(file_name, "a");

		if (rez == 1)
			fprintf(f, "True\n");
		else
			fprintf(f, "False\n");

		fclose(f);
	}
}

void hash_clear(my_hash *ht)
{
	int i;
	elem *curr;
	elem *prev;

	for (i = 0 ; i < ht->hash_size ; i++) {
		curr = ht->buckets[i];
		if (curr->next != NULL) {
			prev = curr;
			curr = curr->next;

			while (curr != NULL) {
				prev = curr;
				curr = curr->next;
				free(prev);
			}
		}
		ht->buckets[i]->next = NULL;
	}
}

void hash_print_bucket(my_hash *ht, char buffer[])
{
	int index, i;
	char *ch;
	char *idx;
	char *file_name;
	elem *head;

	ch = strtok(buffer, " \n");
	idx = strtok(NULL, " \n");

	for (i = 0 ; i < strlen(idx) ; i++) {
		if (!isdigit(idx[i])) {
			perror("Invalid argument!");
			hash_destroy(ht);
			exit(-1);
		}

	}

	file_name = strtok(NULL, " \n");
	index = atoi(idx);
	head = ht->buckets[index];


	if (file_name == NULL) {

		if (head->next != NULL) {
			while (head->next != NULL) {
				head = head->next;
				printf("%s", head->val);
				if (head->next == NULL) {
					printf("\n");
					break;
				} else {
					printf(" ");
				}
			}
		}
	} else {
		FILE *f = fopen(file_name, "a");

		if (head->next != NULL) {
			while (head->next != NULL) {
				head = head->next;
				fprintf(f, "%s", head->val);
				if (head->next == NULL) {
					fprintf(f, "\n");
					break;
				} else {
					fprintf(f, " ");
				}
			}
		}

		fclose(f);
	}
}

void hash_print(my_hash *ht, char buffer[])
{
	int i;
	elem *head;
	char *ch;
	char *file_name;

	ch = strtok(buffer, " \n");
	file_name = strtok(NULL, " \n");

	if (file_name == NULL) {
		for (i = 0 ; i < ht->hash_size ; i++) {
			head = ht->buckets[i];

			if (head->next != NULL) {
				while (head->next != NULL) {
					head = head->next;
					printf("%s", head->val);
					if (head->next == NULL) {
						printf("\n");
						break;
					} else {
						printf(" ");
					}
				}
			}
		}
	} else {
		FILE *f = fopen(file_name, "a");
		if (f != NULL) {
			for (i = 0 ; i < ht->hash_size ; i++) {
				head = ht->buckets[i];

				if (head->next != NULL) {
					while (head->next != NULL) {
						head = head->next;
						fprintf(f, "%s", head->val);
						if (head->next == NULL) {
							fprintf(f, "\n");
							break;
						} else {
							fprintf(f, " ");
						}
					}
				}
			}
			fclose(f);
		} else {
			perror("Invalid file!");
			hash_destroy(ht);
			exit(-1);
		}
	}
}

my_hash *hash_double(my_hash *ht)
{
	int i;
	elem *head;

	my_hash *ht2 = hash_init(ht->hash_size * 2);

	for (i = 0 ; i < ht->hash_size ; i++) {
		head = ht->buckets[i];

		if (head->next != NULL) {
			while (head->next != NULL) {
				head = head->next;
				hash_add(ht2, head->val);
			}
		}
	}

	hash_clear(ht);
	hash_destroy(ht);

	return ht2;
}

my_hash *hash_halve(my_hash *ht)
{
	int i;
	elem *head;

	my_hash *ht2 = hash_init(ht->hash_size / 2);

	for (i = 0 ; i < ht->hash_size ; i++) {
		head = ht->buckets[i];

		if (head->next != NULL) {
			while (head->next != NULL) {
				head = head->next;
				hash_add(ht2, head->val);
			}
		}
	}

	hash_clear(ht);
	hash_destroy(ht);

	return ht2;
}

my_hash *process_command(my_hash *ht, char buffer[])
{
	char command[10];
	char arg[BUFSIZE];

	sscanf(buffer, "%s %s\n", command, arg);

	if (strcmp(command, "add") == 0) {
		hash_add(ht, arg);
		return ht;
	}
	if (strcmp(command, "remove") == 0) {
		hash_remove(ht, arg);
		return ht;
	}
	if (strcmp(command, "find") == 0) {
		hash_find(ht, buffer);
		return ht;
	}
	if (strcmp(command, "clear") == 0) {
		hash_clear(ht);
		return ht;
	}
	if (strcmp(command, "print_bucket") == 0) {
		hash_print_bucket(ht, buffer);
		return ht;
	}
	if (strcmp(command, "print") == 0) {
		hash_print(ht, buffer);
		return ht;
	}
	if (strcmp(command, "resize") == 0) {
		if (strcmp(arg, "double") == 0) {
			return hash_double(ht);
		} else if (strcmp(arg, "halve") == 0) {
			return hash_halve(ht);
		}
	}
	if (strcmp(command, "quit") == 0) {
		printf("Bye!\n");
		hash_destroy(ht);

		exit(0);
	} else {
		perror("Wrong command!\n");
		hash_destroy(ht);
		exit(-1);
	}
}

int main(int argc, char **argv)
{
	int i;
	long hash_size = 0;
	char buffer[BUFSIZE];
	my_hash *ht;


	if (argc == 1) {			// Nici un argument
		printf("Usage: ./tema1 hash_size [input_file1] [input_file2] ...\n");
		return 0;
	}
	if (argc == 2) {			// Un argument

		hash_size = atol(argv[1]);
		ht = hash_init(hash_size);

		while (fgets(buffer, BUFSIZE, stdin) != NULL) {
			ht = process_command(ht, buffer);
		}

	} else {			// Mai multe argumente
		hash_size = atol(argv[1]);
		ht = hash_init(hash_size);

		for (i = 2 ; i < argc ; i++) {
			FILE *f = fopen(argv[i], "r");

			if (f != NULL) {
				while (fgets(buffer, BUFSIZE, f) != NULL) {

					if (strcmp(buffer, "\n") != 0)
						ht = process_command(ht, buffer);
				}
				fclose(f);
			}
		}
	}

	return 0;
}