#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <stdio.h>

void    process_64(char *ptr, Elf64_Ehdr *elf_header)
{
    Elf64_Shdr* section_header_table = (Elf64_Shdr*) ((char*) elf_header + elf_header->e_ehsize);

    printf("section_header_table.sh_size = %d", elf_header->e_shnum);
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


