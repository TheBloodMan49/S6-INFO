/*
 * Analyse n-grams of a text file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "string.h"

size_t Strlen(const char *s);

/* Some statistics */
int wcnt = 0;       /* Number of n-grams */
int ucnt = 0;       /* Number of unique n-grams */
int mcnt = 0;       /* Count of most frequent n-grams */
int scnt = 0;       /* Count of singleton n-grams */
char *mstring = ""; /* Most frequent n-grams */
int llen = 0;       /* Length of the longest n-gram */
char *lstring = ""; /* Longest n-gram */
int lcnt = 0;       /* Number of n-grams having maximum length */

/* Convert string to lower case */
void lower(char *s) {
  int i;
  int len = Strlen(s);

  for (i = 0; i < len; i++)
    if (s[i] >= 'A' && s[i] <= 'Z')
      s[i] -= ('A' - 'a');
}

/* Compute length of string */
size_t Strlen(const char *s) { return strlen(s); }

/* The hash table */

typedef struct Helem {
  char *word;
  int freq;
  struct Helem *next;
} h_rec, *h_ptr;

h_ptr *htable;
int tsize;

static void new_table(int size) {
  tsize = size;
  htable = (h_ptr *)calloc(size, sizeof(h_ptr));
  if (!htable) {
    fprintf(stderr, "Couldn't allocate hash array, exiting\n");
    exit(1);
  }
}

static h_ptr new_elem(char *s) {
  h_ptr result = (h_ptr)malloc(sizeof(h_rec));
  int wlen = Strlen(s);
  if (wlen > llen) {
    lstring = s;
    llen = wlen;
    lcnt = 1;
  } else if (wlen == llen)
    lcnt++;
  if (!result) {
    fprintf(stderr, "Couldn't allocate hash element, exiting\n");
    exit(1);
  }

  result->word = s;
  result->freq = 1;
  return result;
}

/* Hash function */
unsigned h_fun(char *s) {
    unsigned val = 0;
    int c;
    while ((c = *s++))
      val = ((val ^ c) << 4) | ((val >> 28) & 0xF);
    return val % tsize;
}

/* Better hash function */
unsigned h_fun2(char *s) {
  unsigned val = 5381;
  int c;
  while ((c = *s++))
    val = ((val << 5) + val) + c;
  return val % tsize;
}

unsigned MurmurOAAT32 ( const char * s)
{
  unsigned h = 3323198485ul;
  for (;*s;++s) {
    h ^= *s;
    h *= 0x5bd1e995;
    h ^= h >> 15;
  }
  return h % tsize;
}

char *save_string(char *s) {
  char *result = (char *)malloc(Strlen(s) + 1);
  if (!result) {
    fprintf(stderr, "Couldn't allocate space for string, exiting\n");
    exit(1);
  }
  strcpy(result, s);
  return result;
}

/* Search for string in the linked list. If the string is found, its frequency is updated; otherwise, it is added to the list. Returns the updated list */
h_ptr find_elem(h_ptr ls, char *s) {
  h_ptr elem = ls;
  h_ptr last = NULL;
  for (elem = ls; elem; elem = elem->next) {
    char *word = elem->word;
    // Check if the word matches the string
    if (strcmp(s, word) == 0) {
      int freq = ++elem->freq;
      if (freq > mcnt) {
        mcnt = freq;
        mstring = word;
      }
      return ls;
    }
    last = elem;
  }
  // If no matching element is found, create new element and add to list
  elem = new_elem(save_string(s));
  ucnt++;
  elem->next = NULL;
  if (last) {
    last->next = elem;
    return ls;
  } else
    return elem;
}

/* Comparison function for sorting */
int compare_elem(const void *velem1, const void *velem2) {
  h_ptr elem1 = *(h_ptr *)velem1;
  h_ptr elem2 = *(h_ptr *)velem2;
  return elem2->freq - elem1->freq;
}

/* Sort hash table elements by decreasing frequency */
h_ptr sort_words() {
  h_ptr ls = NULL;
  h_ptr elem;
  h_ptr *array = calloc(ucnt, sizeof(h_ptr));
  int i, j;
  int cnt = 0;
  scnt = 0; /* Count singletons */
  for (i = 0; i < tsize; i++)
    for (elem = htable[i]; elem; elem = elem->next) {
      if (elem->freq == 1)
        scnt++;
      /* Insert element into array in sorted order (insertion sort) */
      for (j = cnt; j > 0 && elem->freq > array[j - 1]->freq; j--)
        array[j] = array[j - 1];
      array[j] = elem;
      cnt++;
    }
  ls = array[0];
  for (j = 0; j < cnt - 1; j++)
    array[j]->next = array[j + 1];
  array[cnt - 1]->next = NULL;
  free((void *)array);
  return ls;
}

void insert_string(char *s) {
  int index;
  lower(s);
  index = MurmurOAAT32(s);
  htable[index] = find_elem(htable[index], s);
}

