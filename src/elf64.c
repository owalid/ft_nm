#include "ft_nm.h"
#include "libft.h"

void            get_type_64(Elf64_Sym sym, Elf64_Shdr *shdr, char *type)
{
    char  c;
    unsigned char st_bind = ELF64_ST_BIND(sym.st_info);
    char st_type = ELF64_ST_TYPE(sym.st_info);

    // SHT_PROGBITS == debug
    if (sym.st_shndx == SHN_ABS) // The symbol’s value is absolute, and will not be changed by further linking. 
        c = 'A';
    else if (sym.st_shndx == SHN_COMMON) // The symbol is common. Common symbols are uninitialized data.
        c = 'C';
    else if (st_bind == STB_WEAK && st_type == STT_OBJECT) // The symbol is a weak object. When a weak defined symbol is linked with a normal defined symbol, the normal defined symbol is used with no error.
    {
        c = 'V';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'v';
    }
    else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) // The symbol is in the BSS data section. This section typically contains zero-initialized or uninitialized data
        c = 'B';
    else if (st_bind == STB_WEAK) // The symbol is a weak symbol that has not been specifically tagged as a weak object symbol. When a weak defined symbol is linked with a normal defined symbol, the normal defined symbol is used with no error.
    {
        c = 'W';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'w';
    }
    else if (sym.st_shndx == SHN_UNDEF)
        c = 'U';
    else if (st_bind == STB_GNU_UNIQUE)
        c = 'u';
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
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) // The symbol is in the text (code) section. 
        c = 'T';
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

    ft_bzero(type, 4);
    ft_memset(type, 0, 4);
    ft_memset(type, ' ', 3);
    type[1] = c;
}


void    print_symbol_64(Elf64_Sym sym, Elf64_Shdr *shdr, char *str)
{
    // print symbol like:
    // <st_value> OR <' ' * 16> <symbol> <st_name>

    char current_sym_value[17];
    char type[4];

    if (sym.st_name)
    {
        get_type_64(sym, shdr, type);

        if ((sym.st_value && type[1] != 'U' && type[1] != 'w') || type[1] == 'a' || ft_toupper(type[1]) == 'T' || ft_toupper(type[1]) == 'D' || type[1] == 'B')
        {
            ft_bzero(current_sym_value, 17);
            get_formated_sym_value(sym.st_value, current_sym_value, 64); 
            ft_putstr(current_sym_value);
        }
        else
        {
            char spaces[17];
            ft_bzero(spaces, 17);
            for (int i = 0; i < 16; i++)
                spaces[i] = ' ';
            ft_putstr(spaces);
        }
        ft_putstr(type);
        ft_putendl(str + sym.st_name);
    }
}

void        ft_insert_sort_sym_array_64(Elf64_Sym *tab, size_t size, char *str, t_ft_nm_options *options, t_ft_nm_ctx *context)
{
    // https://en.wikipedia.org/wiki/Insertion_sort
    if (!size)
        return;
    size_t i = 0, j = 0, k = 0, len_shdrs = 0, len_current = 0, len_next = 0;
    char *tab_lower[size];
    Elf64_Sym tmp;

    ft_bzero(&tmp, sizeof(Elf64_Sym));

    // get max size of string
    for (; i < size; i++)
    {
        len_current = ft_strlen((str + tab[i].st_name));
        if (len_shdrs < len_current)
            len_shdrs = len_current;
    }
    
    char tmp_str[len_shdrs];
    ft_bzero(tmp_str, len_shdrs);
    i = 0;

    // make copy with lower string and alnum
    for (; i < size; i++)
    {
        j = 0;
        k = 0;
        len_current = ft_strlen((str + tab[i].st_name));

        if ((tab_lower[i] = ft_strnew(len_shdrs)) == NULL)
            print_error(ERROR_MALLOC, context);

        for (; k < len_current; k++)
        {
            if (ft_isalnum((str + tab[i].st_name)[k]) || (str + tab[i].st_name)[k] == '$')
                tab_lower[i][j++] = (str + tab[i].st_name)[k];
        }
        
        ft_memcpy(tab_lower[i], ft_strlowcase(tab_lower[i]), j);
    }

    i = 1;
	while (i < size)
	{
        j = i;

        // compare string value according options
        while (j > 0 && get_comp_sort_sym(tab_lower[j - 1], tab_lower[j], str + tab[j - 1].st_name, str + tab[j].st_name, tab[j - 1].st_value, tab[j].st_value, options))
        {
           
            tmp = tab[j];
            tab[j] = tab[j - 1];
            tab[j - 1] = tmp;

            len_current = ft_strlen(tab_lower[j]);
            len_next = ft_strlen(tab_lower[j - 1]);
            
            ft_memcpy(tmp_str, tab_lower[j], len_current);
            ft_memcpy(tab_lower[j], tab_lower[j - 1], len_next);
            ft_bzero(tab_lower[j] + len_next, len_shdrs - len_next);

            ft_memcpy(tab_lower[j - 1], tmp_str, len_current);
            ft_bzero(tab_lower[j - 1] + len_current, len_shdrs - len_current);
            j--;
        }
        i++;
	}

    i = 0;
    for (; i < size; i++)
        free(tab_lower[i]);
    
}

