
#ifndef FTNM_H
# define FTNM_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <elf.h>

typedef struct		s_ft_nm_symbols
{
    char            *name;
    char            type_char;
    unsigned long   b;
}                   t_ft_nm_symbols;

#endif