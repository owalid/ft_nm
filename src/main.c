# include "ft_nm.h"
# include "libft.h"


int main(int argc, char *argv[]) {
    int fd;
    char *ptr;
    struct stat buf;

    if (argc != 2) {
        ft_putstr("usage: ./ft_nm [file]");
        return (0);
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        ft_putstr("error: open");
        return (0);
    }

    if (fstat(fd, &buf) < 0) {
        ft_putstr("error: fstat");
        return (0);
    }

    if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
        ft_putstr("error: mmap");
        return (0);
    }
}
    