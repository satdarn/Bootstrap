# Bootstrap Programming Language

Bootstrap is a statically-typed, compiled systems programming language designed for self-hosting capabilities. It provides the safety and expressiveness of modern languages while maintaining the simplicity needed for straightforward compiler implementation.

## Features

- **Strong Static Typing** with no implicit conversions
- **Explicit Memory Management** with ergonomic `new`/`delete` syntax
- **Method Syntax** that desugars to function calls
- **Type-Safe File Handling** with opaque `File` type
- **Array and Struct Literals** for convenient initialization
- **Self-Hosting Design** - compiler written in Bootstrap itself

## Quick Example

```bootstrap
struct DynArray {
    data: int*;
    length: int;
    capacity: int;
    
    func new(): DynArray {
        return {
            data: new int[4],
            length: 0,
            capacity: 4
        };
    }
    
    func push(self: DynArray*, value: int): void {
        if (self.length >= self.capacity) {
            self.resize(self.capacity * 2);
        }
        self.data[self.length] = value;
        self.length = self.length + 1;
    }
    
    func resize(self: DynArray*, new_cap: int): void {
        var new_data: int* = new int[new_cap];
        for (var i: int = 0; i < self.length; i = i + 1) {
            new_data[i] = self.data[i];
        }
        delete[] self.data;
        self.data = new_data;
        self.capacity = new_cap;
    }
}

func main(): int {
    var arr: DynArray = DynArray.new();
    arr.push(42);
    arr.push(100);
    
    printf("Array length: %d\n", arr.length);
    return 0;
}
```

## Installation

### Prerequisites

- **x86-64 Linux** (System V ABI)
- **NASM** assembler
- **GNU ld** linker
- **glibc** development headers

### Building from Source

```bash
git clone https://github.com/yourusername/bootstrap-compiler.git
cd bootstrap-compiler
make build
sudo make install
```

### Usage

```bash
# Compile a Bootstrap program
bootstrap source.bs -o output

# Run the compiled program
./output
```

## Language Overview

### Types

Bootstrap provides five core primitive types:

- `int` - 64-bit signed integer
- `char` - 8-bit character  
- `bool` - boolean (`true` or `false`)
- `void` - no return value
- `File` - opaque file handle

Plus composite types:
- **Pointers**: `int*`, `char*`, etc.
- **Static Arrays**: `int[10]`, `char[256]`
- **Structs**: User-defined aggregate types

### Variables and Initialization

```bootstrap
// Explicit initialization
var x: int = 42;
var name: char* = "hello";
var flag: bool = true;

// Implicit zero-initialization
var count: int;        // initialized to 0
var buffer: char[256]; // all elements initialized to 0
var ptr: int*;         // initialized to null

// Array and struct literals
var arr: int[3] = [1, 2, 3];
var point: Point = {x: 10, y: 20};
```

### Functions and Methods

```bootstrap
// Regular function
func add(a: int, b: int): int {
    return a + b;
}

// Method syntax (desugars to function calls)
struct Rectangle {
    width: int;
    height: int;
    
    func area(self: Rectangle*): int {
        return self.width * self.height;
    }
}

var rect: Rectangle = {width: 10, height: 5};
var area: int = rect.area(); // Desugars to Rectangle_area(&rect)
```

### Memory Management

```bootstrap
// Single allocation
var p: int* = new int;
*p = 42;
delete p;

// Array allocation
var arr: int* = new int[100];
arr[0] = 1;
delete[] arr;
```

### File I/O

```bootstrap
func read_file(path: char*): bool {
    var file: File = fopen(path, "r");
    if (file == null) {
        return false;
    }
    
    var buffer: char[1024];
    while (!feof(file)) {
        if (fread(buffer, 1, 1023, file) > 0) {
            printf("Read: %s\n", buffer);
        }
    }
    
    fclose(file);
    return true;
}
```

## Compiler Architecture

The Bootstrap compiler follows a traditional multi-pass design:

1. **Lexical Analysis** - Convert source text to tokens
2. **Syntax Analysis** - Build Abstract Syntax Tree (AST)  
3. **Desugaring Pass** - Transform method calls and memory operations
4. **Semantic Analysis** - Type checking and symbol resolution
5. **Code Generation** - Emit x86-64 assembly

## Design Philosophy

- **Type Safety**: Strong static typing prevents common errors
- **Explicit Memory Management**: Manual control with ergonomic syntax
- **Systematic Desugaring**: Complex syntax transforms to simple operations
- **Self-Hosting**: Language designed to compile itself
- **Implementation Simplicity**: Straightforward recursive descent parsing

## Documentation

- [Language Specification](docs/specification.pdf) - Complete language reference
- [Standard Library](docs/stdlib.md) - Built-in functions and types
- [Compiler Implementation](docs/compiler.md) - Internal architecture
- [Self-Hosting Guide](docs/self-hosting.md) - How to bootstrap the compiler


### Development Setup

```bash
git clone https://github.com/yourusername/bootstrap-compiler.git
cd bootstrap-compiler
make dev-setup
make test
```

## Project Status

Bootstrap is currently in **pre-alpha development**. The core language features are not implemented, and the self-hosting compiler is not in progress.

### Roadmap

- [ ] Core language implementation
- [ ] Basic standard library
- [ ] File I/O operations
- [ ] Memory management
- [ ] Self-hosting compiler (in progress)
- [ ] Standard library expansion
- [ ] Optimization passes
- [ ] Debug information generation


