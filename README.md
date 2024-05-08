## Prerequisite:
- gcc (g++)
- Make
- [CMake](https://cmake.org/download/)
# Project tlang interpreter
tlang is an interpreted language for a datastructure and algorithm assignment
## Installation
```bash
git clone https://github.com/TranThangBin/tlang.git
mkdir build
cd build
cmake ..
make
```
```
Usage: ./tlang <filename>
```
## Syntax
### Variable declararation
```tlang
var foo = 10;
```
Variable is immutable by default so to make it mutable use the `mut` keyword
```tlang
var mut foo = 10;
```
Also `;` is required at the end of any statement
### Variable assignment
```tlang
foo = 20;
```
### Binary expression
```tlang
foo + 10 * 20 - (3 / 5);
```
You could also do a binary expression before assignment with a binary operator before `=`
```tlang
foo += 3.4;
```
### If statement
```tlang
if (foo != 10 && foo <= 20 || foo > bar) {
    println(foo);
}
```
### For loop
```tlang
for (var mut i = 0; i < 10; i += 1) {
    print(i, " ");
}
```
Since there are no while loop yet you could do the classic purging two initializer and modifier statement
```tlang
for (;foo < 10;) {
    print(foo, ' ');
    foo += 1;
}
```
### Function declararation
```tlang
fun foo(a, b) {
    return a + b;
}
```
The function implicitly return `null` if there are no return value specified
### Function call
```tlang
foo(5, -9);
```
> [!NOTE]
> The language is obviously not production ready
> No support for recursive function, cross function call, higher order function or closure due to memory management choice (unique_ptr)
> No support for circular reference due to reference counting (shared_ptr)
