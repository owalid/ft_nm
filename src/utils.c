# include "ft_nm.h"
# include "libft.h"

void    get_formated_sym_value(unsigned int st_value, char *str, int size)
{
    size = (size == 64) ? 16 : 8;
    char *tmp_str = ft_utoa_base(st_value, size);
    int len_tmp_str = ft_strlen(tmp_str);
    int diff = size - len_tmp_str;

    ft_memset(str, '0', diff);
    ft_memcpy(str+diff, ft_strlowcase(tmp_str), len_tmp_str);
    free(tmp_str);
}

