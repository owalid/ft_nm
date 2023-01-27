#include "ft_nm.h"
#include "libft.h"



void    process_64(char *ptr, Elf64_Ehdr *ehdr)
{
    // printf("ehdr->e_shnum = ")
    Elf64_Shdr* shdr = (Elf64_Shdr*) ((char*) ptr + ehdr->e_shoff); // get the section header
    Elf64_Shdr *symtab, *strtab; // declare symbol tab and str tab
    char *shstrtab = (char*)(ptr + shdr[ehdr->e_shstrndx].sh_offset); // get the section header str tab


    for (size_t i = 0; i < ehdr->e_shnum; i++) // loop over header 
    {
        if (shdr[i].sh_size) {
            // printf("%s\n", &shstrtab[shdr[i].sh_name]);

            if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".symtab") == 0) // get symtab
                symtab = (Elf64_Shdr*) &shdr[i];
            else if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".strtab") == 0) // get strtab
                strtab = (Elf64_Shdr*) &shdr[i];
        }
    }

    // printf("\n\n");
    Elf64_Sym *sym = (Elf64_Sym*) (ptr + symtab->sh_offset); // get symbols
    char* str = (char*) (ptr + strtab->sh_offset); // get str in strtab

    int len_array = 0, i = 0, j = 0;

    for (i = 0; i < symtab->sh_size / sizeof(Elf64_Sym); i++)
    {
        if (str + sym[i].st_name && ft_strlen(str + sym[i].st_name) && sym[i].st_info != 4)
            len_array++;
    }
    // printf("here\n");
    Elf64_Sym array[len_array+1];

    ft_bzero(&array, sizeof(Elf64_Sym)*(len_array+1));

    // printf("sh_size = %lu, sh_offset = %lu\n\n", symtab->sh_size / sizeof(Elf64_Sym), symtab->sh_offset/ sizeof(Elf64_Sym));

    for (i = 0, j = 0; i < symtab->sh_size / sizeof(Elf64_Sym); i++) { // loop over symtab to get symbol name
        //? -u option ?
        // if (str + sym[i].st_name && ft_strlen(str + sym[i].st_name) && sym[i].st_info == 18 && sym[i].st_other == 0 && sym[i].st_value == 0)
        //? ?

        if (str + sym[i].st_name && ft_strlen(str + sym[i].st_name) && sym[i].st_info != 4)
            array[j++] = sym[i];
    }

    ft_sort_sym_array(array, len_array, str);

    // printf("len_array = %d\n", len_array);
    for (i = 0; i < len_array; i++)
        print_symbol(array[i], shdr, str);

}
