#include "ft_nm.h"
#include "libft.h"


int    parse_arg(char **argv, int argc, t_ft_nm_options *options)
{
    int i = 0;
    int j = 0;
    for (; i < argc; i++)
    {
        j++;
        if (ft_strcmp(argv[i], "-r") == 0)
            options->should_reverse = 1;
        else if (ft_strcmp(argv[i], "-u") == 0)
            options->undefined_only = 1;
        else if (ft_strcmp(argv[i], "-a") == 0)
            options->display_all = 1;
        else if (ft_strcmp(argv[i], "-p") == 0)
            options->no_sort = 1;
        else if (ft_strcmp(argv[i], "-g") == 0)
            options->extern_only = 1;
        else if (ft_strcmp(argv[i], "-h") == 0)
            options->display_help = 1;
        else
            j--;
    }

    return j + 1;
}


int main(int argc, char* argv[]) {
    struct stat st;
    char *ptr;
    int num_options = 0;
    t_ft_nm_options  options[1];
    t_ft_nm_ctx      context[1];
    
    ft_bzero(options, sizeof(t_ft_nm_options));
    ft_bzero(context, sizeof(t_ft_nm_ctx));

    num_options = parse_arg(argv, argc, options);
    if (options->display_help)
    {
        ft_putendl(HELPER);
        exit(0);
    }
    if (argc == 1 || num_options == argc)
        context->fd = open("a.out", O_RDONLY);
    else 
        context->fd = open(argv[1], O_RDONLY);
    

    fstat(context->fd, &st);
    ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, context->fd, 0);
    

    context->st_size = st.st_size;
    context->ptr = ptr;

    if (ptr == MAP_FAILED)
        print_error(ERROR_MMAP, context);
    if (ptr[EI_CLASS] == ELFCLASS32) {
        Elf32_Ehdr* elf_header = (Elf32_Ehdr*) ptr;
        process_32(ptr, elf_header, options, context);
    } else if (ptr[EI_CLASS] == ELFCLASS64) {
        Elf64_Ehdr* elf_header = (Elf64_Ehdr*) ptr;
        process_64(ptr, elf_header, options, context);
    } else {
        print_error(ERROR_ELF_CLASS, context);
    }

    munmap(ptr, st.st_size);
    close(context->fd);
    return 0;
}


