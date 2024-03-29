
#ifndef FTNM_H
# define FTNM_H

#include <ar.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

# define ERROR_MMAP "Error mmap failed."
# define ERROR_MALLOC "Error malloc failed."
# define ERROR_ELF_CLASS "Invalid ELF class"
# define ERROR_NO_SYM "no symbols"
# define HELPER "ft_nm [option(s)] [file(s)]\n\
List symbols in [file(s)] (a.out by default).\n\n\
Options:\n\
-r\tReverse the order of the sort (whether numeric or alphabetic); let the last come first.\n\
-u\tDisplay only undefined symbols\n\
-p\tDo not sort the symbols\n\
-g\tDisplay only external symbols.\n\
-h\tDisplay this information"
# define ERRORS_OFFSET "ft_nm invalid shname offset is greater than st_size."
# define ERROR_E_SHOFF_TO_BIG "e_shoff to big."
# define ERROR_E_SHOFF_TO_LOW "e_shoff <= 0."
# define ERROR_E_SNUM_TO_LOW "e_shnum <= 0."
# define ERROR_E_SHSTR_TO_BIG "e_shstrndx to big."
# define ERROR_BAD_ENDIAN "Invalid endian encoding byte index."
# define ERROR_OPEN "Error on open, verify if file exists and available."
# define ERROR_ST_SIZE "Error file is empty."
# define ERROR_AR_TRUNCATED "Error ar file is truncated."
# define ERROR_FILE_FORMAT "Error file format not recognized."
# define MESSAGE_UNKNOW_FILENAME "? unknow filename ?"


typedef struct		s_ft_nm_symbols
{
    char            *name;
    char            type_char;
    unsigned long   b;
}                   t_ft_nm_symbols;


typedef struct  s_ft_nm_options
{
    short           should_reverse;
    short           undefined_only;
    short           display_all;
    short           no_sort;
    short           extern_only;
    short           display_help;
}               t_ft_nm_options;


typedef struct  s_ft_nm_ctx
{
    int             fd;
    off_t           st_size;
    char            *ptr;
    char            *filename;
    short           should_exit;
    short           current_ar;
}               t_ft_nm_ctx;

// ---
// utils.c
// ---
void            get_formated_sym_value(unsigned int st_value, char *str, int size);
void            print_error(char *message, t_ft_nm_ctx *context);
unsigned int    swap32(unsigned int num);
size_t          swap64(size_t val);
int             get_comp_sort_sym(char *low_before, char *low_current, char *raw_before, char *raw_current, unsigned int st_value_before, unsigned int st_value_current, t_ft_nm_options *options);


// ---
// elf64.c
// ---
void            process_64(char *ptr, Elf64_Ehdr *ehdr, t_ft_nm_options *options, t_ft_nm_ctx *context);

// ---
// elf32.c
// ---
void            process_32(char *ptr, Elf32_Ehdr *ehdr, t_ft_nm_options *options, t_ft_nm_ctx *context);

// ---
// ar.c
// ---
void            process_ar(char *ptr, t_ft_nm_options *options, t_ft_nm_ctx *context);

#endif