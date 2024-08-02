
# MyMake - Custom Make Utility

MyMake is a custom implementation of a Make-like utility, designed to manage and automate the build process for software projects.

## 📝 Description

MyMake reads a custom makefile format (`myMakefile` by default) and builds a dependency graph of targets and their associated commands. It then executes these commands in the correct order based on the dependencies and file modification times.

## 🚀 Features

- Custom makefile parsing
- Dependency graph creation and traversal
- Execution of build commands
- File modification time checking
- Support for "clean" target
- Pointer nodes for efficient graph representation

## 🛠️ Usage

```
./mymake [-f makefile] [target]
```

- `-f makefile`: Specify a custom makefile (default is `myMakefile`)
- `target`: Specify the target to build (default is the first target in the makefile)

## 📁 Project Structure

- `mymake.c`: Main program logic
- `graph.h`: Header file with data structures and function prototypes
- `graph_utils.c`: Utility functions for graph operations
- `graph_operations.c`: Core graph manipulation and traversal functions

## 🔧 Building the Project

To build the project, compile all the `.c` files together:

```
gcc mymake.c graph_utils.c graph_operations.c -o mymake
```

## 📄 Makefile Format

The custom makefile format is as follows:

```
target: dependency1 dependency2 ...
    command1
    command2
    ...

clean:
    clean_command
```

- Each target is defined on a new line, followed by its dependencies.
- Commands are indented with a tab and listed on separate lines.
- The `clean` target is special and can be used to clean up build artifacts.

## ⚠️ Error Handling

The program includes various error checks and will exit with an error message if it encounters issues such as:

- Invalid makefile format
- Missing files
- Command execution failures
- Circular dependencies

## 🤝 Contributing

Contributions, issues, and feature requests are welcome. Feel free to check the [issues page](https://github.com/AsemAlkhawar/mymake/issues) if you want to contribute.

## 👨‍💻 Author
**Asem Alkhawar**
- GitHub: [@AsemAlkhawar](https://github.com/AsemAlkhawar)
- LinkedIn: [Asem Alkhawar](https://www.linkedin.com/in/asem7)


