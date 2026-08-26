#include "s21_cat.h"

int main(int argc, char *argv[])
{
    Flags flags;
    init_flags(&flags);

    int option;

    while ((option = getopt_long(argc, argv, "bensvtET", long_options, NULL)) !=
           -1)
    {
        switch (option)
        {
        case 'n':
            flags.n = 1;
            break;

        case 'b':
            flags.b = 1;
            break;

        case 's':
            flags.s = 1;
            break;

        case 'v':
            flags.v = 1;
            break;

        case 'e':
            flags.e = 1;
            flags.v = 1;
            break;

        case 't':
            flags.t = 1;
            flags.v = 1;
            break;

        case 'E':
            flags.e = 1;
            break;

        case 'T':
            flags.t = 1;
            break;

        default:
            return 1;
        }
    }

    if (flags.b)
    {
        flags.n = 0;
    }

    if (optind == argc)
    {
        return 0;
    }
    else
    {
        for (int i = optind; i < argc; i++)
        {
            FILE *file = fopen(argv[i], "r");

            if (file == NULL)
            {
                perror(argv[i]);
                continue;
            }

            process_file(file, &flags);
            fclose(file);
        }
    }

    return 0;
}

void init_flags(Flags *flags)
{
    flags->n = 0;
    flags->b = 0;
    flags->s = 0;
    flags->v = 0;
    flags->e = 0;
    flags->t = 0;
}

void print_char(unsigned char c, Flags *flags)
{
    if (flags->e && c == '\n')
    {
        putchar('$');
    }
    else if (flags->t && c == '\t')
    {
        printf("^I");
        return;
    }

    if (!flags->v)
    {
        putchar(c);
        return;
    }

    if (c == 127)
    {
        printf("^?");
    }
    else if (c >= 128 && c <= 159)
    {
        printf("M-^%c", c - 64);
    }
    else if (c >= 160 && c <= 254)
    {
        printf("M-%c", c - 128);
    }
    else if (c == 255)
    {
        printf("M-^?");
    }
    else if (c < 32 && c != '\n' && c != '\t')
    {
        printf("^%c", c + 64);
    }
    else
    {
        putchar(c);
    }
}

void process_file(FILE *file, Flags *flags)
{
    int c;
    int line = 1;
    int prev = '\n';
    int empty = 0;

    while ((c = fgetc(file)) != EOF)
    {
        if (flags->s)
        {
            if (c == '\n' && prev == '\n')
            {
                empty++;

                if (empty > 1)
                {
                    prev = c;
                    continue;
                }
            }
            else
            {
                empty = 0;
            }
        }

        if (prev == '\n')
        {
            if (flags->b)
            {
                if (c != '\n')
                {
                    printf("%6d\t", line);
                    line++;
                }
            }
            else if (flags->n)
            {
                printf("%6d\t", line);
                line++;
            }
        }

        print_char((unsigned char)c, flags);

        prev = c;
    }
}