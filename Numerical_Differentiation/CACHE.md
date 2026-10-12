# CACHE.md
# Numerical Computing (NC) — C++ Library Development Instructions

## 1. Purpose

This file contains permanent instructions for developing, extending, testing,
and maintaining the Numerical Computing (NC) C++ library.

Before starting any new task, the AI coding assistant MUST read this file
and understand the project conventions.

The objective is to avoid repeating common instructions in every prompt and
maintain consistency across all NC C++ assignments and library components.

These instructions apply to:
- Numerical Computing laboratory assignments
- Numerical methods and algorithms
- Numerical integration and differentiation
- Numerical error calculations
- Mathematical computations
- Graph generation
- Library development
- Testing and documentation

---

## 2. Mandatory Workflow

Before modifying the project, follow these steps.

1. Read this Numericl_Computing/Numerical_Differentiation/CACHE.md file completely.
2. Inspect the existing project directory structure.
3. Read the relevant source files, header files, and build configuration.
4. Understand the existing implementation before making changes.
5. Identify the new assignment's requirements.
6. Reuse existing library functions whenever appropriate.
7. Implement the requested functionality without unnecessarily changing
   unrelated code.
8. Generate the required numerical results.
9. Save generated graphs and output files in their designated directories.
10. Compile and test the C++ implementation.
11. Verify the output files and graph images.
12. Summarize the files created or modified and the tests performed.

IMPORTANT:
- Do not assume the project structure without inspecting it.
- Do not overwrite existing work without checking its contents.
- Do not create duplicate implementations of existing functionality.
- Do not modify unrelated files.
- If the existing project differs from these conventions, inspect it first
  and preserve its established structure unless a change is necessary.

---

## 3. Project Directory Structure

Use the following structure as the standard convention.

NC_CPP/
├── CACHE_CPP.md
├── Include/
│   ├── numerical_integration.hpp
│   ├── numerical_differentiation.hpp
│   └── ...
├── Src/
│   ├── numerical_integration.cpp
│   ├── numerical_differentiation.cpp
│   └── ...
├── Graph/
│   ├── integration_graph.png
│   ├── error_graph.png
│   └── ...
├── Output/
│   ├── integration_output.txt
│   ├── error_output.txt
│   └── ...
├── Tests/
│   └── ...
├── README.md
├── Makefile
└── ...

This is a reference structure, not a requirement to recreate the entire
project for every assignment.

If the existing repository uses different directory names or a different
build system, follow the existing repository structure unless the task
explicitly requests otherwise.

### Directory Responsibilities

#### Include/

Store C++ header files with the `.hpp` extension.

Header files should contain:
- Function declarations
- Class declarations
- Required type definitions
- Necessary standard-library includes
- Documentation for public APIs

Keep implementation details in source files when appropriate.

#### Src/

Store C++ implementation files with the `.cpp` extension.

Source files should contain:
- Function definitions
- Numerical algorithm implementations
- Mathematical calculations
- Supporting implementation logic

Do not place the entire implementation in a header file unless the
implementation genuinely requires it, such as a template.

#### Graph/

Store generated graph images in PNG format.

Examples:
- Numerical integration graphs
- Absolute error versus number of subdivisions
- Comparison of numerical methods
- Convergence graphs
- Error analysis graphs

Use meaningful filenames related to the experiment.

#### Output/

Store numerical results and execution output in plain-text `.txt` files.

Examples:
- integration_output.txt
- differentiation_output.txt
- error_analysis.txt
- numerical_results.txt

Use a separate output file for each independent experiment when appropriate.

#### Tests/

Store automated tests or verification programs when required by the project.

Do not introduce an unnecessary testing framework if the existing project
uses a simpler testing approach.

---

## 4. C++ Coding Standards

Follow these conventions unless the repository already establishes a
different consistent style.

### 4.1 Language and File Types

