#include <stdio.h>

void write_vlv(unsigned long value, unsigned char vlv[5])
{
    for (int i = 31; i >= 0; i--)
        printf("%lu", value >> i & 1);
    puts("");
    for (int i = 0; i < 32; i++) {
        unsigned long val = value >> i & 1;

    }
    int top = 0;

    puts("");
    return;
}

unsigned long read_vlv(unsigned char vlv[5])
{
    unsigned long result = 0;
    for (int i = 0; i < 5; i++)
        result += vlv[i] & 0x7F;
    return result;
}

int main(int argc, char *argv[])
{
    unsigned char arr[5] = {0xff, 0x91, 0x81, 0x83, 0x1};
    printf("%lu\n", read_vlv(arr));

    unsigned char vlv[5] = { 0 };
    write_vlv(4294967295, vlv);
    for (int i = 0; i < 5; i++)
        printf("%d ", vlv[i]);
    puts("");

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
