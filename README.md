# Custom Unix Shell (C)

I built this as a student operating systems project in C.
The main goal was to make my own shell that handles built-in OS-style commands like `cd`, `dir`, `echo`, `environ`, `help`, `pause`, and `quit`, plus simple input/output redirection and background execution.

After I got the C shell working, I made a web demo so I could show how the shell behaves without needing to compile and run it locally.
The real project is still the C implementation in `src`.

## Build

From the `src` folder:

```bash
make myshell
```

This builds the binary to `bin/myshell`.

Run it from the project root:

```bash
./bin/myshell
```
