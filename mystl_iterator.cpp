#ifndef _MYSTL_ITERATOR_CPP
#define _MYSTL_ITERATOR_CPP

namespace mystd {
  using input_iterator_tag          = std::input_iterator_tag;
  using output_iterator_tag         = std::output_iterator_tag;
  using forward_iterator_tag        = std::forward_iterator_tag;
  using bidirectional_iterator_tag  = std::bidirectional_iterator_tag; 
  using random_access_iterator_tag  = std::random_access_iterator_tag; 

  template <typename _Iter>
  class _Iterator_traits_base {
    public:
    using iterator_category = typename _Iter::iterator_category;
    using value_type        = typename _Iter::value_type;
    using difference_type   = typename _Iter::difference_type;
    using pointer           = typename _Iter::pointer;
    using reference         = typename _Iter::reference;
  };

  template <typename _Tp>
  class _Iterator_traits_pointer_base {
    public: 
    using iterator_category = random_access_iterator_tag; 
    using value_type        = _Tp;
    using difference_type   = ptrdiff_t;
    using pointer           = _Tp*;
    using reference         = _Tp&;
  };

  template <typename _Tp>
  class iterator_traits;

  template <typename _Iter>
  class iterator_traits : public _Iterator_traits_base<_Iter> {};

  template <typename _Tp>
  class iterator_traits<_Tp*> : public _Iterator_traits_pointer_base<_Tp> {};

  template <typename _Iterator_category,
            typename _Tp,
            typename _Dif_type = ptrdiff_t,
            typename _Pointer  = _Tp*,
            typename _Ref      = _Tp&>
  class Iterator_help {
    public:
      using value_type        = _Tp;
      using iterator_category = _Iterator_category;
      using difference_type   = _Dif_type;
      using pointer           = _Pointer;
      using reference         = _Ref;
  };

  template <typename _It>
  class Iterator_show {
    public:
      static void show() {
        std::cout << "value_type = " << typeid(typename iterator_traits<_It>::value_type).name() << '\n';
        std::cout << "iterator_category = " << typeid(typename iterator_traits<_It>::iterator_category).name() << '\n';
        std::cout << "difference_type = " << typeid(typename iterator_traits<_It>::difference_type).name() << '\n';
        std::cout << "pointer = " << typeid(typename iterator_traits<_It>::pointer).name() << '\n';
        std::cout << "reference = " << typeid(typename iterator_traits<_It>::reference).name() << '\n';
      }
  };
}
#endif