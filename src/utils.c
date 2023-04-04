# include "ft_nm.h"
# include "libft.h"

void    print_error(char *message, t_ft_nm_ctx *context)
{
    ft_putstr_fd("ft_nm: ", 2);
    ft_putstr_fd(context->filename, 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(message, 2);
    munmap(context->ptr, context->st_size);
    if (context->filename != NULL)
        free(context->filename);
    if (context->should_exit)
        exit(1);
}

void    get_formated_sym_value(unsigned int st_value, char *str, int size)
{
    size = (size == 64) ? 16 : 8;
    char *tmp_str = ft_utoa_base(st_value, 16);
    int len_tmp_str = ft_strlen(tmp_str);
    int diff = size - len_tmp_str;

    ft_memset(str, '0', diff);
    ft_memcpy(str+diff, ft_strlowcase(tmp_str), len_tmp_str);
    free(tmp_str);
}

int    get_comp_sort_sym(char *low_before, char *low_current, char *raw_before, char *raw_current, unsigned int st_value_before, unsigned int st_value_current, t_ft_nm_options *options)
{
    short comp = 0;
    int str_raw_cmp = 0;

    if (ft_strcmp(low_before, low_current) == 0)
    {
        str_raw_cmp = ft_strcmp(raw_before, raw_current);
        if (str_raw_cmp == 0)
            return st_value_before > st_value_current;
        else
            comp = str_raw_cmp > 0;
    }
    else
        comp = ft_strcmp(low_before, low_current) > 0;

    return (options->should_reverse) ? !comp : comp;
}

unsigned int swap32(unsigned int num) {
    return ((num>>24)&0xff) | // move byte 3 to byte 0
        ((num<<8)&0xff0000) | // move byte 1 to byte 2
        ((num>>8)&0xff00) | // move byte 2 to byte 1
        ((num<<24)&0xff000000); // byte 0 to byte 3
}

size_t swap64(size_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}