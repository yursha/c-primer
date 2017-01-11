`MyType` represents a user defined type.

`int` represents a built-in type.

```c++
MyType a {value};
int a {1}; 	  // constructor (1)

MyType a = {value};
int a = {1};  // constructor (1)

MyType a = value;
int a = 1;    // constructor (1)

MyType a(value);
int a

auto a = value;
MyType a {};  // default constructor
MyType a;     // default constructor
int a {};     // default constructor (0)
int a;        // uninitialized
```
