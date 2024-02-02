#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c\n"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 


int main ()
{
	int flags = ((1<<1)|(1<<2)|(1<<5));
	printf("Unset Flag:\n");
	int flag3 = (1<<2);
	printf("Flags   :"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(flags));
	printf("Flag 3  :"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(flag3));
	printf("~Flag 3 :"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(~flag3));
	flags &= ~flag3;
	printf("Do (flags &= ~flag3):\n");
	printf("Flags   :"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(flags));
	printf("Set Flag:\n");
	printf("Do (flags |= (1 << 0)):\n");
	flags |= (1 << 0);
	printf("Flags   :"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(flags));
	return (0);
}
