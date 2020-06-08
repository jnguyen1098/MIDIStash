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

    size_t chunklen = 4 + 4;

    unsigned char vlv[5] = { 0 };
    fseek(binaryfile, 0, SEEK_END);
    unsigned long binaryfile_size = ftell(binaryfile);
    size_t num_bytes = write_vlv(binaryfile_size, vlv);

    chunklen += num_bytes;
    chunklen += binaryfile_size;
    unsigned char vlv2[5] = { 0 };
    printf("chunklen is %lu\n", chunklen);
    size_t meta_bytes = write_vlv(chunklen, vlv2);
    for (size_t i = 0; i < 4 - meta_bytes; i++)
        fputc(0x00, midifile);
    for (size_t i = 0; i < meta_bytes; i++)
        fputc((char)vlv2[i], midifile);

    rewind(binaryfile);

    char start_of_text_event[] = {0xFF, 0x01, 0x00};
    fputc(0x0, midifile);
    fputs(start_of_text_event, midifile);

    for (size_t i = 0; i < num_bytes; i++) {
        printf("byte: %x\n", vlv[i]);
        fputc((char)vlv[i], midifile);
    }

    // write bytes here
    for (;;) {
        char c = fgetc(binaryfile);
        if (c == EOF) break;
        fputc(c, midifile);
    }

    char end_of_track_event[] = {0xFF, 0x2F, 0x00, 0x00};
    fputc(0x0, midifile);
    fputs(end_of_track_event, midifile);
    fputc(0x0, midifile);

    rewind(midifile);
    return 1;

    // 00 FF 01 1A => start of text event
    // 00 FF 2F 00 => end of track event
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        return fprintf(stderr, "Usage: %s midifile binaryfile\n", argv[0]), 1;

    FILE *midifile, *binaryfile;
    if (!(midifile = fopen(argv[1], "r+")) || !(binaryfile = fopen(argv[2], "r")))
        return fprintf(stderr, "File error\n"), 2;

    if (!operate_midi(midifile, binaryfile))
        return fprintf(stderr, "Invalid MIDI\n"), 3;

    fclose(midifile);
    fclose(binaryfile);
    return 0;
}
