*This project has been created as part of the 42 curriculum by <bgomes-b>.*

# ft_printf

## Description

The **ft_printf** project consists in recreating the standard C library function `printf`. The goal is to gain a deep understanding of **variadic functions**, **format parsing**, and **low-level output handling** in C.

This implementation reproduces the behavior of `printf` for a defined set of conversion specifiers and flags, handling formatted output to the standard output (stdout).

By completing this project, students improve their skills in:
- Parsing strings and complex flags
- Managing variable argument lists
- Modular and reusable code design
- Memory and output control in C

## Instructions

### Compilation

The project is compiled using `make`. To include the bonus features, use the command: `make bonus`. This will generate a static library named `libftprintf.a`.

### Usage

1. Include the header in your project: `#include "ft_printf.h"`
2. Compile your program by linking the library: `gcc main.c libftprintf.a`
3. Example call: `ft_printf("Hex: %#x | Width: %10d | Signal: %+d\n", 255, 42, 42);`

### Supported Conversions & Bonus Flags

**Mandatory conversions:**
- %c : character
- %s : string
- %p : pointer address
- %d / %i : signed decimal integer
- %u : unsigned decimal integer
- %x / %X : hexadecimal (lowercase / uppercase)
- %% : percent sign

**Bonus Flags implemented:**
- '-' : Left-justify within the given field width.
- '0' : Left-pad the number with zeroes instead of spaces.
- '.' : Precision (minimum number of digits for integers, max characters for strings).
- '#' : Alternate form (adds 0x or 0X prefix to hexadecimals).
- ' ' (space) : Leaves a blank space before a positive number.
- '+' : Forces a plus sign before positive numbers.
- 'width' : Minimum number of characters to be printed.

## Algorithm and Data Structures

### Overall Approach

The algorithm processes the format string character by character:
1. If a regular character is found, it is written directly to stdout.
2. If a '%' character is found, a parsing function scans the string to extract flags, width, and precision.
3. These values are stored in a dedicated data structure.
4. The conversion specifier is analyzed to retrieve the argument via `va_arg`.
5. A dispatcher calls the appropriate handler, passing the flag structure to apply formatting rules.
The function returns the total number of printed characters.

### Variadic Arguments

The project uses the <stdarg.h> library:
- `va_start` to initialize the argument list.
- `va_arg` to retrieve arguments based on expected types.
- `va_end` to clean up.

### Data Structures

To handle the complexity of the bonus flags, a struct was implemented:
- **t_print**: A structure containing integer and boolean fields to track the state of all flags for each specific call. This prevents "propeller" functions with too many arguments and keeps the logic organized.

### Design Choices

- **Modular Handlers:** Each conversion has a dedicated function to manage the interaction between data types and the flag struct.
- **System Calls:** Output is performed using the `write` system call to comply with 42 constraints.
- **Recursive Logic:** Used for numeric conversions to handle digit order and base changes efficiently.

## Resources

### Technical References

- man printf / man stdarg
- GNU C Library Documentation
- https://en.cppreference.com/w/c/variadic

### AI Usage

AI tools were used as learning and support resources, specifically to:
- Flag Logic: Clarify the priority rules between conflicting flags (e.g., how '-' overrides '0').
- Refactoring: Suggestions to optimize the parsing loop and improve code modularity.
- Documentation: Assistance in structuring and translating this README to English.

All implementation decisions, coding, and debugging were done manually, in compliance with the 42 school's rules.

## Author

- <bgomes-b>
