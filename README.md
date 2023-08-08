
[![wakatime](https://wakatime.com/badge/user/57a746b4-0744-4dc9-a0f3-61d9ea529bde/project/e68bc64c-210d-4e0a-bc14-27378f81d981.svg)](https://wakatime.com/badge/user/57a746b4-0744-4dc9-a0f3-61d9ea529bde/project/e68bc64c-210d-4e0a-bc14-27378f81d981)

## Sources
- [https://wiki.osdev.org/ELF](https://wiki.osdev.org/ELF)
- [https://www.sco.com/developers/gabi/latest/contents.html](https://www.sco.com/developers/gabi/latest/contents.html)
- [https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)


<img src="https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#/media/File:ELF_Executable_and_Linkable_Format_diagram_by_Ange_Albertini.png">

## Elf header

L'en-tête ELF commence par un magic. Ce magic de l'en-tête ELF fournit des informations sur le fichier. Les 4 premières parties hexadécimales définissent qu'il s'agit d'un fichier ELF (45=E,4c=L,46=F), préfixé par la valeur 7f.

### Class

32 bits ou 64 bits

### Data

Il comporte deux options: 01 pour le LSB (Least Significant Bit), également connu sous le nom de little-endian. Ensuite, il y a la valeur 02, pour MSB (Most Significant Bit, big-endian).

### **OS/ABI**

Les fonctions communes de chaque système d'exploitation se chevauchent largement.

En outre, chacun d'eux possède des fonctions spécifiques, ou du moins des différences mineures entre eux. La définition du bon ensemble se fait avec une Interface Binaire d'Application.

### Type

Le champ "type" nous indique l'objectif du fichier. Il existe quelques types de fichiers courants.

- CORE (value 4)
- DYN (Shared object file), for libraries (value 3)
- EXEC (Executable file), for binaries (value 2)
- REL (Relocatable file), before linked into an executable file (value 1)

## File data

Outre l'en-tête ELF, les fichiers ELF se composent de trois parties.

1. Program Headers or Segments (9)
2. Section Headers or Sections (28)
3. Data

### **Program headers**

Un fichier ELF se compose de zéro ou plusieurs segments, et décrit comment créer une image de processus/mémoire pour l'exécution.

Lorsque le noyau voit ces segments, il les utilise pour les mapper dans l'espace d'adressage virtuel, en utilisant l'appel système mmap(2). En d'autres termes, il convertit des instructions prédéfinies en une image mémoire.

Si votre fichier ELF est un binaire normal, il a besoin de ces en-têtes de programme. Sinon, il ne s'exécutera tout simplement pas. Il utilise ces en-têtes, avec la structure de données sous-jacente, pour former un processus.

Ce processus est similaire pour les bibliothèques partagées.


### **ELF sections**

#### **Section headers**

Les en-têtes de section définissent toutes les sections du fichier. Comme nous l'avons dit, cette "vue" est utilisée pour la liaison et la relocalisation.

Pour les fichiers exécutables, il y a quatre sections principales : .text, .data, .rodata, et .bss. Chacune de ces sections est chargée avec des droits d'accès différents, qui peuvent être vus avec readelf -S.

- .text: Contient du code exécutable.
- .data: Données initialisées, avec droits d'accès en lecture/écriture
- .rodata: Données initialisées, avec des droits d'accès en lecture uniquement (=A).
- .bss: Données non initialisées, avec droits d'accès en lecture/écriture (=WA)


#### **Section groups**

Certaines sections peuvent être regroupées, car elles forment un tout, ou en d'autres termes être une dépendance.

Les linkers les plus récents supportent cette fonctionnalité. Pourtant, il n'est pas courant de trouver cela si souvent.


#### Binaires statiques et dynamiques

Lorsqu'il s'agit de binaires ELF, il est bon de savoir qu'il existe deux types et comment ils sont liés. Le type est soit statique soit dynamique et fait référence aux bibliothèques utilisées.

À des fins d'optimisation, nous voyons souvent que les binaires sont "dynamiques", ce qui signifie qu'ils ont besoin de composants externes pour fonctionner correctement.

Souvent, ces composants externes sont des bibliothèques normales, qui contiennent des fonctions courantes, comme l'ouverture de fichiers ou la création d'un socket réseau.

Les binaires statiques, par contre, ont toutes les bibliothèques incluses.


## Symbols

- `A` (absolute) : indicates that the symbol has an absolute value that is not bound to any section.
- `B` (uninitialized data) : indicates that the symbol is defined in a section that contains uninitialized data.
- `C` (common) : indicates that the symbol is uninitialized data that is not bound to any section.
- `D` (initialized data) : indicates that the symbol is defined in a section that contains initialized data.
- `R` (read only data) : indicates that the symbol is defined in a section that contains read-only data.
- `T` (code) : indicates that the symbol is defined in a section that contains code.
- `U` (undefined) : indicates that the symbol is not defined in the file.
- `V` (weak) : indicates that the symbol is a weak symbol.
- `W` (warning) : indicates that the symbol is a warning symbol.


### Elf.h

Small representation of elf header

```
ELF Header
    |
    |- File information (Elf64_Ehdr)
        |- e_ident (Identification bytes)
        |- e_type (Type of file)
        |- e_machine (Architecture)
        |- e_version (ELF version)
        |- e_entry (Entry point virtual address)
        |- e_phoff (Program header table file offset)
        |- e_shoff (Section header table file offset)
        |- e_flags (Processor-specific flags)
        |- e_ehsize (ELF header size in bytes)
        |- e_phentsize (Program header table entry size)
        |- e_phnum (Program header table entry count)
        |- e_shentsize (Section header table entry size)
        |- e_shnum (Section header table entry count)
        |- e_shstrndx (Section header string table index)
    |
    |- Program header table (Elf64_Phdr)
        |- p_type (Type of segment)
        |- p_flags (Segment flags)
        |- p_offset (File offset of segment)
        |- p_vaddr (Virtual address of segment)
        |- p_paddr (Physical address of segment)
        |- p_filesz (Size of segment in file)
        |- p_memsz (Size of segment in memory)
        |- p_align (Alignment of segment)
    |
    |- Section header table (Elf64_Shdr)
        |- sh_name (Section name)
        |- sh_type (Type of section)
        |- sh_flags (Section flags)
        |- sh_addr (Section virtual address)
        |- sh_offset (Section file offset)
        |- sh_size (Section size)
        |- sh_link (Link to other section)
        |- sh_info (Extra information)
        |- sh_addralign (Section alignment)
        |- sh_entsize (Entry size if section holds a table)
    |
    |- Symbol table (Elf64_Sym)
        |- st_name (Symbol name)
        |- st_info (Symbol type and binding)
        |- st_other (Reserved)
        |- st_shndx (Section index of symbol)
        |- st_value (Symbol value)
        |- st_size (Symbol size)
    |
    |- Relocation table (Elf64_Rel and Elf64_Rela)
        |- r_offset (Relocation offset)
        |- r_info (Relocation type and symbol index)
        |- (Elf64_Rela also includes: r_addend (Addend))
```

**e_shnum**

Ce champ contient le nombre d'entrées de la table des entêtes de sections.

Ainsi, la taille en octets de la table des entêtes de sections pourra être obtenue en multipliant `e_shentsize` par `e_shnum`.

S'il n'y a pas de table des entêtes de sections, le champ e_shnum contiendra zéro.

**e_shoff**

Ce champ contient le décalage en octets de la table des entêtes de sections. Si ce fichier ne contient pas de table des entêtes des sections, ce champ contient zéro.



### Ar file



Le contenu d'un fichier de table de symboles d'archive est le suivant, où la taille des mots est de 4 octets pour une table de symboles de 32 bits et de 8 bytes pour une table de symboles de 64 bits.


1. Le nombre de symboles. Longueur: wordsize bytes.
2. Le tableau des décalages dans le fichier archive. Longueur: wordsize bytes * «le nombre de symboles».
3. La table de chaîne de symboles. Longueur: ar_size - wordsize bytes * («le nombre de symboles» + 1).


La table de symboles 32 bits suivante, par exemple, définit 4 symboles. L'archive membre à l'offset de fichier 114 définit le `name`.

L'archive membre à l'offset de fichier 122 définit l'objet.

L'archive membre à l'offset de fichier 426 définit la fonction et l'archive membre à l'offset de fichier 434 définit le `name2`.


```
Offset     +0   +1   +2   +3
          ___________________
 0       |         4         | 4 offset entries
         |___________________|
 4       |       114         | name
         |___________________|
 8       |       122         | object
         |___________________|
12       |       426         | function
         |___________________|
16       |       434         | name2
         |___________________|
20       |  n | a  | m  | e  |
         |____|____|____|____|
24       | \0 | o  | b  | j  |
         |____|____|____|____|
28       |  e | c  | t  | \0 |
         |____|____|____|____|
32       |  f | u  | n  | c  |
         |____|____|____|____|
36       |  t | i  | o  | n  |
         |____|____|____|____|
40       | \0 | n  | a  | m  |
         |____|____|____|____|
44       |  e | 2  | \0 |    |
         |____|____|____|____|
```

La même exemple, en utilisant une table de symboles 64 bits serait rendu comme suit. Le membre de l'archive à l'offset de fichier 134 définit le name. Le membre de l'archive à l'offset de fichier 142 définit l'objet. Le membre de l'archive à l'offset de fichier 446 définit la fonction et le membre de l'archive à l'offset de fichier 454 définit le name2.

```
Offset     +0   +1   +2   +3   +4   +5   +6   +7
          _______________________________________
 0       |                  4                    | 4 offset entries
         |_______________________________________|
 8       |                134                    | name
         |_______________________________________|
16       |                142                    | object
         |_______________________________________|
24       |                446                    | function
         |_______________________________________|
32       |                454                    | name2
         |_______________________________________|
40       |  n | a  | m  | e  | \0 | o  | b  | j  |
         |____|____|____|____|___________________|
48       |  e | c  | t  | \0 |  f | u  | n  | c  |
         |____|____|____|____|___________________|
56       |  t | i  | o  | n  | \0 | n  | a  | m  |
         |____|____|____|____|___________________|
64       |  e | 2  | \0 |    |
         |____|____|____|____|
```


Si le nom d'un membre de l'archive est plus long que 15 octets, un membre spécial de l'archive contient une table des noms de fichiers, chacun suivi d'une barre oblique et d'un saut de ligne. Ce membre de table de chaînes, s'il est présent, précédera tous les membres de l'archive «normaux». La table de symboles spéciale de l'archive n'est pas un membre «normal» et doit être le premier s'il existe. L'entrée ar_name de l'en-tête du membre de la table de chaînes contient un nom de longueur zéro ar_name [0] == '/', suivi d'une barre oblique finale (ar_name [1] == '/'), suivie d'espaces (ar_name [2] == ' ', etc.). Les décalages dans la table de chaînes commencent à zéro. Des exemples de valeurs ar_name pour des noms de fichiers courts et longs apparaissent ci-dessous.


```
Offset   +0   +1   +2   +3   +4   +5   +6   +7   +8   +9
       __________________________________________________
 0     | f  | i  | l  | e  | _  | n  | a  | m  | e  | _  |
       |____|____|____|____|____|____|____|____|____|____|
10     | s  | a  | m  | p  | l  | e  | /  | \n | l  | o  |
       |____|____|____|____|____|____|____|____|____|____|
20     | n  | g  | e  | r  | f  | i  | l  | e  | n  | a  |
       |____|____|____|____|____|____|____|____|____|____|
30     | m  | e  | x  | a  | m  | p  | l  | e  | /  | \n |
       |____|____|____|____|____|____|____|____|____|____|
```

```
   Member Name                            ar_name
_______________________________________________________________
short-name           | short-name/  | Not in string table
                     |              |
file_name_sample     | /0           | Offset 0 in string table
                     |              |
longerfilenamexample | /18          | Offset 18 in string table
_____________________|______________|___________________________
```