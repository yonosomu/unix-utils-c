#include "s21_grep.h"

void add_pattern(Flags *flags, const char *pattern)
{
    if (flags->pattern_count < MAX_PATTERNS)
    {
        flags->patterns[flags->pattern_count++] = strdup(pattern);
    }
}

void load_patterns_from_file(Flags *flags, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        if (!flags->s)
            perror(filename);
        return;
    }

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1)
    {
        line[strcspn(line, "\r\n")] = '\0';
        add_pattern(flags, line);
    }

    free(line);
    fclose(file);
}

void parse_args(int argc, char **argv, Flags *flags)
{
    char *positional_args[MAX_PATTERNS + MAX_FILES];
    int positional_count = 0;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && argv[i][1] != '\0')
        {
            char *flag_str = argv[i];

            for (int j = 1; flag_str[j] != '\0'; j++)
            {
                if (flag_str[j] == 'e')
                {
                    flags->e = 1;

                    if (flag_str[j + 1] != '\0')
                    {
                        add_pattern(flags, &flag_str[j + 1]);
                    }
                    else if (i + 1 < argc)
                    {
                        add_pattern(flags, argv[i + 1]);
                        i++;
                    }
                    break;
                }
                else if (flag_str[j] == 'f')
                {
                    flags->f = 1;

                    if (flag_str[j + 1] != '\0')
                    {
                        load_patterns_from_file(flags, &flag_str[j + 1]);
                    }
                    else if (i + 1 < argc)
                    {
                        load_patterns_from_file(flags, argv[i + 1]);
                        i++;
                    }
                    break;
                }
                else
                {
                    switch (flag_str[j])
                    {
                    case 'i':
                        flags->i = 1;
                        break;
                    case 'v':
                        flags->v = 1;
                        break;
                    case 'c':
                        flags->c = 1;
                        break;
                    case 'l':
                        flags->l = 1;
                        break;
                    case 'n':
                        flags->n = 1;
                        break;
                    case 'h':
                        flags->h = 1;
                        break;
                    case 's':
                        flags->s = 1;
                        break;
                    case 'o':
                        flags->o = 1;
                        break;
                    }
                }
            }
        }
        else
        {
            positional_args[positional_count++] = argv[i];
        }
    }

    int start_file_idx = 0;

    if (flags->pattern_count == 0 && positional_count > 0)
    {
        add_pattern(flags, positional_args[0]);
        start_file_idx = 1;
    }

    for (int i = start_file_idx; i < positional_count; i++)
    {
        if (flags->file_count < MAX_FILES)
        {
            flags->files[flags->file_count++] = positional_args[i];
        }
    }

    if (flags->pattern_count == 0 || flags->file_count == 0)
    {
        fprintf(stderr, "usage: s21_grep [OPTIONS] PATTERN FILE...\n");
        exit(1);
    }
}

int compile_all_regexes(Flags *flags)
{
    int reg_flags = REG_EXTENDED;
    if (flags->i)
        reg_flags |= REG_ICASE;

    for (int i = 0; i < flags->pattern_count; i++)
    {
        if (regcomp(&(flags->compiled_regexes[i]), flags->patterns[i], reg_flags) !=
            0)
        {
            if (!flags->s)
            {
                fprintf(stderr, "grep: trailing backslash (\\)\n");
            }
            return 0;
        }
    }
    return 1;
}

int match_any_pattern(Flags *flags, const char *line)
{
    for (int i = 0; i < flags->pattern_count; i++)
    {
        if (regexec(&(flags->compiled_regexes[i]), line, 0, NULL, 0) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void print_only_matches(Flags *flags, const char *line, const char *filename,
                        int line_number)
{
    regmatch_t match;

    for (int p = 0; p < flags->pattern_count; p++)
    {
        const char *ptr = line;
        while (regexec(&(flags->compiled_regexes[p]), ptr, 1, &match, 0) == 0)
        {
            if (match.rm_so == match.rm_eo)
            {
                break;
            }

            if (flags->file_count > 1 && !flags->h)
            {
                printf("%s:", filename);
            }
            if (flags->n)
            {
                printf("%d:", line_number);
            }

            printf("%.*s\n", (int)(match.rm_eo - match.rm_so), ptr + match.rm_so);
            ptr += match.rm_eo;
        }
    }
}

void process_file(Flags *flags, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        if (!flags->s)
            perror(filename);
        return;
    }

    char *line = NULL;
    size_t len = 0;
    int line_number = 0;
    int count = 0;

    while (getline(&line, &len, file) != -1)
    {
        line_number++;
        int matched = match_any_pattern(flags, line);

        if (flags->v)
            matched = !matched;

        if (matched)
        {
            count++;

            if (flags->l)
                continue;
            if (flags->c)
                continue;

            if (flags->o && !flags->v)
            {
                print_only_matches(flags, line, filename, line_number);
                continue;
            }

            if (flags->file_count > 1 && !flags->h)
            {
                printf("%s:", filename);
            }
            if (flags->n)
            {
                printf("%d:", line_number);
            }

            printf("%s", line);
            if (strlen(line) == 0 || line[strlen(line) - 1] != '\n')
            {
                printf("\n");
            }
        }
    }

    if (flags->c)
    {
        if (flags->file_count > 1 && !flags->h)
        {
            printf("%s:", filename);
        }
        if (flags->l)
        {
            printf("%d\n", count > 0 ? 1 : 0);
        }
        else
        {
            printf("%d\n", count);
        }
    }

    if (flags->l && count > 0)
    {
        printf("%s\n", filename);
    }

    free(line);
    fclose(file);
}

void free_flags(Flags *flags)
{
    for (int i = 0; i < flags->pattern_count; i++)
    {
        free(flags->patterns[i]);
        regfree(&(flags->compiled_regexes[i]));
    }
}

int main(int argc, char **argv)
{
    Flags flags = {0};

    parse_args(argc, argv, &flags);

    if (!compile_all_regexes(&flags))
    {
        free_flags(&flags);
        return 1;
    }

    for (int i = 0; i < flags.file_count; i++)
    {
        process_file(&flags, flags.files[i]);
    }

    free_flags(&flags);
    return 0;
}
