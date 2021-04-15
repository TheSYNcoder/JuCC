# C++ Guidelines: Code Style

## Comments, Formatting, and Libraries

Please **comment** your code. Comment all the class definitions, non-trivial member functions and variables, and all the steps in your algorithms.
We generally follow the [Google C++ style guide](https://google.github.io/styleguide/cppguide.html). As they mention in that guide, these rules exist to keep the code base manageable while still allowing coders to use C++ language features productively.
Make sure that you follow the [naming rules](https://google.github.io/styleguide/cppguide.html#General_Naming_Rules). For instance, use `class UpperCaseCamelCase` for type names, `int lower_case_with_underscores` for variable/method/function names.

Please refrain from using any libraries other than the `STL` (and `googletest` for unit testing) without contacting us.

## Code Organization and Best Practice
We strive to write modern C++17 code, but C++ is a language with a lot of legacy features from the 80s. Certain guidelines must be followed to make the use of language features tasteful and elegant.

### `.h` and `.cpp` files

Surprisingly, there is no universal standards on what to call c++ code files. In this project, we will use `.h` for headers, inside the various `/include` directories, and `.cpp` for the implementation. 

When possible, implementation should be separated between `.h` and `.cpp` files, as this will make the compilation process much faster and hassle-free. Documentation should be written in the `.h` files. There are a couple of exceptions to this rule:
  - One-liners or otherwise boilerplate code that is unlikely to change. What constitutes a one-liner is somewhat ambiguous and can be subjective.
  - Templates. The c++ compiler generates instantiations of actual code based on the template given, and may need this information in the compilation units themselves, thus requiring all definition to be present in the included header. There are two solutions to this: either write all of the template code in the header or explicitly instantiate templates. Because doing the latter is painful, we generally will write those definitions in-place.

### Forward Declarations

When referring to some object only by reference, object or some template arguments, it is not necessary that the code knows its structure. As a result, we do not necessarily need to provide its complete declaration with members and methods (i.e. #include), but can get away with a hint that such a class exist. Example is given below:

```c++
class Foo;
...
void DoSomething(Foo *foo);  // compiles
void DoSomethingElse(Foo *foo) {
  foo->bar();  // error, member access into opaque type
}
...
```
Doing this saves re-compilation time. As a rule of thumb, forward declare when possible in the header, but always include the actual headers in the `.cpp` file.

### Concurrent Data Structures

There are many implementations of concurrent data structures online, of differing quality. Before you bring in any external implementation, ask yourself:
- Do I need a concurrent data structure?
- Is this data structure implementation the right choice for my use case and workload?
- Will someone in the future want to swap this out for a different implementation?

Concurrent data structures, especially lock-free ones, are not magic. They perform well only in the environment they are designed for. Our advice is to always start simple, and ensure correctness with latches and other simple mechanisms. Performance gain needs to be measured and verified on representative benchmarks, taken against multiple alternatives.

Finally, always prefer a wrapper whose underlying implementation can be swapped out with minimal effort to direct invocation of third-party code, when it comes to data structures.
