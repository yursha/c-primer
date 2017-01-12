# Primitive value types
## Integral
- `bool`
- `char`, `wchar_t`, `char16_t`, `char32_t`
- `short`, `int`, `long`, `long long`
- `enum`, `enum class`

Integral types can be `signed` or `unsigned`.

## Floating-point
- `float`, `double`, `long double`

- `void`

# Primitive address types
- `int*` - pointers
- `char[]` - arrays
- `double&`, `vector<int>&&` - references

# Complex types
- `struct`, `union`, `class`

`union` type has specialized use cases:
- Saving memory. By storing objects with not overlapping value lifetimes at the same location.
- Type punning (reinterpretation). I.e. storing value as one type and then reading it as another type.
