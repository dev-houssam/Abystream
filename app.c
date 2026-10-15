#include <unistd.h>

#define EXIT_CODE 0
#define OUT 1

int 
main(void)
{
    write(OUT, "0x000000\n", 9);
    return EXIT_CODE;
}
