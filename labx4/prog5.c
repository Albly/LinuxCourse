/* program counts number of occurences of different words in a given file as arg*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>                      // for open sys call
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define	HASHTAB_SIZE		((1<<9)-1)	// initial table size
#define HASHSEED		0x12345678	    // hash starting seed
#define NUMBUCKETS		10		        // number of collisions
#define	MAXWORD_LEN		20		        // max word length

typedef unsigned int uint32_t;

typedef struct hashdata{
	long count;		                // number of occurencies
	long colis;		                // count collisions
	char word[MAXWORD_LEN];          // word 
} hashdata_type;

static long _collisions = 0; 					    // count the total number of collisions
static hashdata_type *hashtabs[NUMBUCKETS] = {0};	// exapand hash 

void usage(int argc, char **argv){
	fprintf(stderr,"usage: %s <asci-file>\n",argv[0]);
	exit(0);
}

char *get_word(char **ps){               
    // search the data for the end of word
	char *p1, *p2;

	p1 = *ps;
	while ( ! isalnum(*p1) && *p1 ) p1++;    //skip all not letters
	*ps = p2 = p1;
	while (   isalnum(*p2) && *p2 ) p2++;  

// if we are in the debuging mode
#ifdef DEBUG
	{
		char word[50];
		int  wsize = p2 - p1;       
		strncpy(word, p1, wsize);
		word[wsize] = '\0';
		printf("word found: %d -%s-\n",wsize, word);
	}
#endif
	return p2;
}

uint32_t MurmurOAAT_32(const char* str, int strlen, uint32_t h){
    // One-byte-at-a-time hash based on Murmur's mix
    	int i;

        for (i=0; i < strlen; i++ ) { 
                     h ^= *str++;
                     h *= 0x5bd1e995;
                     h ^= h >> 15;
    	}
    	return h;
}

hashdata_type * allhash(){
	hashdata_type * hashtable;

	if ( ( hashtable = malloc(sizeof(hashdata_type) * HASHTAB_SIZE) ) == NULL ) { perror ("malloc hashdata"); exit(4); }
	else memset(hashtable, (int) 0, sizeof(hashdata_type) * HASHTAB_SIZE);
	return hashtable;
}

void hashfree(){
	int i;
	for ( i=0; i < NUMBUCKETS; i++) if ( hashtabs[i] != NULL ) free(hashtabs[i]);
}

void printinfo(){
	hashdata_type *htab;
	int i,j, newl;
	int sparce[NUMBUCKETS] = {0};

	printf("%32s   Freq  Colis\n","word");
	for ( i=0; i < HASHTAB_SIZE; i++ ) {
		newl = 0;
		for (j=0; j < NUMBUCKETS; j++) { 
			if ( (htab = hashtabs[j]) == NULL) break;
			if ( htab[i].count > 0 ) {
				printf("%16s    %d    %d", htab[i].word,htab[i].count, htab[i].colis);
				sparce[j]++;
				newl = 1;
			}
		}
		if ( newl ) printf("\n");
	}
	for (j=0; j < NUMBUCKETS; j++) { 
		printf(" %.2f ", (double)sparce[j]/HASHTAB_SIZE);
	}
	printf("collisions: %ld\n",_collisions);
}


long process_words(char *data){
	char *p1, *p2;          // pointers of strting and endings
	hashdata_type *htab;
	long word_count = 0;
	uint32_t hashvalue;
	int hindex;

	p2 = data;

	do {
		int wsize;
		int i;
		p1 = p2;
		p2 = get_word(&p1);
		if ( p2 == NULL ) break;
		wsize = ((p2 - p1) < MAXWORD_LEN ? (p2-p1): (MAXWORD_LEN-1));
		
		word_count++;

		hashvalue = MurmurOAAT_32(p1, wsize, HASHSEED);
		hindex = hashvalue & HASHTAB_SIZE;

		for(i=0; i < NUMBUCKETS; i++) { 

			htab = hashtabs[i]; 
			if ( htab == NULL ) htab = hashtabs[i] = allhash();	

			if( htab[hindex].count == 0 ) {
				strncpy(htab[hindex].word, p1, wsize);
				htab[hindex].word[wsize] = '\0';
				htab[hindex].count++;
				break; 
			}
			else {
				if ( strncmp(htab[hindex].word, p1, wsize ) == 0 ) {
					htab[hindex].count++;
					break;
				}
				else {
					htab[hindex].colis++;
				}
			}
		}
		if (i == NUMBUCKETS) _collisions++;

	} while ( p2 > p1 );

	return word_count;
}

int main(int argc, char *argv[]){

	const long pagesize = sysconf(_SC_PAGESIZE);   // get the default page size

	char  *data;
	struct stat file_status;
	int    fd;
	char * file_name;
	size_t alloc_bytes;
	long   pagesdef;

	long   word_count = 0;

	if(argc <= 1) usage(argc,argv); 
	else file_name = argv[1];

	if ( ( fd = open(file_name,O_RDONLY) ) < 0 ) { perror("open"); exit(1); }
	if ( fstat(fd, & file_status) < 0) { perror("fstat"); exit(1); }
	pagesdef = 1 + (file_status.st_size + pagesize -1)/pagesize;    

	alloc_bytes = pagesdef * pagesize;             // allocate memory with integral #pages
	data = (char *) mmap(NULL, alloc_bytes, PROT_READ, MAP_PRIVATE, fd, (off_t)0);
	if ( data == NULL ) { perror("mmap"); exit(1); }
	else close(fd);


	word_count = process_words(data); // count words


	printf("number of words in file %s (size %ld): %ld\n",file_name, file_status.st_size, word_count);
	printinfo();
	

	hashfree();
	munmap(data,alloc_bytes);
	exit(0);
}
