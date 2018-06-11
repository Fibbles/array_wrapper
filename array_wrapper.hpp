#ifndef FIBB_ARRAY_WRAPPER
#define FIBB_ARRAY_WRAPPER

#include <iterator>
#include <type_traits>
#include <algorithm>
#include <stdexcept>
#include <array>

namespace fibb
{
    /* The Array_Wrapper is intended to wrap a raw C array so that it can be passed to a template
       expecting a std::array. All methods are designed to give the same functionality as those
       of std::array.

       The Array_Wrapper does not own the underlying array and as such the underlying array must
       exist for the lifetime of the Array_Wrapper.

       Although std::array supports a size of zero as a special case, zero sized raw arrays are illegal
       in C++, therefore passing a zero sized raw array to Array_Wrapper is undefined behaviour. */

    template <typename T, size_t N>
    class Array_Wrapper
    {
        public:
            /* TYPES */
            using value_type = T;
            using pointer = T*;
            using const_pointer = const T*;
            using reference = T&;
            using const_reference = const T&;
            using size_type = size_t;
            using difference_type = ptrdiff_t;
            using iterator = T*;
            using const_iterator = const T*;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            /* CONSTRUCTORS */
            Array_Wrapper(T (&array_)[N]) // sized array
                : m_array(array_)
            {
                static_assert(N > 0);
            }

            Array_Wrapper(T*& array_) // decayed array pointer, size must be known at compile time
                : m_array(array_)
            {
                static_assert(N > 0);
            }

            /* COMPARISON */
            constexpr bool operator==(const Array_Wrapper& other) const noexcept
            {
                return std::equal(begin(), end(), other.begin());
            }

            constexpr bool operator!=(const Array_Wrapper& other) const noexcept { return !(*this == other); }

            constexpr bool operator<(const Array_Wrapper& other) const noexcept
            {
                return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
            }

            constexpr bool operator>(const Array_Wrapper& other) const noexcept
            {
                auto comp = [] (const_reference a, const_reference b) -> bool { return a > b; };
                return std::lexicographical_compare(begin(), end(), other.begin(), other.end(), comp);
            }

            constexpr bool operator<=(const Array_Wrapper& other) const noexcept
            {
                auto comp = [] (const_reference a, const_reference b) -> bool { return a <= b; };
                return std::lexicographical_compare(begin(), end(), other.begin(), other.end(), comp);
            }

            constexpr bool operator>=(const Array_Wrapper& other) const noexcept
            {
                auto comp = [] (const_reference a, const_reference b) -> bool { return a >= b; };
                return std::lexicographical_compare(begin(), end(), other.begin(), other.end(), comp);
            }

            /* ASSIGNMENT */
            // copy assignment will copy elements into the underlying array
            constexpr Array_Wrapper& operator=(const Array_Wrapper& other)
                noexcept(std::is_nothrow_copy_assignable_v<value_type>)
            {
                if (*this != other)
                {
                    std::copy(other.begin(), other.end(), begin());
                }
                return *this;
            }

            // copy assignment will copy elements into the underlying array
            constexpr Array_Wrapper& operator=(const std::array<value_type, N>& other)
                noexcept(std::is_nothrow_copy_assignable_v<value_type>)
            {
                if (m_array != other.data())
                {
                    // Iterators for std::array are implementation defined
                    // The data() method always returns a raw ptr
                    std::copy(other.data(), other.data() + other.size(), begin());
                }
                return *this;
            }

            // move assignment will move elements into the underlying array
            constexpr Array_Wrapper& operator=(Array_Wrapper&& other)
                noexcept(std::is_nothrow_move_assignable_v<value_type>)
            {
                if (*this != other)
                {
                    std::move(other.begin(), other.end(), begin());
                }
                return *this;
            }

            // move assignment will move elements into the underlying array
            constexpr Array_Wrapper& operator=(std::array<value_type, N>&& other)
                noexcept(std::is_nothrow_move_assignable_v<value_type>)
            {
                if (m_array != other.data())
                {
                    // Iterators for std::array are implementation defined
                    // The data() method always returns a raw ptr
                    std::move(other.data(), other.data() + other.size(), begin());
                }
                return *this;
            }

            void fill(const_reference val) { std::fill_n(begin(), N, val); }

            // Note that swap will not switch the internal pointers
            // Array_Wrapper will behave as if it were std::array so swap actually swaps elements of the internal array
            void swap(Array_Wrapper& other) noexcept(std::is_nothrow_swappable_v<value_type>)
            {
                if (*this != other)
                {
                    std::swap_ranges(begin(), end(), other.begin());
                }
            }

            void swap(std::array<value_type, N>& other) noexcept(std::is_nothrow_swappable_v<value_type>)
            {
                if (m_array != other.data())
                {
                    // Swap ranges expects 3 iterators of the same type
                    // Iterators for std::array are implementation defined
                    // The data() method always returns a raw ptr
                    std::swap_ranges(begin(), end(), other.data());
                }
            }

            /* ITERATORS */
            constexpr iterator begin() noexcept { return m_array; }
            constexpr const_iterator begin() const noexcept { return m_array; }
            constexpr const_iterator cbegin() const noexcept { return m_array; }

            constexpr iterator end() noexcept { return m_array + N; }
            constexpr const_iterator end() const noexcept { return m_array + N; }
            constexpr const_iterator cend() const noexcept { return m_array + N; }

            constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
            constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
            constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

            constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
            constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
            constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

            /* CAPACITY */
            constexpr size_type size() const noexcept { return N; }
            constexpr size_type max_size() const noexcept { return N; }
            constexpr bool empty() const noexcept { return N == 0; }

            /* ELEMENT ACCESS */
            constexpr reference operator[](size_type pos) noexcept { return m_array[pos]; }
            constexpr const_reference operator[](size_type pos) const noexcept { return m_array[pos]; }

            constexpr reference at(size_type pos)
            {
                const Array_Wrapper& const_this = *this;
                return const_cast<reference>(const_this.at(pos));
            }

            constexpr const_reference at(size_type pos) const
            {
                static_assert(std::is_unsigned_v<size_type>);

                if (pos >= N) { raise_range_error(pos); }

                return m_array[pos];
            }

            constexpr reference front() noexcept { return m_array[0]; }
            constexpr const_reference front() const noexcept { return m_array[0]; }
            constexpr reference back() noexcept { return m_array[N - 1]; }
            constexpr const_reference back() const noexcept { return m_array[N - 1]; }
            constexpr pointer data() noexcept { return m_array; }
            constexpr const_pointer data() const noexcept { return m_array; }

        private:
            pointer m_array;

            // ideally this would go into a separate cpp file to prevent inlining
            constexpr void raise_range_error(size_type pos) const
            {
                throw std::out_of_range(std::string("Out of range: ") + std::to_string(pos));
            }
    };
}

#endif // FIBB_ARRAY_WRAPPER

