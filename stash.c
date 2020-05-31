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

int operate_midi(FILE *midifile, FILE *binaryfile)
{
    // ptr sanity
    if (!midifile)
        return 0;

    // MThd 0x00 0x00 0x00 0x06
    if (fgetc(midifile) != 0x4D || fgetc(midifile) != 0x54 ||
        fgetc(midifile) != 0x68 || fgetc(midifile) != 0x64 ||
        fgetc(midifile) != 0x00 || fgetc(midifile) != 0x00 ||
        fgetc(midifile) != 0x00 || fgetc(midifile) != 0x06)
        return 0;

    // Type-1 MIDI
    if (fgetc(midifile) != 0x00 || fgetc(midifile) != 0x01)
        return 0;

    // Track count
    size_t trkcnt = (fgetc(midifile) << 8) + fgetc(midifile) + 1;
    fseek(midifile, -2, SEEK_CUR);
    fputc((unsigned char)((trkcnt >> 8) % 256), midifile);
    fputc((unsigned char)((trkcnt % 256)), midifile);

    fseek(midifile, 0, SEEK_END);

    char str[] = {'M', 'T', 'r', 'k', '\0'};
    fputs(str, midifile);

    unsigned char vlv[5] = { 0 };
    fseek(binaryfile, 0, SEEK_END);
    unsigned long binaryfile_size = ftell(binaryfile);
    size_t num_bytes = write_vlv(binaryfile_size, vlv);
    rewind(binaryfile);
    for (size_t i = 0; i < num_bytes; i++) {
        printf("byte: %x\n", vlv[i]);
        fputc((char)vlv[i], midifile);
    }

    rewind(midifile);
    return 1;
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

        fclose(midifile);
        return 0;
    }

    // encrypt
    else if (argc == 3) {
        FILE *midifile, *binaryfile;
        if (!(midifile = fopen(argv[1], "r+")) || !(binaryfile = fopen(argv[2], "r")))
            return fprintf(stderr, "File error\n"), 2;

        if (!operate_midi(midifile, binaryfile))
            return fprintf(stderr, "Invalid MIDI\n"), 3;

        fclose(midifile);
        fclose(binaryfile);
        return 0;
    }

    return fprintf(stderr, "Usage: %s midifile [binaryfile]\n", argv[0]);
}
