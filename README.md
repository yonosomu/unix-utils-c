# Unix Utils in C

Custom implementations of the Unix `cat` and `grep` command-line utilities written in C.

## Features

### s21_cat

Implementation of the `cat` utility for displaying file contents.

Supported flags:

- `-b` — number non-empty output lines
- `-e` — display `$` at the end of each line and non-printing characters
- `-n` — number all output lines
- `-s` — suppress repeated empty lines
- `-t` — display TAB characters as `^I`
- `-v` — display non-printing characters

### s21_grep

Implementation of the `grep` utility for searching text patterns in files.

Supported flags:

- `-e` — pattern
- `-i` — ignore case
- `-v` — invert match
- `-c` — count matching lines
- `-l` — print names of matching files
- `-n` — print line numbers
- `-h` — suppress file names in output
- `-s` — suppress error messages
- `-f` — take patterns from a file
- `-o` — print only matching parts of a line

## Technologies

- C
- POSIX
- Regular expressions
- File I/O
- Command-line argument parsing
- Make

## Build

Build both utilities:

make

Or build them separately:

make s21_cat
make s21_grep

## Usage

### cat

./s21_cat [OPTIONS] [FILE...]

Example:

./s21_cat -n test.txt

### grep

./s21_grep [OPTIONS] PATTERN [FILE...]

Example:

./s21_grep -in "hello" test.txt

## Testing

The implementations were tested against the behavior of the standard Unix `cat` and `grep` utilities.

## Project Structure

s21_cat/
├── s21_cat.c
└── ...

s21_grep/
├── s21_grep.c
└── ...

## Purpose

The project was created to practice C programming and systems programming concepts, including file I/O, command-line argument parsing, string processing, regular expressions, and working with Unix/POSIX APIs.