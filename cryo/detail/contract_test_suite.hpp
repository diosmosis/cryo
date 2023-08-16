///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_DETAIL_CONTRACT_TEST_SUITE_HPP )
#define CRYO_DETAIL_CONTRACT_TEST_SUITE_HPP

#include <boost/test/unit_test.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/for_each.hpp>

template <typename T>
struct concrete_test_suite;

template <typename C, typename T, typename F>
struct contract_test_suite;

namespace cryo { namespace detail
{
    template <typename T>
    struct identity_fixture
    {
        T & concrete_type() { return instance; }

        T instance;
    };

    template <typename T>
    struct test_case_runner
    {
        typedef void (__cdecl *test_case_fptr)(T *);
        typedef void result_type;

        test_case_fptr f;

        test_case_runner(test_case_fptr f_)
            : f(f_)
        {}

        void operator()() const
        {
            T instance;
            f(&instance);
        }
    };

    template <typename T, typename N>
    struct contract_test_case_runner
    {
        typedef typename T::contracts_vector contracts_vector;
        typedef typename boost::mpl::at<contracts_vector, N>::type contract_type;

        typedef void (__cdecl *test_case_fptr)(contract_type *);
        typedef void result_type;

        test_case_fptr f;

        contract_test_case_runner(test_case_fptr f_)
            : f(f_)
        {}

        void operator()() const
        {
            T instance;
            f(&boost::fusion::at<N>(instance.contracts));
        }
    };

    template <typename T>
    static int & get_test_case_count()
    {
        static int count = 0;
        return count;
    }

    template <typename T>
    struct test_case_counter
    {
        test_case_counter()
        {
            ++get_test_case_count<T>();
        }
    };

    template <typename F, typename T>
    inline void register_test_cases(T & instance)
    {
        using namespace boost::unit_test;

        typedef typename F::test_case_fptr test_case_fptr;
        typedef char const* (*test_case_name_getter)(void *);

        // get test case count
        int test_case_count = get_test_case_count<T>();

        // get the vtable
        union
        {
            test_case_fptr * testcases;
            test_case_name_getter * namegetters;
        } vtable;

        vtable.testcases = *static_cast<test_case_fptr **>(static_cast<void *>(&instance));

        // add individual test cases
        for (int i = 0; i != test_case_count * 2; i += 2)
        {
            char const* fname = vtable.namegetters[i](&instance);

            framework::master_test_suite().add(
                make_test_case(callback0<>(F(vtable.testcases[i + 1])), fname));
        }
    }

    template <typename T>
    struct test_suite_registrar
    {
        struct register_impl
        {
            register_impl(T & instance_)
                : instance(instance_)
            {}

            template <typename N>
            void operator()(N) const
            {
                register_test_cases<contract_test_case_runner<T, N> >(boost::fusion::at<N>(instance.contracts));
            }

            T & instance;
        };

        test_suite_registrar()
        {
            using namespace boost::unit_test;

            static bool ran = false;

            if (!ran)
            {
                ran = true;

                // TODO: This creates the data to be tested once more than is actually necessary.
                // Since the instance will be created/destroyed per test case, perhaps one more isn't that 
                // big a deal...
                T instance;

                register_test_cases<test_case_runner<T> >(instance);
                boost::fusion::for_each(
                    boost::mpl::range_c<int, 0, T::contract_count>(), register_impl(instance));
            }
        }

        static test_suite_registrar<T> instance;
    };

    template <typename D, typename T, typename F>
    struct contract_suite_base
    {
        typedef contract_test_suite<D, T, F> this_type;

        T * instance;

        contract_suite_base() {}

        T * operator -> () { return instance; }
        T & operator * () { return *instance; }
    };

    template <typename T, typename F, typename Contracts>
    struct concrete_suite_base : F
    {
        template <typename C>
        struct suite_of
            : boost::mpl::identity<contract_test_suite<C, T, F> >
        {};

        typedef F fixture_type;
        typedef concrete_test_suite<T> this_type;

        typedef typename boost::mpl::transform<Contracts, suite_of<boost::mpl::_1> >::type contracts_vector;

        struct set_instance
        {
            set_instance(concrete_suite_base & self_)
                : self(self_)
            {}

            template <typename C>
            void operator()(C & c) const
            {
                static_cast<fixture_type &>(self).concrete_type();
                c.instance = &static_cast<fixture_type &>(self).concrete_type();
            }

            concrete_suite_base & self;
        };

        enum { contract_count = boost::mpl::size<Contracts>::value };

        contracts_vector contracts;

        concrete_suite_base()
        {
            boost::fusion::for_each(contracts, set_instance(*this));
        }
    };
}}

#define CONTRACT_TEST_SUITE( contract )                                                                               \
    template <typename T, typename F>                                                                                 \
    struct contract_test_suite<contract, T, F>                                                                        \
        : cryo::detail::contract_suite_base<contract, T, F>                                                           \
    /**/

#define CONTRACT_TEST_CASE( name )                                                                                    \
    cryo::detail::test_case_counter<this_type> _ ## name ## _counter;                                                 \
    virtual char const* __cdecl name ## _name() { return #name ; }                                                    \
    virtual void __cdecl name()                                                                                       \
    /**/

#define CONCRETE_TEST_SUITE( type, contracts )                                                                        \
    CONCRETE_TEST_SUITE( type, cryo::detail::identity_fixture<type>, contracts )                                      \
    /**/

#define CONCRETE_TEST_SUITE_FIXTURE( type, fixture, contracts )                                                       \
    template <>                                                                                                       \
    struct concrete_test_suite<type>                                                                                  \
        : cryo::detail::concrete_suite_base<                                                                          \
        type, fixture, boost::fusion::vector<contracts> >                                                             \
    /**/

#define CONCRETE_TEST_CASE( name )                                                                                    \
    cryo::detail::test_case_counter<this_type> _ ## name ## _counter;                                                 \
    virtual char const* __cdecl name ## _name() { return #name ; }                                                    \
    virtual void __cdecl name()                                                                                       \
    /**/

#define USE_CONCRETE_TEST_SUITE( name )                                                                               \
    cryo::detail::test_suite_registrar<concrete_test_suite<name> >                                                    \
        cryo::detail::test_suite_registrar<concrete_test_suite<name> >::instance;                                     \
    /**/

#endif // #if !defined( CRYO_DETAIL_CONTRACT_TEST_SUITE_HPP )
