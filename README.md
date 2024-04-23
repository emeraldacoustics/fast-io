# fast-io
A useful class to speed up input and output in C++

Instead of `cin` and `cout`, use `fin` and `fout` to input and output various data types including `int`, `float`, `double`, and `char *`.
- `fin >> n;`
- `fout << n;`

Don't use `fin` and `fout` inconsistently with `cin`, `cout`, `scanf()` or `printf()` because it can be a cause of potential errors.