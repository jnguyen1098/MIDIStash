#include <stdio.h>

void write_vlv(unsigned long value, char vlv[5])
{
    return;
}

unsigned long read_vlv(char vlv[5])
{
    return 0;
}

int main(int argc, char *argv[])
{
    char arr[5] = {81, 81, 81, 81, 1};
    printf("%lu\n", read_vlv(arr));

    if (argc == 1)
        return fprintf(stderr, "Usage: %s midifile [binaryfile]\n", argv[0]), 1;

    // decrypt
    if (argc == 2) {
        FILE *midifile;
        if (!(midifile = fopen(argv[1], "r")))
            return fprintf(stderr, "File error\n"), 2;

        // 

        return 0;
    }

    // encrypt
    else if (argc == 3) {
        FILE *midifile, *binaryfile;
        if (!(midifile = fopen(argv[1], "r+")) || !(binaryfile = fopen(argv[2], "r")))
            return fprintf(stderr, "File error\n"), 2;

        // 

        return 0;
    }

    return fprintf(stderr, "Usage: %s midifile [binaryfile]\n", argv[0]);
}
