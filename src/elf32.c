#include "ft_nm.h"
#include "libft.h"



void        get_type_32(Elf32_Sym sym, Elf32_Shdr *shdr, char* type)
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

    ft_bzero(type, 4);
    ft_memset(type, 0, 4);
    ft_memset(type, ' ', 3);
    type[1] = c;
}


void    print_symbol_32(Elf32_Sym sym, Elf32_Shdr *shdr, char *str)
{
    char current_sym_value[9];
    char type[4];

    if (sym.st_name)
    {
        get_type_32(sym, shdr, type);

        if ((sym.st_value && type[1] != 'U' && type[1] != 'w') || type[1] == 'a' || type[1] == 'T' || type[1] == 't')
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
        ft_putstr(type);
        ft_putendl(str + sym.st_name);
    }
}

void        ft_insert_sort_sym_array_32(Elf32_Sym *tab, int size, char *str, t_ft_nm_options *options)
{
    // https://en.wikipedia.org/wiki/Insertion_sort
    ssize_t i = 0, j = 0, k = 0;
    size_t max_len = 0, len_current = 0, len_before = 0;
    char *tab_lower[size];
    Elf32_Sym tmp;
    ft_bzero(&tmp, sizeof(Elf32_Sym));

    // get max size of string
    for (; i < size; i++)
    {
        len_current = ft_strlen((str + tab[i].st_name));
        if (max_len < len_current)
            max_len = len_current;
    }
    
    char tmp_str[max_len];
    ft_bzero(tmp_str, max_len);
    i = 0;
    len_current = 0;

    // make copy with lower string and alnum
    for (; i < size; i++)
    {
        j = 0;
        k = 0;
        len_current = ft_strlen((str + tab[i].st_name));
        tab_lower[i] = ft_strnew(max_len);

        for (; k < len_current; k++)
        {
            if (ft_isalnum((str + tab[i].st_name)[k]))
                tab_lower[i][j++] = (str + tab[i].st_name)[k];
        }
        
        ft_memcpy(tab_lower[i], ft_strlowcase(tab_lower[i]), j);
    }
    

    i = 1;
	while (i < size)
	{
        j = i;
        
        len_current = ft_strlen(str + tab[j].st_name);
        len_before = ft_strlen(str + tab[j - 1].st_name);
        
        while (j > 0 && get_comp_sort_sym(tab_lower[j - 1], tab_lower[j], str + tab[j - 1].st_name, str + tab[j].st_name, tab[j - 1].st_value, tab[j].st_value, options))
        {
           
            tmp = tab[j];
            tab[j] = tab[j - 1];
            tab[j - 1] = tmp;

            len_current = ft_strlen(tab_lower[j]);
            len_before = ft_strlen(tab_lower[j - 1]);
            
            ft_memcpy(tmp_str, tab_lower[j], len_current);
            ft_memcpy(tab_lower[j], tab_lower[j - 1], len_before);
            ft_bzero(tab_lower[j] + len_before, max_len - len_before);

            ft_memcpy(tab_lower[j - 1], tmp_str, len_current);
            ft_bzero(tab_lower[j - 1] + len_current, max_len - len_current);
            j--;
        }
        i++;
	}

    for (; i < size; i++)
        free(tab_lower[i]);
}


int     filter_comp_sym_32(Elf32_Shdr* shdr, Elf32_Sym sym, char *str, unsigned long max_len, t_ft_nm_options *options)
{
    short comp = 0;

    if (str + sym.st_name && ft_strlen(str + sym.st_name))
    {
        if (options->undefined_only)
            comp = ((sym.st_info == SHT_SYMTAB_SHNDX || ELF32_ST_BIND(sym.st_info) == STB_WEAK) && (sym.st_other == 0 && sym.st_value == 0));
        else if (options->extern_only && sym.st_shndx < max_len)
            comp = ((shdr[sym.st_shndx].sh_type == SHT_NOBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE) && ELF32_ST_BIND(sym.st_info) != STB_LOCAL ) // B
                            ||  (ELF32_ST_BIND(sym.st_info) == STB_WEAK) // w, W
                            || ((shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_MERGE) || shdr[sym.st_shndx].sh_flags == (SHF_ALLOC)) && ELF32_ST_BIND(sym.st_info) != STB_LOCAL) // R
                            || (shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE) && ELF32_ST_BIND(sym.st_info) != STB_LOCAL) // D
                            || ((sym.st_shndx == SHN_UNDEF) && !(ELF32_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)) // U
                            || (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR) && ELF32_ST_BIND(sym.st_info) != STB_LOCAL)); // T 
        else if (options->display_all)
            comp = 1;
        else
            comp = (sym.st_info != 4);  
    }
    
    return comp;
}

