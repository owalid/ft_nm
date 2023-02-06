# include "ft_nm.h"
# include "libft.h"

void    print_error(char *message, t_ft_nm_ctx *context)
{
    ft_putendl(message);
    munmap(context->ptr, context->st_size);
    // close(context->fd);
    exit(1);
}

void    debug_print_options(t_ft_nm_options *options)
{
    printf("=== options ===");
    printf("options->should_reverse = %d\n", options->should_reverse);
    printf("options->undefined_only = %d\n", options->undefined_only);
    printf("options->display_all = %d\n", options->display_all);
    printf("options->no_sort = %d\n", options->no_sort);
    printf("options->extern_only = %d\n\n", options->extern_only);
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



int    get_comp_sort_sym(char *low_current, char *low_next, ssize_t len_current, ssize_t len_next, t_ft_nm_options *options)
{
    short comp = 0;

    if (ft_strcmp(low_current, low_next) == 0)
        comp = len_current < len_next;
    else
        comp = ft_strcmp(low_current, low_next) > 0;

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