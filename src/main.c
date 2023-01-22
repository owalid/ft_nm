#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <stdio.h>

void    process_64(char *ptr, Elf64_Ehdr *elf_header)
{
    Elf64_Shdr* section_ht = (Elf64_Shdr*) ((char*) elf_header + elf_header->e_ehsize);

    printf("elf_header =\n");
    printf("elf_header->e_ident =        %s\n", elf_header->e_ident);
	printf("elf_header->e_type =         %d\n", elf_header->e_type);
	printf("elf_header->e_machine =      %d\n", elf_header->e_machine);
	printf("elf_header->e_version =      %d\n", elf_header->e_version);
    printf("elf_header->e_entry =        %lx\n", elf_header->e_entry);
	printf("elf_header->e_phoff =        %lx\n", elf_header->e_phoff);
	printf("elf_header->e_shoff =        %lx\n", elf_header->e_shoff);
    printf("elf_header->e_flags =        %s\n", elf_header->e_flags);
    printf("elf_header->e_ehsize =       %d\n", elf_header->e_ehsize);
    printf("elf_header->e_phentsize =    %d\n", elf_header->e_phentsize);
	printf("elf_header->e_phnum =        %x\n", elf_header->e_phnum);
	printf("elf_header->e_shentsize =    %x\n", elf_header->e_shentsize);
	printf("elf_header->e_shnum =        %x\n", elf_header->e_shnum);
	printf("elf_header->e_shstrndx =     %x\n", elf_header->e_shstrndx);
    printf("elf_header->e_ehsize =       %d\n", elf_header->e_ehsize);
	printf("elf_header->e_phnum =        %d\n", elf_header->e_phnum);


    printf("\n\n\nsection_ht =\n");
    printf("section_ht->sh_name =           %d\n", section_ht->sh_name);
    printf("section_ht->sh_type =           %d\n", section_ht->sh_type);
    printf("section_ht->sh_flags =          %d\n", section_ht->sh_flags);
    printf("section_ht->sh_addr =           %d\n", section_ht->sh_addr);
    printf("section_ht->sh_size =           %d\n", section_ht->sh_size);
    printf("section_ht->sh_link =           %d\n", section_ht->sh_link);
    printf("section_ht->sh_info =           %d\n", section_ht->sh_info);
    printf("section_ht->sh_addralign =      %d\n", section_ht->sh_addralign);
    printf("section_ht->sh_entsize =        %d\n", section_ht->sh_entsize);
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


