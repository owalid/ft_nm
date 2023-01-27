# include "ft_nm.h"
# include "libft.h"

// TODO REDO THIS FUNCTION
void            print_type(Elf64_Sym sym, Elf64_Shdr *shdr)
{
    char  c;
    unsigned char st_bind = ELF64_ST_BIND(sym.st_info);
    char st_type = ELF64_ST_TYPE(sym.st_info);


    // If lowercase, the symbol is usually local
    // If uppercase, the symbol is global (external)

    if (st_bind == STB_GNU_UNIQUE)
        c = 'u';
    else if (st_bind == STB_WEAK) // The symbol is a weak symbol that has not been specifically tagged as a weak object symbol. When a weak defined symbol is linked with a normal defined symbol, the normal defined symbol is used with no error.
    {
        c = 'W';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'w';
    }
    else if (st_bind == STB_WEAK && st_type == STT_OBJECT) // The symbol is a weak object. When a weak defined symbol is linked with a normal defined symbol, the normal defined symbol is used with no error.
    {
        c = 'V';
        if (sym.st_shndx == SHN_UNDEF)
            c = 'v';
    }
    else if (sym.st_shndx == SHN_UNDEF) // The symbol is undefined.
        c = 'U';
    else if (sym.st_shndx == SHN_ABS) //  value is absolute, and will not be changed by further linking
        c = 'A';
    else if (sym.st_shndx == SHN_COMMON) // The symbol is common. Common symbols are uninitialized data.
        c = 'C';
    else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) // The symbol is in the BSS data section. This section typically contains zero-initialized or uninitialized data
    {
        c = 'B';
        if (st_bind == STB_LOCAL)
            c = 'b';
    }
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
    {
        c = 'R';
        if (st_bind == STB_LOCAL)
            c = 'r';
    }
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) // The symbol is in the initialized
    {
        c = 'D';
        if (st_bind == STB_LOCAL)
            c = 'd';
    }
    else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
        && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) // The symbol is in the text (code) section. 
    {
        c = 'T';
        if (st_bind == STB_LOCAL)
            c = 't';
    }
    // else if () // The symbol is in an initialized data section for small objects.
    //     c = 'G';
    // else if () // The symbol is a unique global symbol. This is a GNU extension to the standard set of ELF symbol bindings. 
    //     c = 'u';
    // else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
    //     c = 'D';
    else
        c = 'd';

    printf("%c ", c);
}


void    print_symbol(Elf64_Sym sym, Elf64_Shdr *shdr, char *str)
{
    char current_sym_value[17];

    if (sym.st_name)
    {
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


void		ft_sort_sym_array(Elf64_Sym *tab, int size, char *str)
{
	int i = 0, j = 0, k = 0, l = 0, comp = 0;
    ssize_t len_current = 0, len_next = 0;
    Elf64_Sym tmp;
    ft_bzero(&tmp, sizeof(Elf64_Sym));

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

            while(l < len_current && l < len_next && low_current[l + j] == low_next[k + l])
                l++;

            if (low_current[j+l] - low_next[k+l] == 0)
                comp = len_current < len_next;
            else if (options->should_reverse)
                comp = low_current[j+l] - low_next[k+l] < 0;
            else
                comp = low_current[j+l] - low_next[k+l] > 0;

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
