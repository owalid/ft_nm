
#ifndef FTNM_H
# define FTNM_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

# define ERROR_MMAP "Error mmap failed."
# define ERROR_ELF_CLASS "Invalid ELF class"
# define ERROR_NO_SYM "No symbol"


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
}               t_ft_nm_options;


typedef struct  s_ft_nm_ctx
{
    int             fd;
    off_t           st_size;
    char            *ptr;
}               t_ft_nm_ctx;


// static t_ft_nm_ctx      context[0];

// ---
// utils.c
// ---

void            get_formated_sym_value(unsigned int st_value, char *str, int size);
void            print_error(char *message, t_ft_nm_ctx *context);
unsigned int    swap32(unsigned int num);
size_t          swap64(size_t val);

// ! DEBUG NEED TO REMOVE
void    debug_print_options();

// void		    ft_sort_sym_array(Elf64_Sym *tab, int size, char *str);
// void            print_symbol(Elf64_Sym sym, Elf64_Shdr *shdr, char *str);


// ---
// elf64.c
// ---
void            process_64(char *ptr, Elf64_Ehdr *ehdr, t_ft_nm_options *options, t_ft_nm_ctx *context);

// ---
// elf32.c
// ---
void            process_32(char *ptr, Elf32_Ehdr *ehdr, t_ft_nm_options *options, t_ft_nm_ctx *context);


#endif