#include "ft_nm.h"
#include "libft.h"

void print_obj_file(char *name, char *ar_symtab, t_ft_nm_ctx *context)
{
    /*
        The ar_name field contains the name directly, and is terminated by a slash (/)

        If the member's name is longer than 15 characters,
            ar_name contains a slash (/) followed by offset in the archive string table
    */
    size_t cpt = 0;
    ssize_t offset_longfunc = ft_atol(name + 1); // get offset in case of member name is loger than 15 chars

    if (name[0] == '/' && ar_symtab) // if len(name) > 15 chars
    {
        name = ar_symtab + offset_longfunc;
        cpt = 0;
    }

        while(name[cpt++] != '/') // loop until get /
        ;
    
    if (--cpt > 0)
    {
        if (context->filename != NULL)
        {
            free(context->filename);
            context->filename = NULL;
        }
        
        if ((context->filename = ft_strndup(name, cpt)) == NULL)
        {
            print_error(ERROR_MALLOC, context);
            return;
        }
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
    short saved_should_exit = context->should_exit;

    ft_memcpy(&current_ar, ptr + SARMAG, sizeof(current_ar));
    ptr += SARMAG; // pass magic string
	size -= SARMAG; // pass magic string

    ft_putchar('\n');
    while (size >= sizeof(current_ar))
    {
        ft_memcpy(&current_ar, ptr, sizeof(current_ar));
        ptr += sizeof(current_ar);

        ar_size = ft_atoi(current_ar.ar_size); // get current ar_size as number

        if (size < (ar_size + sizeof(current_ar)))
        {
            print_error(ERROR_AR_TRUNCATED, context);
            return;
        }
        is_last = (size - ar_size) <= sizeof(current_ar);
        size -= ar_size + sizeof(current_ar);
        flag = (ar_size > EI_CLASS && (ptr[EI_CLASS] == ELFCLASS32 || ptr[EI_CLASS] == ELFCLASS64)); // get if current ptr is an ELF32 or ELF64
        short need_exit = is_last && saved_should_exit;
        ft_memcpy(&context->should_exit, &need_exit, sizeof(short));
        context->should_exit = is_last && saved_should_exit;
        if (flag)
        {
            if (!current_ar.ar_name)
                return;
            print_obj_file(current_ar.ar_name, ar_symtab, context);
        
            // process as elf32 or elf64
            if (ptr[EI_CLASS] == ELFCLASS32) {
                Elf32_Ehdr* elf_header = (Elf32_Ehdr*) ptr;
                process_32(ptr, elf_header, options, context);
            } else if (ptr[EI_CLASS] == ELFCLASS64) {
                Elf64_Ehdr* elf_header = (Elf64_Ehdr*) ptr;
                process_64(ptr, elf_header, options, context);
            }
            if (!is_last && flag) // end
                ft_putchar('\n');
        } else {
            print_error(ERROR_FILE_FORMAT, context);
        }

        if (!ar_symtab
            && (ft_strncmp("/SYM64/         ", current_ar.ar_name, 16) == 0 // 64 bits
                || ft_strncmp("//              ", current_ar.ar_name, 16) == 0)) // 32 bits
            ar_symtab = ptr; // update archive string table

        ptr += ar_size; // update ptr
    }
    context->should_exit = saved_should_exit;
}