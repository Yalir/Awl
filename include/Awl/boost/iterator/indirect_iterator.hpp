// (C) Copyright David Abrahams 2002.
// (C) Copyright Jeremy Siek    2002.
// (C) Copyright Thomas Witt    2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_INDIRECT_ITERATOR_23022003THW_HPP
#define BOOST_INDIRECT_ITERATOR_23022003THW_HPP

#include <Awl/boost/iterator.hpp>
#include <Awl/boost/iterator/iterator_adaptor.hpp>

#include <Awl/boost/pointee.hpp>
#include <Awl/boost/indirect_reference.hpp>
#include <Awl/boost/detail/iterator.hpp>

#include <Awl/boost/detail/indirect_traits.hpp>

#include <Awl/boost/type_traits/is_same.hpp>
#include <Awl/boost/type_traits/add_reference.hpp>

#include <Awl/boost/mpl/bool.hpp>
#include <Awl/boost/mpl/identity.hpp>
#include <Awl/boost/mpl/eval_if.hpp>
#include <Awl/boost/mpl/not.hpp>
#include <Awl/boost/mpl/has_xxx.hpp>

#ifdef BOOST_MPL_CFG_NO_HAS_XXX
# include <Awl/boost/shared_ptr.hpp>
# include <Awl/boost/scoped_ptr.hpp>
# include <Awl/boost/mpl/bool.hpp>
# include <memory>
#endif 

#include <Awl/boost/iterator/detail/config_def.hpp> // must be last #include

namespace boost
{
  template <class Iter, class Value, class Category, class Reference, class Difference>
  class indirect_iterator;

  namespace detail
  {
    template <class Iter, class Value, class Category, class Reference, class Difference>
    struct indirect_base
    {
        typedef typename iterator_traits<Iter>::value_type dereferenceable;
        
        typedef iterator_adaptor<
            indirect_iterator<Iter, Value, Category, Reference, Difference>
          , Iter
          , typename ia_dflt_help<
                Value, pointee<dereferenceable>
            >::type
          , Category
          , typename ia_dflt_help<
                Reference
              , mpl::eval_if<
                    is_same<Value,use_default>
                  , indirect_reference<dereferenceable>
                  , add_reference<Value>
                >
            >::type
          , Difference
        > type;
    };

    template <>
    struct indirect_base<int, int, int, int, int> {};
  } // namespace detail

    
  template <
      class Iterator
    , class Value = use_default
    , class Category = use_default
    , class Reference = use_default
    , class Difference = use_default
  >
  class indirect_iterator
    : public detail::indirect_base<
        Iterator, Value, Category, Reference, Difference
      >::type
  {
      typedef typename detail::indirect_base<
          Iterator, Value, Category, Reference, Difference
      >::type super_t;

      friend class iterator_core_access;

   public:
      indirect_iterator() {}

      indirect_iterator(Iterator iter)
        : super_t(iter) {}

      template <
          class Iterator2, class Value2, class Category2
        , class Reference2, class Difference2
      >
      indirect_iterator(
          indirect_iterator<
               Iterator2, Value2, Category2, Reference2, Difference2
          > const& y
        , typename enable_if_convertible<Iterator2, Iterator>::type* = 0
      )
        : super_t(y.base())
      {}

  private:    
      typename super_t::reference dereference() const
      {
# if BOOST_WORKAROUND(__BORLANDC__, < 0x5A0 )
          return const_cast<super_t::reference>(**this->base());
# else
          return **this->base();
# endif 
      }
  };

  template <class Iter>
  inline
  indirect_iterator<Iter> make_indirect_iterator(Iter x)
  {
    return indirect_iterator<Iter>(x);
  }

  template <class Traits, class Iter>
  inline
  indirect_iterator<Iter,Traits> make_indirect_iterator(Iter x, Traits* = 0)
  {
    return indirect_iterator<Iter, Traits>(x);
  }

} // namespace boost

#include <Awl/boost/iterator/detail/config_undef.hpp>

#endif // BOOST_INDIRECT_ITERATOR_23022003THW_HPP
