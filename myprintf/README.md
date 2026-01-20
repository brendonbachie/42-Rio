*This project has been created as part of the 42 curriculum by <bgomes-b>.*

# ft_printf

## Description

The **ft_printf** project consists in recreating the standard C library function `printf`.  
The goal is to gain a deep understanding of **variadic functions**, **format parsing**, and  
**low-level output handling** in C.

This implementation reproduces the behavior of `printf` for a defined set of conversion  
specifiers, handling formatted output to the standard output (`stdout`).

By completing this project, students improve their skills in:
- Parsing strings
- Managing variable argument lists
- Modular and reusable code design
- Memory and output control in C

## Instructions

### Compilation

The project is compiled using `make`:

```bash
make
```

This will generate a static library:

```bash
libftprintf.a
```

### Usage

Include the header and link the library in your project:

```c
#include "ft_printf.h"
```

Compile your program with:

```bash
gcc main.c libftprintf.a
```

### Example

```c
ft_printf("Hello %s, number: %d\n", "world", 42);
```

### Supported Conversions

Mandatory conversions implemented:
- `%c` : character
- `%s` : string
- `%p` : pointer address
- `%d` / `%i` : signed decimal integer
- `%u` : unsigned decimal integer
- `%x` / `%X` : hexadecimal (lowercase / uppercase)
- `%%` : percent sign

## Algorithm and Data Structures

### Overall Approach

The algorithm processes the format string character by character:

1. If a regular character is found, it is written directly to `stdout`.
2. If a `%` character is found:
   - The next character is analyzed to determine the conversion specifier.
   - The corresponding argument is extracted using `va_arg`.
   - A specific handler function is called to format and print the value.

The function keeps track of the total number of printed characters, which is returned at  
the end, mimicking the behavior of the original `printf`.

### Variadic Arguments

The project uses the `<stdarg.h>` library:
- `va_start` to initialize the argument list
- `va_arg` to retrieve arguments based on expected types
- `va_end` to clean up

### Data Structures

No complex data structures are required for the mandatory part.  
The project relies mainly on:
- Primitive types
- Strings (`char *`)
- Recursive or iterative number conversion functions

Numbers are converted to strings using:
- Division and modulo operations
- Recursive printing for correct digit order

### Design Choices

- Each conversion specifier is handled by a dedicated function to improve readability  
  and maintainability.
- Output is performed using the `write` system call to comply with 42 constraints.
- The code is modular to allow easy extension (e.g., bonus flags and width handling).

## Resources

### Technical References

- `man printf`
- `man stdarg`
- GNU C Library Documentation
- https://en.cppreference.com/w/c/variadic

### AI Usage

AI tools were used as **learning and support resources**, specifically to:
- Clarify concepts related to variadic functions and format parsing
- Review algorithm design and edge cases
- Improve code readability and documentation quality

All implementation decisions, coding, and debugging were done manually, in compliance  
with the 42 school's rules.

## Notes

- This project does not implement buffer management like the original `printf`.
- Behavior is undefined for unsupported or invalid format specifiers.
- Bonus features (flags, width, precision) are implemented only if explicitly included.

## Author

- <bgomes-b>
