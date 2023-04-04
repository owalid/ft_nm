#include "ft_nm.h"
#include "libft.h"


void print_obj_file(char *name, char *ar_symtab)
{
    /*
        The ar_name field contains the name directly, and is terminated by a slash (/)

        If the member's name is longer than 15 characters,
            ar_name contains a slash (/) followed by offset in the archive string table
    */
    size_t cpt = 0;
    ssize_t offset_longfunc = ft_atol(name+1);

    if (name[0] == '/' && ar_symtab)
    {
        name = ar_symtab + offset_longfunc;
        cpt = 0;
    }

    while(name[cpt++] != '/')
        ;
    
    if (--cpt > 0)
    {
        write(1, name, cpt);
        ft_putstr(":\n");
    }
}

void process_ar(char *ptr, t_ft_nm_options *options, t_ft_nm_ctx *context)
{
    ssize_t ar_size = 0;
    short is_last = 0, flag = 0;
    struct ar_hdr current_ar;
    off_t size = context->st_size;
    char *ar_symtab = NULL;

    ft_memcpy(&current_ar, ptr + SARMAG, sizeof(current_ar));
    ptr += SARMAG; // pass magic string
	size -= SARMAG; // pass magic string

    ft_putchar('\n');
    while (size >= sizeof(current_ar))
    {
        ft_memcpy(&current_ar, ptr, sizeof(current_ar));
        ptr += sizeof(current_ar);

        ar_size = ft_atoi(current_ar.ar_size); // get current ar_size as number
        is_last = (size - ar_size) <= sizeof(current_ar);
        size -= ar_size + sizeof(current_ar);
        flag = (ptr[EI_CLASS] == ELFCLASS32 || ptr[EI_CLASS] == ELFCLASS64);

        if (flag)
            print_obj_file(current_ar.ar_name, ar_symtab);

        if (ptr[EI_CLASS] == ELFCLASS32) {
            Elf32_Ehdr* elf_header = (Elf32_Ehdr*) ptr;
            process_32(ptr, elf_header, options, context);
        } else if (ptr[EI_CLASS] == ELFCLASS64) {
            Elf64_Ehdr* elf_header = (Elf64_Ehdr*) ptr;
            process_64(ptr, elf_header, options, context);
        }

        if (!is_last && flag)
            ft_putchar('\n');
        if (!ar_symtab
            && (ft_strncmp("/SYM64/         ", current_ar.ar_name, 16) == 0// 64
                || ft_strncmp("//              ", current_ar.ar_name, 16) == 0)) // 32
            ar_symtab = ptr; // get archive string table

        ptr += ar_size;
    }
}