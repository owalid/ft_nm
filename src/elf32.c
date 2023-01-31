#include "ft_nm.h"
#include "libft.h"



void            print_type_32(Elf32_Sym sym, Elf32_Shdr *shdr)
{
    char  c;
    unsigned char st_bind = ELF64_ST_BIND(sym.st_info);
    char st_type = ELF64_ST_TYPE(sym.st_info);


    // SHT_PROGBITS == debug

    if (sym.st_shndx == SHN_ABS) // The symbol’s value is absolute, and will not be changed by further linking. 
        c = 'A';
    else if (sym.st_shndx == SHN_COMMON) // The symbol is common. Common symbols are uninitialized data.
        c = 'C';
    else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) // The symbol is in the BSS data section. This section typically contains zero-initialized or uninitialized data
        c = 'B';
    else if (st_bind == STB_WEAK) // The symbol is a weak symbol that has not been specifically tagged as a weak object symbol. When a weak defined symbol is linked with a normal defined symbol, the normal defined symbol is used with no error.
    {
        c = 'W';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'w';
    }

    else if (shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_MERGE) || shdr[sym.st_shndx].sh_flags == (SHF_ALLOC))
        c = 'R';
    else if (shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) // The symbol is in the initialized
        c = 'D';
    else if (sym.st_shndx == SHN_COMMON)
        c = 'C';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == SHF_EXECINSTR)
        c = 'p';
    else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS &&
        shdr[sym.st_shndx].sh_flags ==  (SHF_ALLOC | SHF_WRITE))
        c = 's';
    else if (st_bind == STB_WEAK && st_type == STT_OBJECT) // The symbol is a weak object. When a weak defined symbol is linked with a normal defined symbol, the normal defined symbol is used with no error.
    {
        c = 'V';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'v';
    }
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) // The symbol is in the text (code) section. 
        c = 'T';
    else if (sym.st_shndx == SHN_UNDEF)
        c = 'U';
    else if (st_bind == STB_GNU_UNIQUE)
        c = 'u';
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS)
    {
        c = 'N';
        if (shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
            c = 'n';
    }
    else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
        c = 'D';
    else if (sym.st_shndx == SHN_UNDEF)
        c = (st_bind == STB_WEAK) ? 'w' : 'U';
    else
        c = '?';

    // If lowercase, the symbol is usually local
    // If uppercase, the symbol is global (external)
    if (c != '?' && c != 'U' && c != 'I' && c != 'N' && c != 'W' && c != 'N'
        && ft_isupper(c) == 1 && st_bind == STB_LOCAL)
    {
        c = ft_tolower(c);
    }

    char final[4];
    ft_bzero(final, 4);
    ft_memset(final, 0, 4);
    ft_memset(final, ' ', 3);
    final[1] = c;
    ft_putstr(final);
}


void    print_symbol_32(Elf32_Sym sym, Elf32_Shdr *shdr, char *str)
{
    char current_sym_value[9];

    if (sym.st_name)
    {
        if (sym.st_value)
        {
            ft_bzero(current_sym_value, 9);
            get_formated_sym_value(sym.st_value, current_sym_value, 32);
            ft_putstr(current_sym_value);
        }
        else
        {
            char spaces[9];
            ft_bzero(spaces, 9);
            for (int i = 0; i < 8; i++)
                spaces[i] = ' ';
            ft_putstr(spaces);
        }
        print_type_32(sym, shdr);
        ft_putendl(str + sym.st_name);
    }
}


