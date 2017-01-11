`MyType` represents a user defined type.

`int` represents a built-in type.

# List initialization
```c++
MyType a {value};
int a {1}; 	  // constructor (1)
```
# List initialization (C-style)
```c++
MyType a = {value};
int a = {1};  // constructor (1)
```

# C-style initialization
```c++
MyType a = value;
int a = 1;    // constructor (1)
```

# Constructor initialization
```c++
MyType a(value);
int a
```

# Initialization with type deduction
```c++
auto a = value;
```
# Initialization with default values.
```c++
MyType a {};  // default constructor
MyType a;     // default constructor
int a {};     // default constructor (0)
int a;        // uninitialized
```
