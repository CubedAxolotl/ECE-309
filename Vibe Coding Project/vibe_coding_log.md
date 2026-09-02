# Vibe Coding Log — C Command-Line Program

**Date:** September 1, 2026
**Topic:** Writing and debugging a simple C command-line program (`harness.c`)

---

## 1. Initial Request

Wrote a beginner-friendly C program using only standard libraries (`stdio.h`, `string.h`) that:
- Runs an infinite loop reading input with `fgets`
- Exits the loop when the user types `exit`
- Prints a hardcoded greeting if the input contains `hello`
- Otherwise echoes the input back

Key functions used: `fgets`, `strcspn` (to strip the trailing newline), `strcmp` (exact match for `exit`), and `strstr` (substring match for `hello`).

---

## 2. Debugging Journey

| Step | Issue | Cause | Fix |
|------|-------|-------|-----|
| 1 | `./program./program` not recognized | Typo — ran the path twice by accident, plus wrong syntax for Windows | Use `./program.exe` or `.\program.exe` |
| 2 | File was named `harness.cpp` | Wrong extension — treated as C++ instead of C | Rename to `harness.c` |
| 3 | `.\harness.exe` not recognized after compiling | `gcc` exited with code `1` — compile silently failed, no `.exe` created | Investigate further |
| 4 | Errors weren't printing anywhere, even redirected to a file | Ruled out: Desktop folder permissions, Windows Defender (checked protection history — clean) | Investigate `cc1.exe` directly |
| 5 | `cc1.exe --version` produced zero output | Root cause found: **gcc was installed via WSL (Ubuntu on Windows)**, not as a native Windows binary. PowerShell was invoking the wrong/incomplete toolchain. | Run the compiler through WSL |

### Root Cause & Solution
The `gcc` installed was inside **WSL (Windows Subsystem for Linux)**, not a native Windows install. The correct way to compile and run was:

```powershell
wsl gcc harness.c -o harness
wsl ./harness
```

Or, simply work directly inside the Ubuntu/WSL terminal:

```bash
gcc harness.c -o harness
./harness
```

---

## 3. Final Working Program (`harness.c`)

```c
#include <stdio.h>   // needed for printf, fgets, etc.
#include <string.h>  // needed for strstr, strcspn

int main(void) {
    // This buffer will hold whatever the user types.
    char input[100];

    // Start an infinite loop. We'll break out of it manually when needed.
    while (1) {
        // Ask the user for input.
        printf("Enter something (type 'exit' to quit): ");

        // fgets reads a line of text from the keyboard (stdin).
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // Remove the trailing newline character that fgets keeps.
        input[strcspn(input, "\n")] = '\0';

        // Check if the user typed exactly "exit".
        if (strcmp(input, "exit") == 0) {
            printf("Exiting program. Goodbye!\n");
            break;
        }

        // Check if the word "hello" appears anywhere in the input.
        if (strstr(input, "hello") != NULL) {
            printf("Hello there! Nice to see you.\n");
        }
        else {
            // Otherwise, echo the input back to the user.
            printf("You said: %s\n", input);
        }
    }

    return 0;
}
```

---

## 4. Deterministic Test Script (`test.sh`)

To verify the program behaves the same way every time, a small Bash script feeds fixed input into it automatically:

```bash
#!/bin/bash
# This script feeds fixed input into the harness program
# so we can check its output is always the same (deterministic).

printf "hello\nexit\n" | ./harness
```

**Usage:**
```bash
chmod +x test.sh
./test.sh
```

Optional variant that saves output to a file for comparison:

```bash
#!/bin/bash
printf "hello\nexit\n" | ./harness > output.txt
echo "Output saved to output.txt"
```

---

## 5. Key Takeaways

- `.cpp` vs `.c` matters — it changes which compiler/language rules apply.
- On Windows, compiled programs need `.exe`, and PowerShell requires `./` or `.\` to run local executables.
- A `gcc` exit code of `1` with **no visible error text** is a red flag — it usually means an early compilation stage (like `cc1.exe`) is crashing or not running at all, rather than a code syntax error.
- Multiple installations of `gcc` (e.g., MSYS2 vs WSL) can conflict — always confirm *which* toolchain is actually being invoked with `where.exe gcc` or by checking `$env:Path`.
- Piping fixed input via `printf "input\n" | ./program` is a simple, effective way to make interactive CLI programs deterministic for testing.
