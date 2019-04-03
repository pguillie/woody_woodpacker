#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define JMP_OFF (48)

char opcode[] = {
	0x50, 0x57, 0x56, 0x52, 0xe8, 0x0e, 0x00, 0x00,
	0x00, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20,
	0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x0a, 0xb8,
	0x01, 0x00, 0x00, 0x00, 0xbf, 0x01, 0x00, 0x00,
	0x00, 0x5e, 0xba, 0x0e, 0x00, 0x00, 0x00, 0x0f,
	0x05, 0x5a, 0x5e, 0x5f, 0x58, 0x90, 0x90, 0xe9,
	0x42, 0x42, 0x42, 0x42
};

static int inject(void *ptr)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)ptr;
	Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)ptr + ehdr->e_phoff);
	Elf64_Half phnum = ehdr->e_phnum;

	while (phnum-- && !(phdr->p_type == PT_LOAD && phdr->p_flags & PF_X))
		phdr++;
	Elf64_Addr entry = phdr->p_offset + phdr->p_filesz;
	*(int *)(opcode + JMP_OFF) = (int)(ehdr->e_entry - entry - sizeof(opcode));
	printf("Jump patched: 'jmp %#x'\n", *(int *)(opcode + JMP_OFF));
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
	inject(ptr);
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
