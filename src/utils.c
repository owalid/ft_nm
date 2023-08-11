# include "ft_nm.h"
# include "libft.h"

void    print_error(char *message, t_ft_nm_ctx *context)
{
    // print error with path of file, free and exit
    short have_filename = (context->filename != NULL);
    ft_putstr_fd("ft_nm: ", 2);
    if (have_filename)
        ft_putstr_fd(context->filename, 2);
    else
        ft_putstr_fd(MESSAGE_UNKNOW_FILENAME, 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(message, 2);
    if (!context->current_ar || context->should_exit)
        munmap(context->ptr, context->st_size);
    if (have_filename)
    {
        free(context->filename);
        context->filename = NULL;
    }
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
    tmp_str = NULL;
}

int    get_comp_sort_sym(char *low_before, char *low_current, char *raw_before, char *raw_current, unsigned int st_value_before, unsigned int st_value_current, t_ft_nm_options *options)
{
    short comp = 0;
    int str_raw_cmp = 0;
    size_t i = 0;

    if (ft_strcmp(low_before, low_current) == 0)
    {
        str_raw_cmp = ft_strcmp(raw_before, raw_current);
        i = 0;
        while (raw_before[i] == raw_current[i] && raw_before[i] && raw_current[i])
            i++;
        if (str_raw_cmp == 0)
            comp = st_value_before > st_value_current;
        else {
            if (raw_current[i] == '(' && raw_current[i+1] == '*')
                comp = 0;
            else if (raw_before[i] == ft_toupper(raw_current[i]) || ft_toupper(raw_before[i]) == raw_current[i])
                comp = raw_before[i] == ft_toupper(raw_current[i]);
            else
                comp = str_raw_cmp > 0;
        }
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