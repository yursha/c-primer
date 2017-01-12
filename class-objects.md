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
- delete/cleanup
  + when a stack frame containing an objects goes out of scope
  + when a `delete` is called on a heap object
