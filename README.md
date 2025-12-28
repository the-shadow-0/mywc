# mywc — A modern, lightweight `wc` replacement 

![Build Status](https://img.shields.io/badge/build-passing-brightgreen) ![License](https://img.shields.io/badge/license-MIT-blue) ![Platform](https://img.shields.io/badge/platform-unix%2Flinux-lightgrey)

> **mywc** — a small, fast, POSIX-friendly `wc` clone written in modern C. Works with files or `stdin`, supports `-l`, `-w` and `-c`, and is designed to be readable, testable and easy to extend.

---

## Highlights

* ✅ Simple, portable C (ISO C11)
* ✅ Efficient buffered I/O (`fread`) for speed
* ✅ Correct byte counting (`-c`) and word detection (uses `isspace()`)
* ✅ Handles multiple files and prints totals like GNU `wc`
* ✅ Clear CLI with `-l`, `-w`, `-c` (defaults to all when no flags given)
* ✅ Friendly error messages and exit codes

---

## Table of Contents

1. [Quick start](#quick-start)
2. [Build & install](#build--install)
3. [Usage examples](#usage-examples)
4. [Design notes](#design-notes)
5. [Contributing](#contributing)
6. [License](#license)

---

## Quick start

Clone and build:

```bash
git clone https://github.com/the-shadow-0/mywc.git
cd mywc
gcc -std=c11 -O2 -o mywc mywc.c
```

Run against a file (or use `stdin`):

```bash
./mywc file.txt
cat file.txt | ./mywc
./mywc -l -w file1.txt file2.txt
```

---

## Build & install

**Compile locally**

```bash
gcc -std=c11 -O2 -o mywc mywc.c
```

**Install to `/usr/local/bin`** (requires `sudo` on most systems)

```bash
sudo install -m755 mywc /usr/local/bin/mywc
```

**Notes**: The program uses only the C standard library and should compile cleanly on Linux, macOS and other POSIX-like systems. For Windows, use a POSIX compatibility layer (WSL or MSYS2) or adapt the I/O mode.

---

## Usage examples

**Default (lines, words, bytes):**

```bash
./mywc file.txt
# =>  12  34 1234 file.txt
```

**Only lines:**

```bash
./mywc -l file.txt
# =>    12 file.txt
```

**Multiple files + totals:**

```bash
./mywc fileA.txt fileB.txt
# =>   10  20 200 fileA.txt
# =>   15  25 300 fileB.txt
# =>   25  45 500 total
```

---

## Design notes

* **Word detection** uses `isspace()` so behavior follows the current C locale for whitespace detection.
* **Bytes** are counted from the number of bytes read from the stream (matching `wc -c`).
* The implementation is intentionally small and readable — perfect for teaching, embedding in small toolchains, or replacing `wc` in restricted environments.

---

## Contributing

Contributions are welcome! Suggested workflow:

1. Fork the repo
2. Create a branch for your feature or fix
3. Add tests where appropriate
4. Open a pull request with a clear description

Please follow the repository's coding conventions. Add a `TODO.md` or issue for larger design changes.

---

## License

This project is released under the **MIT License** — see `LICENSE`.

---

## Acknowledgements

Inspired by the traditional LINUX `wc` utility and minimal C teaching examples. Contributions and improvements are appreciated!

---
