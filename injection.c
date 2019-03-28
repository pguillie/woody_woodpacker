#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define JMP_IDX (10)

int opcode[] = {/* writes "Hello, World!" and jumps */
	0x00000ee8, 0x6c654800, 0x202c6f6c, 0x6c726f57,
	0xb80a2164, 0x00000001, 0x000001bf, 0x0eba5e00,
	0x0f000000, 0xe9909005, 0x42424242
};

static int inject(void *ptr)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)ptr;
	Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)ptr + ehdr->e_phoff);
	Elf64_Half phnum = ehdr->e_phnum;

	while (phnum-- && !(phdr->p_type == PT_LOAD && phdr->p_flags & PF_X))
		phdr++;
	Elf64_Addr entry = phdr->p_offset + phdr->p_filesz;
	opcode[JMP_IDX] = (int)(ehdr->e_entry - entry - sizeof(opcode));
	printf("Jump to original entry point patched: 'jmp %#x'\n",
		opcode[JMP_IDX]);
	ehdr->e_entry = entry;
	printf("New entry point set to %#x\n", ehdr->e_entry);
	phdr->p_filesz += sizeof(opcode);
	phdr->p_memsz += sizeof(opcode);
	memcpy((char *)ptr + entry, opcode, sizeof(opcode));
	return (0);
}


int main(int ac, char *av[])
{
	void *ptr;
	struct stat buf;
	int fd;

	fd = open((ac == 2 ? av[1] : "a.out"), O_RDONLY);
	if (fd < 0) {
		dprintf(2, "Failed to open %s\n", (ac == 2 ? av[1] : "a.out"));
		return (1);
	}
	if (fstat(fd, &buf) < 0) {
		fputs("Failed to get file stat", stderr);
		close(fd);
		return (1);
	}
	ptr = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0);
	close(fd);
	if (ptr == MAP_FAILED) {
		fputs("Failed to map file", stderr);
		return (1);
	}
	inject(ptr); //return value
	fd = open((ac == 2 ? av[1] : "a.out"), O_WRONLY);
	if (fd < 0) {
		fputs("Failed to reopen file", stderr);
		munmap(ptr, buf.st_size);
		return (1);
	}
	if (write(fd, ptr, buf.st_size) < 0) {
		fputs("Failed to write changes", stderr);
		close(fd);
		munmap(ptr, buf.st_size);
		return (1);
	}
	close(fd);
	munmap(ptr, buf.st_size);
	puts("Code successfully injected !");
	return (0);
}
