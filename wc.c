#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <locale.h>
#include <unistd.h>

#define BUFSIZE 8192

typedef unsigned long long ull;

struct counts
{
    ull lines;
    ull words;
    ull bytes;
};

static void print_usage(const char *prog)
{
    fprintf(stderr,
            "Usage: %s [-c] [-w] [-l] [file...]\n"
            "  -c    print byte counts\n"
            "  -w    print word counts\n"
            "  -l    print line counts\n"
            "If no option is given, print lines, words and bytes.\n",
            prog);
}

static int process_stream(FILE *f, struct counts *out)
{
    unsigned char buf[BUFSIZE];
    size_t n;
    int in_word = 0;

    while ((n = fread(buf, 1, BUFSIZE, f)) > 0)
    {
        out->bytes += n;
        for (size_t i = 0; i < n; ++i)
        {
            unsigned char ch = buf[i];
            if (ch == '\n')
                out->lines++;

            if (isspace((int)ch))
            {
                in_word = 0;
            }
            else
            {
                if (!in_word)
                {
                    out->words++;
                    in_word = 1;
                }
            }
        }
    }

    if (ferror(f))
    {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int opt;
    bool show_l = false, show_w = false, show_c = false;
    bool any_option = false;
    const char *prog = argv[0];

    setlocale(LC_CTYPE, "");

    while ((opt = getopt(argc, argv, "lwc?h")) != -1)
    {
        switch (opt)
        {
        case 'l':
            show_l = true;
            any_option = true;
            break;
        case 'w':
            show_w = true;
            any_option = true;
            break;
        case 'c':
            show_c = true;
            any_option = true;
            break;
        case '?':
        case 'h':
        default:
            print_usage(prog);
            return (opt == 'h') ? 0 : 1;
        }
    }

    if (!any_option)
    {
        show_l = show_w = show_c = true;
    }

    struct counts total = {0, 0, 0};
    int file_count = 0;
    int status = 0;

    if (optind == argc)
    {
        struct counts c = {0, 0, 0};
        if (process_stream(stdin, &c) != 0)
        {
            perror("stdin");
            return 1;
        }
        if (show_l)
            printf("%7llu", c.lines);
        if (show_w)
            printf("%7llu", c.words);
        if (show_c)
            printf("%7llu", c.bytes);
        putchar('\n');
        return 0;
    }

    for (int i = optind; i < argc; ++i)
    {
        const char *filename = argv[i];
        FILE *f = fopen(filename, "rb");
        if (f == NULL)
        {
            fprintf(stderr, "%s: cannot open '%s': %s\n", prog, filename, strerror(errno));
            status = 1;
            continue;
        }

        struct counts c = {0, 0, 0};
        if (process_stream(f, &c) != 0)
        {
            fprintf(stderr, "%s: read error on '%s': %s\n", prog, filename, strerror(errno));
            fclose(f);
            status = 1;
            continue;
        }
        fclose(f);

        if (show_l)
            printf("%7llu", c.lines);
        if (show_w)
            printf("%7llu", c.words);
        if (show_c)
            printf("%7llu", c.bytes);
        printf(" %s\n", filename);

        total.lines += c.lines;
        total.words += c.words;
        total.bytes += c.bytes;
        file_count++;
    }

    if (file_count > 1)
    {
        if (show_l)
            printf("%7llu", total.lines);
        if (show_w)
            printf("%7llu", total.words);
        if (show_c)
            printf("%7llu", total.bytes);
        printf(" total\n");
    }

    return status;
}