/* Sort hash table elements by decreasing frequency */
h_ptr sort_words2() {
  h_ptr ls = NULL;
  h_ptr elem;
  h_ptr *array = calloc(ucnt, sizeof(h_ptr));
  int i, j;
  int cnt = 0;
  scnt = 0; /* Count singletons */
  for (i = 0; i < tsize; i++)
    for (elem = htable[i]; elem; elem = elem->next) {
      if (elem->freq == 1)
        scnt++;
      array[cnt] = elem;
      cnt++;
    }
  qsort((void *)array, cnt, sizeof(h_ptr), compare_elem);
  ls = array[0];
  for (j = 0; j < cnt - 1; j++)
    array[j]->next = array[j + 1];
  array[cnt - 1]->next = NULL;
  free((void *)array);
  return ls;
}

/* Extract word from file */
#define BSIZE 1024
char buf[BSIZE];
int bufvalid = 0;
FILE *infile;

void init_token(FILE *in) {
  bufvalid = 0;
  infile = in;
}

static char *skipchar = " \t\n\r.,:;/<>()[]{}?!\"-'\0xc2\0xa0";

/* Keep getting tokens.  Return NULL when no more */
char *get_word() {
  char *s = NULL;
  while (1) {
    if (bufvalid) {
      s = strtok(NULL, skipchar);
      if (s)
        break;
    }
    if (!fgets(buf, BSIZE, infile))
      return NULL;
    bufvalid = 1;
    s = strtok(buf, skipchar);
    if (s)
      break;
  }
  wcnt++;
  return s;
}

#define MAXNG 10

char *get_token(int ngram) {
  /* Buffer of last ngram-1 tokens */
  static char token_buf[MAXNG][BSIZE];
  static int first = 1;
  static int bindex = 0; /* In which buffer to insert next token */
  static char sbuf[BSIZE];
  char *nextpos = sbuf;
  int i;
  int index;

  if (ngram == 1)
    return get_word();
  if (first) {
    /* Get ngram-1 tokens */
    while (bindex < ngram - 1) {
      char *word = get_word();
      if (!word) {
        return NULL; /* Document doesn't have enough tokens */
      }
      strcpy(token_buf[bindex++], word);
    }
    first = 0;
  }
  /* Get new token */
  char *word = get_word();
  if (!word) {
    return NULL; /* No more ngrams */
  }
  strcpy(token_buf[bindex++], word);
  if (bindex >= MAXNG)
    bindex = 0;
  /* Generate string of last ngram-1 tokens */
  index = bindex - ngram;
  if (index < 0)
    index += MAXNG;
  for (i = 0; i < ngram; i++) {
    if (i != 0)
      *nextpos++ = ' ';
    word = token_buf[index];
    strcpy(nextpos, word);
    nextpos += Strlen(word);
    index++;
    if (index >= MAXNG)
      index = 0;
  }
#if 0
     printf("Next n-gram = '%s'\n", sbuf);
#endif
  return sbuf;
}

/* Find statistics of word frequency in document */
void word_freq(FILE *src, int shown, int ngram, int size) {
  char *s;
  h_ptr ls;

  init_token(src);
  new_table(size);

  while ((s = get_token(ngram))) {
    insert_string(s);
  }
  printf("Results:\n");
  if (shown > 0) {
    ls = sort_words2();
    while (ls && shown--) {
      printf("%d\t'%s'\n", ls->freq, ls->word);
      ls = ls->next;
    }
  }
  printf("%d n-grams, %d unique, %d singletons.  Most common (%d) = '%s'.  "
         "Longest (%d) = '%s' (length %d) \n",
         wcnt, ucnt, scnt, mcnt, mstring, lcnt, lstring, llen);
}

/* The code below processes options provided via command line arguments */
typedef enum { INT_OPTION, DOUBLE_OPTION, STRING_OPTION } option_t;

typedef struct {
  char *name;
  option_t type;
  union {
    int *i;
    double *d;
    char **s;
  } valp;
} option_entry;

#define MAX_OPTION 100
static option_entry options[MAX_OPTION];

static int option_count = 0;

/* Determine length of string match */
static int match_length(char *s, char *t) {
  int result = 0;
  while (*s == *t) {
    result++;
    if (*s == '\0')
      break;
    s++;
    t++;
  }
  return result;
}

void usage(char *prog) {
  int j;
  fprintf(stderr, "Usage: %s", prog);
  for (j = 0; j < option_count; j++) {
    switch (options[j].type) {
    case INT_OPTION:
      fprintf(stderr, " [-%s (%d)]", options[j].name, *(options[j].valp.i));
      break;
    case DOUBLE_OPTION:
      fprintf(stderr, " [-%s (%.2f)]", options[j].name, *(options[j].valp.d));
      break;
    case STRING_OPTION:
      fprintf(stderr, " [-%s (%s)]", options[j].name, *(options[j].valp.s));
      break;
    }
  }
  fprintf(stderr, "\n");
  exit(1);
}