- Use modern C++ supported by the project's compiler configuration.
- Use `.hpp` for header files.
- Use `.cpp` for implementation files.
- Follow the project's existing C++ standard.
- Do not change the compiler standard unnecessarily.

### 4.2 Header Files

Every public header should:
- Have an appropriate include guard or use `#pragma once`.
- Include the standard headers required by its declarations.
- Avoid unnecessary dependencies.
- Clearly expose the library's public interface.
- Use appropriate namespaces when required by the project.

Do not place `using namespace std;` in public header files.

### 4.3 Source Files

Source files should:
- Include their corresponding header when applicable.
- Keep implementation logic separate from declarations.
- Use meaningful variable and function names.
- Avoid unnecessary global variables.
- Handle invalid inputs appropriately.

### 4.4 Object-Oriented Programming

Use object-oriented programming when it improves the library's design.

Apply the following principles where appropriate:
- Encapsulation
- Abstraction
- Inheritance
- Polymorphism

Prefer simple, maintainable designs over unnecessary class hierarchies.

Do not force every numerical algorithm into a class if a function-based
implementation is more suitable.

Reuse existing classes and interfaces whenever possible.

### 4.5 Comments

Add useful comments explaining:
- The numerical method being implemented
- Important mathematical formulas
- Non-obvious algorithmic steps
- Assumptions and restrictions
- Important numerical limitations

Use Doxygen-style documentation for public functions and classes when
appropriate.

Avoid:
- Comments that merely repeat the code
- Excessive comments on obvious statements
- Incorrect or outdated explanations

Example:

/**
 * @brief Computes a numerical integral using the trapezoidal rule.
 *
 * @param a Lower integration limit.
 * @param b Upper integration limit.
 * @param n Number of subdivisions.
 * @return Approximation of the definite integral.
 */

---

## 5. Numerical Computing Requirements

For every numerical method, clearly identify:
- The mathematical problem
- The formula used
- The input parameters
- The numerical approximation
- Relevant error calculations
- The expected output

Use double precision for general floating-point numerical calculations
unless the assignment requires another type.

### 5.1 Numerical Integration

When implementing a numerical integration method:

1. Identify the integrand.
2. Identify the integration limits.
3. Identify the numerical method.
4. Define the number of subdivisions or step size.
5. Implement the correct numerical formula.
6. Calculate the approximation.
7. Compare it with the exact integral when available.
8. Calculate the absolute error when required.
9. Store the results in a text file.
10. Generate the requested graphs.

Do not assume an exact integral is available.

If the exact integral is unknown, use another appropriate error-estimation
method only when the assignment permits it.

### 5.2 Absolute Error

When both exact and numerical values are available, calculate:

Absolute Error = |Exact Value - Numerical Approximation|

For experiments involving different values of n:

- Keep the mathematical problem unchanged.
- Vary n according to the assignment.
- Calculate the approximation for each n.
- Calculate the absolute error for each n.
- Record the results in a table.
- Plot n against absolute error when requested.

Do not confuse the exact integral with the numerical approximation.

### 5.3 Validation

Where appropriate, validate the implementation using:
- Known analytical results
- Simple test functions
- Boundary cases
- Small and large subdivision counts
- Comparisons with another numerical method

Avoid silently accepting invalid values such as n <= 0 when n must be
positive.

---

## 6. Graph Generation Requirements

Save all generated graph images in the Graph/ directory using PNG format.

Requirements:

1. Use meaningful filenames.
2. Include an appropriate graph title.
3. Label the x-axis.
4. Label the y-axis.
5. Include a legend when multiple data series are plotted.
6. Use readable tick labels.
7. Include units when applicable.
8. Ensure the graph represents the actual computed data.
9. Do not fabricate or manually invent numerical results.
10. Save the graph to disk rather than only displaying it interactively.

Examples:

Graph/error_vs_n.png
Graph/integration_comparison.png
Graph/convergence_analysis.png

For an absolute-error graph:
- X-axis: Number of subdivisions (n)
- Y-axis: Absolute error
- Plot the computed error for every requested value of n.

