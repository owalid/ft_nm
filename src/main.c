#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <stdio.h>
#include "libft.h"



void            print_type(Elf64_Sym sym, Elf64_Shdr *shdr)
{
    char  c;

    if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
        c = 'u';
    else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
    {
        c = 'W';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'w';
    }
    else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
    {
        c = 'V';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'v';
    }
    else if (sym.st_shndx == SHN_UNDEF)
        c = 'U';
    else if (sym.st_shndx == SHN_ABS)
        c = 'A';
    else if (sym.st_shndx == SHN_COMMON)
        c = 'C';
    else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
        c = 'B';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
        c = 'R';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
        c = 'D';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
        c = 'T';
    else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
        c = 'D';
    else
        c = 'd';

    printf("%c ", c);
}

void    get_formated_sym_value(unsigned int st_value, char *str)
{
    char *tmp_str = ft_utoa_base(st_value, 16);
    int len_tmp_str = ft_strlen(tmp_str);
    int diff = 16 - len_tmp_str;

    ft_memset(str, '0', diff);
    ft_memcpy(str+diff, tmp_str, len_tmp_str);
    free(tmp_str);
}

void		ft_sort_sym_table(Elf64_Sym *tab, int size, char *str)
{
	int i = 0, j = 0, k = 0, l = 0;
    ssize_t len_current = 0, len_next = 0;
    Elf64_Sym tmp;

	while (i < size)
	{
            len_current = ft_strlen((str + tab[i].st_name));
            len_next = ft_strlen((str + tab[i + 1].st_name));
            char low_current[len_current+1];
            char low_next[len_next+1];

            ft_bzero(low_current, len_current+1);
            ft_bzero(low_next, len_next+1);
            ft_memcpy(low_current, str + tab[i].st_name, len_current);
            ft_memcpy(low_next, str + tab[i + 1].st_name, len_next);

            ft_memcpy(low_current, ft_strlowcase(low_current), len_current);
            ft_memcpy(low_next, ft_strlowcase(low_next), len_next);

            while (j < len_current && !ft_isalnum(low_current[j]))
                j++;
            
            while (k < len_next && !ft_isalnum(low_next[k]))
                k++;

            while(l < len_current && l < len_next && low_current[l + j] == low_next[k + l])
                l++;
    

            if (low_current[j+l] - low_next[k+l] > 0)
            {
                tmp = tab[i];
                tab[i] = tab[i + 1];
                tab[i + 1] = tmp;
                i = 0;
            }
            else
                i++;
    

            j = 0;
            k = 0;
            l = 0;
	}
}

void    print_symbol(Elf64_Sym sym, Elf64_Shdr *shdr, char *str)
{
    char current_sym_value[17];

    if (sym.st_value)
    {
        ft_bzero(current_sym_value, 17);
        get_formated_sym_value(sym.st_value, current_sym_value); 
        printf("%s ", current_sym_value);
    }
    else
        printf("\t\t ");
    print_type(sym, shdr);
    printf("%s\n", str + sym.st_name);
}


void    process_64(char *ptr, Elf64_Ehdr *ehdr)
{
    // printf("ehdr->e_shnum = ")
    Elf64_Shdr* shdr = (Elf64_Shdr*) ((char*) ptr + ehdr->e_shoff); // get the section header
    Elf64_Shdr *symtab, *strtab; // declare symbol tab and str tab
    char *shstrtab = (char*)(ptr + shdr[ehdr->e_shstrndx].sh_offset); // get the section header str tab


    for (size_t i = 0; i < ehdr->e_shnum; i++) // loop over header 
    {
        if (shdr[i].sh_size) {
            printf("%s\n", &shstrtab[shdr[i].sh_name]);

            if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".symtab") == 0) // get symtab
                symtab = (Elf64_Shdr*) &shdr[i];
            else if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".strtab") == 0) // get strtab
                strtab = (Elf64_Shdr*) &shdr[i];
        }
    }

    printf("\n\n");
    Elf64_Sym *sym = (Elf64_Sym*) (ptr + symtab->sh_offset); // get symbols
    char* str = (char*) (ptr + strtab->sh_offset); // get str in strtab

    int len_array = 0, i = 0, j = 0;

    for (i = 0; i < symtab->sh_size / sizeof(Elf64_Sym); i++)
    {
        if (str + sym[i].st_name && ft_strlen(str + sym[i].st_name) && sym[i].st_info != 4)
            len_array++;
    }
    printf("here\n");
    Elf64_Sym array[len_array+1];

    ft_bzero(&array, sizeof(Elf64_Sym)*(len_array+1));

    printf("sh_size = %lu, sh_offset = %lu\n\n", symtab->sh_size/ sizeof(Elf64_Sym), symtab->sh_offset/ sizeof(Elf64_Sym));

    for (i = 0, j = 0; i < symtab->sh_size / sizeof(Elf64_Sym); i++) { // loop over symtab to get symbol name
        
        
        //? -u option ?
        // if (str + sym[i].st_name && ft_strlen(str + sym[i].st_name) && sym[i].st_info == 18 && sym[i].st_other == 0 && sym[i].st_value == 0)
        //? ?

        if (str + sym[i].st_name && ft_strlen(str + sym[i].st_name) && sym[i].st_info != 4)
            array[j++] = sym[i];
    }

    ft_sort_sym_table(array, len_array, str);

    for (i = 0; i < len_array; i++)
    {
        print_symbol(array[i], shdr, str);
    }

}


int main(int argc, char* argv[]) {
    int fd = open(argv[1], O_RDONLY);
    struct stat st;
    char *ptr;

    fstat(fd, &st);
    ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    if (ptr[EI_CLASS] == ELFCLASS32) {
        Elf32_Ehdr* elf_header = (Elf32_Ehdr*) ptr;
        printf("\nelf 32\n");
    } else if (ptr[EI_CLASS] == ELFCLASS64) {
        Elf64_Ehdr* elf_header = (Elf64_Ehdr*) ptr;
        process_64(ptr, elf_header);
        printf("\nelf 64\n");
    } else {
        printf("Invalid ELF class\n");
    }

    munmap(ptr, st.st_size);
    close(fd);
    return 0;
}


