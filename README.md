# Fast I/O Library for C++
This library provides a highly optimized alternative to the standard I/O operations (`cin`, `cout`, `scanf()`, `printf()`) in C++. By significantly reducing the overhead associated with these operations, `fistream` and `fostream` classes can perform input and output operations up to 10 times faster, making it particularly useful for performance-critical applications such as competitive programming, large data processing, and more.

## Features
- Enhanced Performance: Achieve faster I/O by minimizing internal operation overhead.
- Ease of Use: Implements standard C++ stream operators, making it easy to integrate and replace existing code with minimal changes.
- Extended Compatibility: Supports all fundamental data types and offers precision control for floating-point numbers.

## Usage
`#include "fistream_x86/64.h"` in your C++ project and replace your `cin` and `cout`, with `fin` and `fout`.
```cpp
#include "fistream_x86.h"

int main()
{
    int a, b;
    fin >> a >> b; // Reading integers using fistream
    fout << "Sum is: " << (a + b) << fendl; // Writing output using fostream
    return 0;
}
```

### Input
```cpp
int x;
double y;
fin >> x >> y; // Read an integer and a double
```

### Output
```cpp
int num = 42;
fout << "Number: " << num << fendl;

long double pi = acosl(-1);
fout << "Pi: " << fsetprecision(20) << pi << fendl;
```

## Caution
Don't use `fin` and `fout` inconsistently with `cin`, `cout`, `scanf()` or `printf()` because it can be a cause of potential errors.

## Contributing
Contributions to improve this library are welcome. Please send pull requests or open an issue if you find bugs or have suggestions.

## License
This project is licensed under the MIT License - see the LICENSE file for details.