#include <stdio.h>

// vlv functions taken from Teragonaudio.com
size_t write_vlv(unsigned long value, unsigned char vlv[5])
{
    unsigned long buffer = value & 0x7F;

    while ((value >>= 7)) {
        buffer <<= 8;
        buffer |= ((value & 0x7F) | 0x80);
    }

    size_t i = 0;
    for (;;) {
        vlv[i++] = (unsigned char)buffer;
        if (buffer & 0x80)
            buffer >>= 8;
        else
            return i;
    }
}

unsigned long read_vlv(unsigned char vlv[5])
{
    unsigned long value;
    unsigned char c;
    int i = 0;

    if ((value = vlv[i++]) & 0x80) {
        value &= 0x7F;
        do
            value = (value << 7) + ((c = vlv[i++]) & 0x7F);
        while (c & 0x80);
    }

    return value;
}

int main(int argc, char *argv[])
{
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