int     filter_comp_sym(Elf64_Shdr* shdr, Elf64_Sym sym, char *str, unsigned long len_shdrs, t_ft_nm_options *options)
{
    short comp = 0;

    if (str + sym.st_name && ft_strlen(str + sym.st_name))
    {
        if (options->undefined_only) // -u
            comp = (((ELF64_ST_BIND(sym.st_info) == STB_WEAK) || (sym.st_shndx == SHN_UNDEF) || (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)) && (sym.st_other == 0 && sym.st_value == 0));
        else if (options->extern_only && sym.st_shndx < len_shdrs)
            comp = ((shdr[sym.st_shndx].sh_type == SHT_NOBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE) && ELF64_ST_BIND(sym.st_info) != STB_LOCAL ) // B
                            ||  (ELF64_ST_BIND(sym.st_info) == STB_WEAK) // w, W
                            || ((shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_MERGE) || shdr[sym.st_shndx].sh_flags == (SHF_ALLOC)) && ELF64_ST_BIND(sym.st_info) != STB_LOCAL) // R
                            || (shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE) && ELF64_ST_BIND(sym.st_info) != STB_LOCAL) // D
                            || ((sym.st_shndx == SHN_UNDEF) && !(ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)) // U
                            || (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR) && ELF64_ST_BIND(sym.st_info) != STB_LOCAL)); // T
        else // no options
            comp = (sym.st_info != 4);  
    }
    
    return comp;
}

void    process_64(char *ptr, Elf64_Ehdr *ehdr, t_ft_nm_options *options, t_ft_nm_ctx *context)
{
    if (ptr[EI_DATA] != 1 && ptr[EI_DATA] != 2) // if endian is not valid, (valid values: 1 or 2)
    {
        print_error(ERROR_BAD_ENDIAN, context);
        return;
    }

    
    short is_little_indian = (ptr[EI_DATA] != 1), have_symtab = 0;
    unsigned long e_shoff = (is_little_indian) ? swap64(ehdr->e_shoff) : ehdr->e_shoff;
    Elf64_Shdr* shdr = (Elf64_Shdr*) ((char*) ptr + e_shoff); // get the section header

    // init symbol tab and str tab
    Elf64_Shdr *symtab = NULL, *strtab = NULL;
    Elf64_Sym *sym = NULL; // init symbols
    unsigned long len_shdrs = (context->st_size - e_shoff) / sizeof(Elf64_Shdr);

    if ((e_shoff+ehdr->e_shnum) > (unsigned long)context->st_size) // if e_shoff is to big
    {
        print_error(ERROR_E_SHOFF_TO_BIG, context);
        return;
    }
    if (e_shoff <= 0) // if e_shoff is to low
    {
        print_error(ERROR_E_SHOFF_TO_LOW, context);
        return;
    }
    if (ehdr->e_shnum <= 0) // if e_shnum is to low
    {
        print_error(ERROR_E_SNUM_TO_LOW, context);
        return;
    }

    if (ehdr->e_shstrndx > context->st_size) // if e_shstrndx is to big
    {
        print_error(ERROR_E_SHSTR_TO_BIG, context);
        return;
    }


    for (ssize_t i = 0; i < (ssize_t)len_shdrs; i++)
        if (shdr[i].sh_type == SHT_SYMTAB)
        {
            have_symtab = 1;
            break;
        }

    if (!have_symtab)
    {
        print_error(ERROR_NO_SYM, context);
        return;
    }

    char *shstrtab = (char*)(ptr + shdr[ehdr->e_shstrndx].sh_offset); // get the section header str tab
    for (size_t i = 0; i < ehdr->e_shnum; i++) // loop over header 
    {
        if (shdr[i].sh_name > context->st_size)
        {
            print_error(ERRORS_OFFSET, context);
            return;
        }
        if (shdr[i].sh_size) { // get symtab and strtab
            if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".symtab") == 0) // get symtab
                symtab = (Elf64_Shdr*) &shdr[i];
            else if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".strtab") == 0) // get strtab
                strtab = (Elf64_Shdr*) &shdr[i];
        }
    }

    if (!symtab || !strtab)
    {
        print_error(ERROR_NO_SYM, context);
        return;
    }

    if (!ptr || !symtab || !symtab->sh_offset || !(sym = (Elf64_Sym*) (ptr + symtab->sh_offset)))
    {
        print_error(ERROR_ELF_CLASS, context);
        return;
    }

    char* str = (char*) (ptr + strtab->sh_offset); // get str in strtab
    size_t len_array = 0, i = 0, j = 0;

    for (i = 0; i < symtab->sh_size / sizeof(Elf64_Sym); i++) // init size of final array
    {
        if (filter_comp_sym(shdr, sym[i], str, len_shdrs, options))
            len_array++;
    }

    Elf64_Sym array[len_array+1];
    ft_bzero(&array, sizeof(Elf64_Sym)*(len_array+1));

    for (i = 0, j = 0; i < symtab->sh_size / sizeof(Elf64_Sym); i++) { // loop over symtab to get symbol name
        if (filter_comp_sym(shdr, sym[i], str, len_shdrs, options))
            array[j++] = sym[i];
    }

    if (!options->no_sort) // -p
        ft_insert_sort_sym_array_64(array, len_array, str, options, context);

    for (i = 0; i < len_array; i++) // print all symbols
        print_symbol_64(array[i], shdr, str);
}
