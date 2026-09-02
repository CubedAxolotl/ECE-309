#!/bin/bash
# This script feeds fixed input into the harness program
# so we can check its output is always the same (deterministic).

# printf sends each word followed by a newline, one per line,
# just like a person typing "hello" and pressing Enter, then
# typing "exit" and pressing Enter.
printf "hello\nexit\n" | ./harness