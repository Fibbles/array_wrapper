#ifndef FIBB_ARRAY_WRAPPER
#define FIBB_ARRAY_WRAPPER

#include <iterator>
#include <type_traits>
#include <algorithm>
#include <stdexcept>
#include <array>

namespace fibb
{
    template <typename T, size_t N>
    class Array_Wrapper
    {
        public:
            /* TYPES */
            using value_type = T;
            using pointer = value_type*;
            using const_pointer = const pointer;
            using reference = value_type&;
            using const_reference = const reference;
            using size_type = size_t;
            using difference_type = ptrdiff_t;
            using iterator = pointer;
            using const_iterator = const_pointer;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            /* CONSTRUCTORS */
            Array_Wrapper(T (&array_)[N]) // sized array
                : m_array(array_)
            { }

            Array_Wrapper(T*& array_) // decayed array pointer
                : m_array(array_)
            { }

            /* COMPARISON */
            bool operator==(const Array_Wrapper& other) const { return m_array == other.m_array; }
            bool operator!=(const Array_Wrapper& other) const { return m_array != other.m_array; }

            /* FILL / SWAP */
            void fill(const value_type& val) { std::fill_n(begin(), N, val); }

            void swap(Array_Wrapper& other) noexcept(std::is_nothrow_swappable_v<value_type>)
            {
                if (m_array != other.m_array)
                {
                    std::swap_ranges(begin(), end(), other.begin());
                }
            }

            void swap(std::array<value_type, N>& other) noexcept(std::is_nothrow_swappable_v<value_type>)
            {
                if (m_array != other.data())
                {
                    // swap ranges expects 3 iterators of the same type
                    // iterators for std::array are implementation defined
                    // the data() method always returns a raw ptr
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
            constexpr bool empty() const noexcept { return N; }

            /* ELEMENT ACCESS */
            constexpr reference operator[](size_type pos) { return m_array[pos]; }
            constexpr const_reference operator[](size_type pos) const { return m_array[pos]; }

            constexpr reference at(size_type pos)
            {
                if (pos >= N) { throw std::out_of_range(std::string("Out of range: ") + std::to_string(pos)); }
                return m_array[pos];
            }

            constexpr const_reference at(size_type pos) const
            {
                if (pos >= N) { throw std::out_of_range(std::string("Out of range: ") + std::to_string(pos)); }
                return m_array[pos];
            }

            constexpr reference front() { return m_array[0]; }
            constexpr const_reference front() const { return m_array[0]; }
            constexpr reference back() { return m_array[N - 1]; }
            constexpr const_reference back() const { return m_array[N - 1]; }
            constexpr pointer data() noexcept { return m_array; }
            constexpr const_pointer data() const noexcept { return m_array; }

        private:
            pointer m_array;
    };
}

#endif // FIBB_ARRAY_WRAPPER