/* Determine which option is the best match. */
static int find_option(char *prog, char *name) {
  int sofar = -1;
  int sofar_length = 0;
  int i;
  int ambiguous = 0;
  for (i = 0; i < option_count; i++) {
    int length = match_length(options[i].name, name);
    if (length > sofar_length) {
      sofar = i;
      sofar_length = length;
      ambiguous = 0;
    } else if (length > 0 && length == sofar_length) {
      ambiguous = 1;
    }
  }
  if (sofar_length == 0) {
    fprintf(stderr, "No match found to option '%s'\n", name);
    usage(prog);
  } else if (ambiguous) {
    fprintf(stderr, "Ambiguous option: '%s'\n", name);
    usage(prog);
  }
  return sofar;
}

void add_int_option(char *name, int *var) {
  options[option_count].name = name;
  options[option_count].type = INT_OPTION;
  options[option_count].valp.i = var;
  option_count++;
}

void add_double_option(char *name, double *var) {
  options[option_count].name = name;
  options[option_count].type = DOUBLE_OPTION;
  options[option_count].valp.d = var;
  option_count++;
}

void add_string_option(char *name, char **var) {
  options[option_count].name = name;
  options[option_count].type = STRING_OPTION;
  options[option_count].valp.s = var;
  option_count++;
}

int parse_options(int argc, char *argv[], char *otherargs[]) {
  int i, j;
  int ocount = 0;
  float f;
  char *prog = argv[0];
  for (i = 1; i < argc; i++) {
    /* Look for options */
    if (*argv[i] != '-') {
      /* Must be another class of argument */
      if (otherargs)
        otherargs[ocount] = argv[i];
      ocount++;
      continue;
    }
    j = find_option(prog, argv[i] + 1);
    i++; /* Move to next argument */
    if (i >= argc) {
      fprintf(stderr, "Missing value for option %s\n", options[j].name);
      usage(prog);
    }
    switch (options[j].type) {
    case INT_OPTION:
      if (sscanf(argv[i], "%d", options[j].valp.i) != 1) {
        fprintf(stderr, "Can't parse argument '%s' as integer\n", argv[i]);
        usage(prog);
      }
      break;
    case DOUBLE_OPTION:
      if (sscanf(argv[i], "%f", &f) != 1) {
        fprintf(stderr, "Can't parse argument '%s' as double\n", argv[i]);
        usage(prog);
      }
      *options[j].valp.d = f;
      break;
    case STRING_OPTION:
      *(options[j].valp.s) = argv[i];
      break;
    default:
      fprintf(stderr, "Internal error.  Don't know option type %d\n",
              options[j].type);
      exit(1);
    }
  }
  return ocount;
}

static char *strsave(char *s) {
  char *result = (char *)malloc(strlen(s) + 1);
  strcpy(result, s);
  return result;
}

void parse_option_file(char *prog, FILE *option_file) {
  int j;
  float f;
  char name[50], val[50];
  while (fscanf(option_file, "%s %s", name, val) == 2) {
    if (name[0] != '-') {
      fprintf(stderr, "Need '-' before option '%s'\n", name);
      usage(prog);
    }
    j = find_option(prog, name + 1);
    switch (options[j].type) {
    case INT_OPTION:
      if (sscanf(val, "%d", options[j].valp.i) != 1) {
        fprintf(stderr, "Can't parse argument '%s' as integer\n", val);
        usage(prog);
      }
      break;
    case DOUBLE_OPTION:
      if (sscanf(val, "%f", &f) != 1) {
        fprintf(stderr, "Can't parse argument '%s' as double\n", val);
        usage(prog);
      }
      *options[j].valp.d = f;
      break;
    case STRING_OPTION:
      *(options[j].valp.s) = strsave(val);
      break;
    default:
      fprintf(stderr, "Internal error.  Don't know option type %d\n",
              options[j].type);
      exit(1);
    }
  }
}

void show_options(FILE *outfile) {
  int i;
  fprintf(outfile, "Used options:\n");
  for (i = 0; i < option_count; i++) {
    switch (options[i].type) {
    case INT_OPTION:
      fprintf(outfile, "%s\t%d\n", options[i].name, *(options[i].valp.i));
      break;
    case DOUBLE_OPTION:
      fprintf(outfile, "%s\t%f\n", options[i].name, *(options[i].valp.d));
      break;
    case STRING_OPTION:
      if (*options[i].valp.s)
        fprintf(outfile, "%s\t%s\n", options[i].name, *(options[i].valp.s));
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  int shown = 1;
  int size = 1024;
  int ngram = 1;
  char *fname = NULL;
  FILE *infile = stdin;
  add_int_option("shown", &shown);
  add_int_option("size", &size);
  add_int_option("ngram", &ngram);
  add_string_option("file", &fname);
  parse_options(argc, argv, NULL);
  show_options(stdout);
  if (fname) {
    infile = fopen(fname, "r");
    if (!infile) {
      fprintf(stderr, "Couldn't open file '%s'\n", fname);
      exit(1);
    }
  }
  word_freq(infile, shown, ngram, size);
  printf("Total time = %f seconds\n", (double)clock() / CLOCKS_PER_SEC);
  fclose(infile);
  return 0;
}
