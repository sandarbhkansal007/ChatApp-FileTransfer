# FileTransfer Application

## Overview

This FileTransfer application is a client-server program written in C that enables users to request a file from the server. The server processes the request and sends the requested file to the client. The client then saves the received file. The server supports concurrent client connections, allowing multiple clients to request files simultaneously.

## Features

- **File Request**: Clients can request files from the server by specifying the file name.
- **File Transfer**: The server sends the requested file to the client.
- **Concurrent Server**: The server can handle multiple clients simultaneously using the `fork()` system call.
- **Simple Command-line Interface**: Easy to use and deploy via command-line.

## Requirements

- **Operating System**: Unix-like OS (Linux, macOS)
- **C Compiler**: GCC or any other C compiler
- **Network**: Server and client should be able to communicate over TCP/IP.

## Compilation

To compile the server and client programs, use the following commands:

```bash
gcc server.c -o server
gcc client.c -o client

./server PORT_NUMBER
./client LOCAL_ADDRESS PORT_NUMBER FILE_NAME
