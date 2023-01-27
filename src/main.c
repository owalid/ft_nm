#include "ft_nm.h"
#include "libft.h"


int    parse_arg(char **argv, int argc, t_ft_nm_options *options)
{
    int i = 0;
    for (; i < argc; i++)
    {
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
    }

    return i + 1;
}


int main(int argc, char* argv[]) {
    int fd = open(argv[1], O_RDONLY);
    struct stat st;
    char *ptr;

    ft_bzero(options, sizeof(t_ft_nm_options));

    parse_arg(argv, argc, options);

    fstat(fd, &st);
    ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    if (ptr[EI_CLASS] == ELFCLASS32) {
        Elf32_Ehdr* elf_header = (Elf32_Ehdr*) ptr;
        process_32(ptr, elf_header);
        printf("\nelf 32\n");
    } else if (ptr[EI_CLASS] == ELFCLASS64) {
        Elf64_Ehdr* elf_header = (Elf64_Ehdr*) ptr;
        process_64(ptr, elf_header);
        // printf("\nelf 64\n");
    } else {
        printf("Invalid ELF class\n");
    }

    munmap(ptr, st.st_size);
    close(fd);
    return 0;
}


