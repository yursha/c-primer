One pointer is said to alias another pointer when both refer to the same location.

In C99, it is illegal to create an alias of a different type than the original.
This is often refered to as the **strict aliasing rule**.
The rule is enabled by default in GCC at optimization levels at or above O2.

Dereferencing a cast of a variable from one type of pointer to a different type is _usually_ in violation of the strict aliasing rule.

Strict aliasing is an assumption, made by the C (or C++) compiler, that dereferencing pointers to objects of different types will never refer to the same memory location (i.e. alias each other.) 
