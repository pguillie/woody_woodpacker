#include <stdio.h>
#include <stdint.h>

void
hash(uint32_t *h, unsigned char c)
{
	h[7] = (h[7] << 5) + h[7] + c + (h[0] ^ c);
	h[6] = (h[6] << 5) + h[6] + h[7];
	h[5] = (h[5] << 5) + h[5] + h[6] + (c << 4);
	h[4] = (h[4] << 5) + h[4] + h[5] + (c << 8);
	h[3] = (h[3] << 5) + h[3] + h[4] + (c << 12);
	h[2] = (h[2] << 5) + h[2] + h[3] + (c << 16);
	h[1] = (h[1] << 5) + h[1] + h[2] + (c << 18);
	h[0] = (h[0] << 5) + h[0] + h[1] + (c << 24);
}

int
main(int ac, char *av[])
{
	uint32_t h[8] = {5381, 5381, 5381, 5381, 5381, 5381, 5381, 5381};

	if (ac != 2) {
		puts("usage");
		return (1);
	}

	//hash
	int len = 0;
	while (av[1][len])
		hash(h, av[1][len++]);
	while (len--)
		hash(h, av[1][len]);

	//display
	char buf[257] = {0};
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 32; j++)
			buf[i * 32 + j] = h[i] & (1 << (31 - j)) ? '1' : '0';
	puts(buf);

	return (0);
}
