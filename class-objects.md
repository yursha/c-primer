# Common actions on class objects
- create (allocate and initialize memory)
  + when variable of object type is declared
  + when `new` operator is called
- copy
  + when passed as function arguments
  + when assigned with a `=` operator (copy assignment operator is called)
  + when copy constructor is called
- move
  + when returned from a function
  + when assigned a temporary object it is moved from.
- delete/cleanup (release resources and deallocate memory - destructor job)
  + when a stack frame containing an objects goes out of scope
  + when a `delete` is called on a heap object

```c++
class X {
  X(SomeType);             // conversion constructor
  X(SomeType1, SomeType2); // ordinary constructor
  X(const X&);             // copy constructor
  X(X&&);                  // move constructor
  X& operator=(const X&);  // copy assignment operator
  X& operator=(X&&);       // move assignment operator
  ~X();                    // destructor
}
```