If the task requires multiple graphs, save each graph separately unless
the assignment explicitly requests a combined figure.

### Graphing Implementation

Use the graphing approach already established in the project.

If an additional graphing library is necessary, explain the dependency
and follow the project's existing build or execution process.

Do not introduce a large external dependency for a simple graph without
first checking whether an existing solution is available.

---

## 7. Output File Requirements

Save requested numerical results in plain-text `.txt` files inside Output/.

The output file should be understandable without reading the source code.

Include relevant information such as:

- Experiment title
- Numerical method
- Function or mathematical expression
- Integration limits
- Number of subdivisions
- Exact value, when available
- Numerical approximation
- Absolute error
- Other requested measurements

For experiments involving different n values, use a clear table.

Example:

Numerical Integration Using the Trapezoidal Rule

Function: f(x) = x^2
Interval: [0, 1]

n        Approximation        Absolute Error
------------------------------------------------
4        ...                  ...
8        ...                  ...
16       ...                  ...
32       ...                  ...

Use actual computed values instead of placeholders in final output files.

Do not save output only in the terminal when the task requires a file.

Do not write the output file in an unrelated directory.

---

## 8. Compilation and Execution

Before declaring the task complete:

1. Check the project's build configuration.
2. Use the existing Makefile or build system when available.
3. Compile the changed source files.
4. Resolve compilation errors.
5. Execute the program.
6. Verify the numerical results.
7. Confirm the output file was generated.
8. Confirm the PNG graph files were generated.
9. Check that output files contain actual results.

Do not invent successful compilation or test results.

If compilation or execution cannot be completed because a dependency or
environment is unavailable, report the exact limitation.

Do not replace an existing build system unnecessarily.

---

## 9. Documentation

Update README.md when the new feature changes the library's public
functionality, build process, usage, or dependencies.

Documentation should explain:
- What the implementation does
- The numerical method used
- How to compile the program
- How to execute it
- Where results are stored
- Where graphs are stored

Do not rewrite the entire README for a small change unless necessary.

---

## 10. Dependency Management

Before introducing a new dependency:

1. Check whether the project already uses it.
2. Determine whether the task genuinely requires it.
3. Check whether an equivalent installed tool is available.
4. Follow the existing dependency-management approach.
5. Document any new dependency.

Do not silently change compiler flags, build configurations, or dependencies.

---

## 11. Task-Specific Instructions Have Priority

These are general project conventions.

The latest assignment prompt determines:
- The required mathematical problem
- The numerical method
- Input parameters
- Required algorithms
- Required output
- Required graphs
- Special file naming
- Any explicitly requested exceptions

If the assignment explicitly requests a different directory, file format,
algorithm, or implementation style, follow the assignment.

If the task conflicts with the existing repository design, inspect the
relevant files and explain the conflict before making a risky change.

Never invent requirements that are not in the assignment.

---

## 12. Final Response Format

After completing a task, provide a concise summary containing:

### Implementation
- What was implemented
- Which numerical method was used

### Files Created or Modified
- Header files
- Source files
- Graph files
- Output files
- Documentation or tests

### Verification
- Compilation status
- Execution status
- Numerical checks performed

### Results
- Important numerical results
- Location of generated graphs
- Location of text output

### Limitations
- Any unresolved issue
- Any dependency that could not be installed
- Any test that could not be performed

Do not claim a task is complete if important requirements remain unfinished.

---

## 13. Instructions for Future Tasks

For every new C++ NC assignment:

1. Read CACHE_CPP.md first.
2. Inspect the repository.
3. Read the new assignment prompt.
4. Follow these reusable project conventions.
5. Apply assignment-specific requirements.
6. Implement, execute, validate, and document the solution.
7. Report the actual changes and results.

The user should not need to repeat the directory structure, comment
requirements, graph-saving requirements, and output-file conventions
in every future prompt.