void    process_32(char *ptr, Elf32_Ehdr *ehdr, t_ft_nm_options *options, t_ft_nm_ctx *context)
{
    if (ptr[EI_DATA] != 1 && ptr[EI_DATA] != 2)
    {
        print_error(ERROR_BAD_ENDIAN, context);
        return;
    }

    short is_little_indian = (ptr[EI_DATA] != 1);
    unsigned int e_shoff = (is_little_indian) ? swap32(ehdr->e_shoff) : ehdr->e_shoff;
    
    if (e_shoff > context->st_size)
    {
        print_error(ERROR_E_SHOFF_TO_BIG, context);
        return;
    }
    if (e_shoff <= 0)
    {
        print_error(ERROR_E_SHOFF_TO_LOW, context);
        return;
    }
    if (ehdr->e_shnum <= 0)
    {
        print_error(ERROR_E_SNUM_TO_LOW, context);
        return;
    }


    if (ehdr->e_shstrndx > context->st_size)
    {
        print_error(ERROR_E_SHSTR_TO_BIG, context);
        return;
    }

    Elf32_Shdr* shdr = (Elf32_Shdr*) ((char*) ptr + e_shoff); // get the section header
    Elf32_Shdr *symtab = NULL, *strtab = NULL; // declare symbol tab and str tab
    Elf32_Sym *sym; // symbols
    short have_symtab = 0;
    char *shstrtab;
    unsigned long max_len =  (context->st_size - e_shoff) / sizeof(Elf32_Shdr);
    size_t i = 0;

    for (i = 0; i < max_len; i++)
    {
        if (shdr[i].sh_type == SHT_SYMTAB)
        {
            have_symtab = 1;
            break;
        }
    }

    if (!have_symtab)
    {
        print_error(ERROR_NO_SYM, context);
        return;
    }

    
    if (!(shstrtab = (char*)(ptr + shdr[ehdr->e_shstrndx].sh_offset))) // get the section header str tab
    {
        if (context->should_exit)
            exit(0);
        else
            return;
    }

    for (i = 0; i < ehdr->e_shnum; i++) // loop over header 
    {
        if (shdr[i].sh_name > context->st_size)
        {
            print_error(ERRORS_OFFSET, context);
            return;
        }
        if (shdr[i].sh_size) {
            if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".symtab") == 0) // get symtab
                symtab = (Elf32_Shdr*) &shdr[i];
            else if (ft_strcmp(&shstrtab[shdr[i].sh_name], ".strtab") == 0) // get strtab
                strtab = (Elf32_Shdr*) &shdr[i];
            if (symtab && strtab)
                break;
        }
    }
    if (!symtab || !strtab)
    {
        print_error(ERROR_NO_SYM, context);
        return;
    }
    if (!ptr || !symtab || !symtab->sh_offset || !(sym = (Elf32_Sym*) (ptr + symtab->sh_offset)))
    {
        print_error(ERROR_ELF_CLASS, context);
        return;
    }
    
    char* str = (char*) (ptr + strtab->sh_offset); // get str in strtab


    int len_array = 0, j = 0;
    for (i = 0; i < symtab->sh_size / sizeof(Elf32_Sym); i++)
    {
        if (filter_comp_sym_32(shdr, sym[i], str, max_len, options))
            len_array++;
    }

    Elf32_Sym array[len_array+1];
    ft_bzero(&array, sizeof(Elf32_Sym)*(len_array+1));

    for (i = 0, j = 0; i < symtab->sh_size / sizeof(Elf32_Sym); i++) { // loop over symtab to get symbol name
        if (filter_comp_sym_32(shdr, sym[i], str, max_len, options))
            array[j++] = sym[i];
    }


    if (!options->no_sort)
        ft_insert_sort_sym_array_32(array, len_array, str, options);

    for (i = 0; i < len_array; i++)
        print_symbol_32(array[i], shdr, str);

}
