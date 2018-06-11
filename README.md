# Array_Wrapper
A non-owning wrapper for raw arrays providing a Standard Library container like interface.

## Why?
The motivation for creating this template is that while `std::array` solves a lot of problems asociated with raw arrays it can't always be used as a drop in replacment because the C++ specification gives no garauntee about the memory layout of `std::array`. Multi-dimensional arrays are a good example of this. You can't safely use `std::array<std::array<int, 2>, 2>` with a function that expects `int**`.

## Example
The interface provides all the same methods as `std::array`. The only thing that differs is the construction (detailed below) and that because the wrapper is non-owning, the original array must exist for the lifetime of the wrapper.

```
constexpr size_t SIZE = 5;

int arr[SIZE] = {1, 3, 5, 7, 9};
fibb::Array_Wrapper wrap1(arr);

int* decayed_arr = arr;
fibb::Array_Wrapper<int, SIZE> wrap2(decayed_arr);

assert(&arr[0] == wrap1.begin() && &arr[0] == wrap2.begin());
```