void		ft_sort_sym_array_32(Elf32_Sym *tab, int size, char *str)
{
	int i = 0, j = 0, k = 0, l = 0, comp = 0;
    ssize_t len_current = 0, len_next = 0;
    Elf32_Sym tmp;
    ft_bzero(&tmp, sizeof(Elf32_Sym));

	while (i + 1 < size)
	{
            len_current = ft_strlen((str + tab[i].st_name));
            len_next = ft_strlen((str + tab[i + 1].st_name));
            char low_current[len_current + 1];
            char low_next[len_next + 1];

            ft_bzero(low_current, len_current + 1);
            ft_bzero(low_next, len_next + 1);
            ft_memcpy(low_current, str + tab[i].st_name, len_current);
            ft_memcpy(low_next, str + tab[i + 1].st_name, len_next);

            ft_memcpy(low_current, ft_strlowcase(low_current), len_current);
            ft_memcpy(low_next, ft_strlowcase(low_next), len_next);

            while (j < len_current && !ft_isalnum(low_current[j]))
                j++;
            
            while (k < len_next && !ft_isalnum(low_next[k]))
                k++;

            if (ft_strcmp(low_current + j, low_next + k) == 0)
                comp = len_current < len_next;
            else
                comp = ft_strcmp(low_current + j, low_next + k) > 0;

            comp = (options->should_reverse) ? !comp : comp;

            if (comp)
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

void    process_32(char *ptr, Elf32_Ehdr *ehdr)
{
    int is_little_indian = (ptr[EI_DATA] != 1);
    unsigned int e_shoff = (is_little_indian) ? swap32(ehdr->e_shoff) : ehdr->e_shoff;
    Elf32_Shdr* shdr = (Elf32_Shdr*) ((char*) ptr + e_shoff); // get the section header
    Elf32_Shdr *symtab = NULL, *strtab = NULL; // declare symbol tab and str tab
    Elf32_Sym *sym; // symbols
    short have_symtab = 0;
    char *shstrtab;

    for (int i = 0; i < (context->st_size - e_shoff) / sizeof(Elf32_Shdr); i++)
        if (shdr[i].sh_type == SHT_SYMTAB) have_symtab = 1;

    if (!have_symtab)
        print_error(ERROR_NO_SYM);
    
    if (!(shstrtab = (char*)(ptr + shdr[ehdr->e_shstrndx].sh_offset))) // get the section header str tab
        exit(0);

    // printf("sh_strtab = %d", shdr->sh_size);

    // get the section header str tab
    // if (!ptr)
    //     print_error(ERROR_ELF_CLASS);

    for (size_t i = 0; i < ehdr->e_shnum; i++) // loop over header 
    {
        if (shdr[i].sh_size) {
            if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".symtab") == 0) // get symtab
                symtab = (Elf32_Shdr*) &shdr[i];
            else if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".strtab") == 0) // get strtab
                strtab = (Elf32_Shdr*) &shdr[i];
        }
    }
    if (!symtab || !strtab)
        print_error(ERROR_NO_SYM);
    if (!ptr || !symtab || !symtab->sh_offset || !(sym = (Elf32_Sym*) (ptr + symtab->sh_offset)))
        print_error(ERROR_ELF_CLASS);
    
    char* str = (char*) (ptr + strtab->sh_offset); // get str in strtab

    int len_array = 0, i = 0, j = 0;

    for (i = 0; i < symtab->sh_size / sizeof(Elf32_Sym); i++)
    {
        if (str + sym[i].st_name && ft_strlen(str + sym[i].st_name) && sym[i].st_info != 4)
            len_array++;
    }
    Elf32_Sym array[len_array+1];
    ft_bzero(&array, sizeof(Elf32_Sym)*(len_array+1));

    for (i = 0, j = 0; i < symtab->sh_size / sizeof(Elf32_Sym); i++) { // loop over symtab to get symbol name
        //? -u option ?
        // if (str + sym[i].st_name && ft_strlen(str + sym[i].st_name) && sym[i].st_info == 18 && sym[i].st_other == 0 && sym[i].st_value == 0)
        //? ?

        if (str + sym[i].st_name && ft_strlen(str + sym[i].st_name) && sym[i].st_info != 4)
            array[j++] = sym[i];
    }

    ft_sort_sym_array_32(array, len_array, str);

    for (i = 0; i < len_array; i++)
        print_symbol_32(array[i], shdr, str);
}
