`vector<int>` represents a user defined type.

`int` represents a built-in type.

# List initialization
Does not allow narrowing.
Recommended by Stroustroup.
```c++
vector<int> v {1}; // initializer-list constructor
int i {1}; 	       // constructor (1)
```
# List initialization (C-style)
Does not allow narrowing.
```c++
vector<int> v = {1}; // initializer-list constructor
int i = {1};  // constructor (1)
```

# C-style initialization
Allows narrowing.
```c++
vector<int> v = 1; ??
int i = 1;    // constructor (1)
```

# Constructor initialization
```c++
vector<int> v(1);  // constructor vector(int)
int i(1);          // 1
```

# Initialization with type deduction
```c++
auto i = 1;        // type is deduced to int
```
# Default Initialization
```c++
vector<int> v {};  // default constructor
int i {};          // default value 0.
```
For built-in types the default value is a suitable representation of zero (`0`, `0.0`, `nullptr`, `'\0'`) 

# Missing Initialization
Objects in static memory are always fully initialized with default values.
Objects in stack or heap memory are left uninitialized if they are of a built-in type.

```c++
vector<int> v;     // default constructor
```
gcc will warn about uninitialized stack variables (`-Wuninitialized`)
```c++
int i;             // if static then initialized with default value, otherwise uninitialized
```

# Class object initialization
- memberwise initialization
- copy initialization
- default initialization (default constructor)
