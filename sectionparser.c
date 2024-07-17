#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

int main(int argc, char** argv)
{
	// Open ELF file & read header
	FILE *elfFile = fopen(argv[1], "r");
	Elf64_Ehdr header;
	fread(&header, sizeof(header), 1, elfFile);


	unsigned long int offset = header.e_shoff;	// File offset of first section header
	unsigned int nSections = header.e_shnum;	// Number of section headers
	unsigned int sizeSection = header.e_shentsize;  // Size of 1 section header


	// Getting section names
	Elf64_Shdr section;
	char* sectionNames = NULL;
	unsigned int shstrOffset = (header.e_shstrndx * header.e_shentsize) + header.e_shoff;
	
	fseek(elfFile, shstrOffset, SEEK_SET);
	fread(&section, 1, sizeof(section), elfFile);

	sectionNames = malloc(section.sh_size);
	fseek(elfFile, section.sh_offset, SEEK_SET);
	fread(sectionNames, 1, section.sh_size, elfFile);

	printf("%s\n", sectionNames);


	
	// --------------- Main Program ---------------

	printf("There are %d section headers, starting at offset 0x%lx:\n\n", nSections, offset);
	printf("Section Headers:\n");
	printf(" [Nr] Name\t\t Type\t\t   Address\t     Offset\n");
	printf("      Size\t\t EntSize\t   Flags  Link  Info  Align\n");


	fseek(elfFile, offset, SEEK_SET); // Point at first section header


	// Iterate through section headers & print contents	
	for (unsigned int i = 0; i < nSections; i++)
	{
		fread(&section, sizeof(section), 1, elfFile);

		// ------------ First row -------------
		
		printf(" [%2d] ", i);
		printf("%-16.16s\t", sectionNames + section.sh_name);

		switch(section.sh_type)
		{
			case SHT_NULL:                  printf(" NULL\t\t");         break;
			case SHT_PROGBITS:              printf(" PROGBITS\t");       break;
			case SHT_SYMTAB:                printf(" SYMTAB\t\t");       break;
			case SHT_STRTAB:                printf(" STRTAB\t\t");       break;
			case SHT_RELA:                  printf(" RELA\t\t");         break;
			case SHT_HASH:                  printf(" HASH\t\t");         break;
			case SHT_DYNAMIC:               printf(" DYN\t\t");          break;
			case SHT_NOTE:                  printf(" NOTE\t\t");         break;
			case SHT_NOBITS:                printf(" NOBITS\t\t");       break;
			case SHT_REL:                   printf(" REL\t\t");          break;
			case SHT_SHLIB:                 printf(" SHLIB\t\t");        break;
			case SHT_DYNSYM:                printf(" DYNSYM\t\t");       break;
			case SHT_INIT_ARRAY:            printf(" INIT_ARRAY\t");     break;
			case SHT_FINI_ARRAY:            printf(" FINI_ARRAY\t");     break;
			case SHT_PREINIT_ARRAY:         printf(" PRE_INIT_ARRAY\t"); break;
			case SHT_GROUP:                 printf(" GROUP\t\t");        break;
			case SHT_SYMTAB_SHNDX:          printf(" SYMTAB_SHNDX\t");   break;
			case SHT_NUM:                   printf(" NUM\t\t");          break;
			case SHT_LOOS ... SHT_HIOS:     printf(" OS\t\t");           break;
			case SHT_LOPROC ... SHT_HIPROC: printf(" PROC\t\t");         break;
			case SHT_LOUSER ... SHT_HIUSER: printf(" USER \t\t");        break;
			default:                        printf(" \t\t");             break;
		}
		
		printf("   %016lx", section.sh_addr);
		printf("  %08lx", section.sh_offset);
		printf("\n");



		
		// ------------ Second row ------------
	
		printf("     ");
		printf(" %016lx  ", section.sh_size);
		printf(" %016lx  ", section.sh_entsize);

		// Check for each flag individually
		if (section.sh_flags & 0x1)        printf("W");
		if (section.sh_flags & 0x2)        printf("A");
		if (section.sh_flags & 0x4)        printf("X");
		if (section.sh_flags & 0x10)       printf("M");
		if (section.sh_flags & 0x20)       printf("S");
		if (section.sh_flags & 0x40)       printf("I");
		if (section.sh_flags & 0x80)       printf("L");
		if (section.sh_flags & 0x100)      printf("O");
		if (section.sh_flags & 0x200)      printf("G");
		if (section.sh_flags & 0x400)      printf("T");
		if (section.sh_flags & 0x0FF00000) printf("o");
		if (section.sh_flags & 0xF0000000) printf("p");
		if (section.sh_flags & 0x40000000) printf("L");
		if (section.sh_flags & 0x80000000) printf("E");


		printf("\t  %4d", section.sh_link);
		printf("  %4d", section.sh_info);
		printf("  %4ld", section.sh_addralign);
		printf("\n");
		
	}



	// Close file
	fclose(elfFile);

	return 0;
}
