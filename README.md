# BlinkDB

## Introduction

**BlinkDB** is an in-memory key-value store inspired by Redis. It provides a REPL for interactive usage, a server that runs in different modes, and a client for connecting to the server. Additionally, automated benchmarking is supported using `redis-benchmark`.

---

## Table of Contents

- [Compilation Instructions](#compilation-instructions)
  - [Compiling the REPL](#compiling-the-repl)
  - [Compiling the Server](#compiling-the-server)
  - [Compiling the Client](#compiling-the-client)
- [Running Instructions](#running-instructions)
  - [Running the REPL](#running-the-repl)
  - [Running the Server](#running-the-server)
  - [Running the Client](#running-the-client)
  - [Benchmarking](#benchmarking)

---

## Compilation Instructions

### Compiling the REPL

To compile the **REPL**, run:

```bash
make -f REPLMakeFile
```

### Compiling the Server

To compile the **Server**, run:

```bash
make -f ServerMakeFile
```

### Compiling the Client

To compile the **Client**, run:

```bash
make -f ClientMakeFile
```

---

## Running Instructions

### Running the REPL

The REPL can be used in two modes:

1. **Interactive User Mode:**

   ```bash
   ./repl 0
   ```

   - Allows manual interaction with BlinkDB.

2. **Test Mode:**

   ```bash
   ./repl 1 <filename>
   ```

   - Runs predefined commands from the specified file.

### Running the Server

The server supports two modes:

1. **Client-Server Mode:**

   ```bash
   ./server 0
   ```

   - Runs the server for client connections.

2. **Redis-Benchmark Mode:**

   ```bash
   ./server 1
   ```

   - Runs the server optimized for `redis-benchmark`.

### Running the Client

The client can be executed in two modes:

1. **Interactive User Mode:**

   ```bash
   ./client 0
   ```

   - Connects to the server for interactive queries.

2. **Test Mode:**

   ```bash
   ./client 1 <filename>
   ```

   - Runs queries from a test file.

### Benchmarking

To run automated benchmarking tests, execute:

```bash
./Benchmark.sh
```

- This will perform `redis-benchmark` tests and save results in the `/results` directory.
- The benchamrking involves 10,000; 1,00,000 and 10,00,000 commands and 10, 100 and 1000 parallel connections.

---

## Notes

- Ensure the server is running before executing the client, REPL or Benchmark.
- Use appropriate filenames when running test mode.
- Benchmark results are stored in `/results`.

---
