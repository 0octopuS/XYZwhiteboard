# Coding Note For Groupmate

## Coding style 

### Variable Naming

- Use descriptive and meaningful names.
- Variable names should be in lowercase, with words separated by underscores (snake_case).
- Avoid single-letter variable names unless they represent loop iterators (i, j, k, etc.).
- Class member variables should end with an underscore (_).

### File Naming

- Use lowercase letters for filenames.
- Separate words in filenames with underscores (snake_case).
- Header files should have a .h extension (example_header.h).
- Source files should have a .cc or .cpp extension (example_source.cc).
### Class and Struct Naming

- Use UpperCamelCase for class names (ClassName).
- Use UpperCamelCase for struct names (StructName).

### Namespace Naming

- Namespace names should be all lowercase, with words separated by underscores (snake_case).

### Function Naming

- Use descriptive names that convey the function's purpose.
- Function names should be in lowercase, with words separated by underscores (snake_case).

### Constant Naming

- Constants should be named in all uppercase letters, with words separated by underscores (SNAKE_CASE).
- Prefix constants with a k, e.g., const int kMaxValue = 100;.

### Indentation and Formatting

- Use 2 spaces for indentation.
- Use Allman style braces, where the opening brace is on the same line as the statement.

### Comments

- Use // for single-line comments and /* */ for multi-line comments.
- Write descriptive comments for non-obvious code sections.
- Comments should be written in English.

### Include Guards

- Use include guards to prevent multiple inclusion of header files.
- Use #pragma once if supported by your compiler for simplicity.

### Other Guidelines

- Follow the Google C++ Style Guide for additional coding conventions, such as type usage, function parameter ordering, and error handling.
- Communicating these guidelines to your collaborators will help maintain a consistent coding style throughout the project and improve code readability and maintainability.



## Git commit standard

Each commit message should be concise and descriptive, summarizing the changes made in the commit. Additionally, it's helpful to include a brief description of the changes in the commit body, if necessary.

For example, commit with "feat: adding functionality of adding a new shape in the whiteboard", "docs: change readme file, adding docker compile commands".

Some commonly used commit types in commit messages include:

- feat: Use when adding a new feature or functionality.
- fix: Use when fixing a bug or resolving an issue.
- docs: Use when updating documentation, such as README files or comments.
- style: Use when making cosmetic changes, such as formatting or whitespace adjustments.
- refactor: Use when refactoring code without changing its external behavior.
- test: Use when adding or modifying tests.
- chore: Use when performing maintenance tasks or other non-functional changes.