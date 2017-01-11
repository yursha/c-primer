`vector<int>` represents a user defined type.

`int` represents a built-in type.

# List initialization
Does not allow narrowing.
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
```c++
vector<int> v = 1; ??
int i = 1;    // constructor (1)
```

# Constructor initialization
```c++
vector<int> v(1); // constructor vector(int)
int i;
```

# Initialization with type deduction
```c++
auto i = 1; // type is deduced to int
```
# Initialization with default values.
```c++
vector<int> v {};  // default constructor
vector<int> v;     // default constructor
int i {};     // default value (0)
int i;        // uninitialized
```
