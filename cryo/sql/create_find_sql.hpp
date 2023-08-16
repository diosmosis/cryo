///////////////////////////////////////////////////////////////////////////////
//
//! \file create_find_sql.hpp
//! Contains the \ref create_find_sql and \ref create_find_autogenerate free functions.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_CREATE_FIND_SQL_HPP )
#define CRYO_SQL_CREATE_FIND_SQL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/detail/switch.hpp>
#include <cryo/sql/output_column_name.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <cryo/sql/output_column_value.hpp>
#include <cryo/sql/for_each.hpp>
#include <cryo/sql/enumerate.hpp>
#include <cryo/extension/concrete_hooks.hpp>
#include <cryo/schema/introspect/primary_key_column_of.hpp>
#include <cryo/schema/introspect/is_simple_entity.hpp>
#include <cryo/schema/introspect/table_name_of.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/schema/introspect/foreign_entity_type_of.hpp>
#include <cryo/schema/introspect/is_entity_reference.hpp>
#include <cryo/schema/introspect/is_column_descriptor.hpp>
#include <cryo/schema/introspect/columns_of.hpp>
#include <cryo/detail/selectable_columns_of.hpp>
#include <cryo/sql/dynamic/col.hpp>
#include <cryo/sql/dynamic/join_index_set.hpp>
#include <cryo/sql/dynamic/calc_join_indices.hpp>
#include <cryo/sql/dynamic/condition.hpp>
#include <cryo/sql/dynamic/output_sql.hpp>
#include <cryo/sql/dynamic/keyword_columns_of.hpp>
#include <cryo/sql/dynamic/group_by.hpp>
#include <cryo/sql/dynamic/output_join.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    namespace detail
    {
        template <typename T>
        struct is_column_terminal
            : boost::mpl::false_
        {};

        template <typename Impl>
        struct is_column_terminal<dynamic::keyword::column_terminal<Impl> >
            : boost::mpl::true_
        {};

        template <typename T>
        struct is_column_node
            : boost::mpl::false_
        {};

        template <typename Cols>
        struct is_column_node<dynamic::keyword::column_path<Cols> >
            : boost::mpl::true_
        {};

        template <typename Cols>
        struct is_column_node<dynamic::keyword::group_by<Cols> >
            : boost::mpl::true_
        {};

        template <typename C>
        struct is_group_by
            : boost::mpl::false_
        {};

        template <typename Cols>
        struct is_group_by<dynamic::keyword::group_by<Cols> >
            : boost::mpl::true_
        {};

        template <typename C>
        struct is_group_by<C const>
            : is_group_by<C>
        {};

        template <typename Columns>
        struct has_group_by
            : boost::mpl::not_<
                typename boost::is_same<
                    typename boost::mpl::find_if<
                        Columns,
                        is_group_by<boost::remove_reference<boost::mpl::_1> >
                    >::type,
                    typename boost::mpl::end<Columns>::type
                >::type
            >::type
        {};

        template <typename Connection>
        struct output_table_column_ref
            : switch_<
                output_table_column_ref<Connection>,
                void (std::stringstream &),
                boost::mpl::vector<
                    is_entity_reference<boost::mpl::_1>
                >
            >
        {
            output_table_column_ref()
                : join_idx(0)
                , alias_count(0)
            {}

            // column references another entity
            template <typename C>
            void case_(std::stringstream & ss, C, boost::mpl::int_<0>) const
            {
                typedef typename foreign_entity_type_of<C>::type foreign_entity_type;

                unsigned int temp = join_idx;
                join_idx = ++alias_count;
                enumerate(columns_of<foreign_entity_type>(), *this, ss, ',');
                join_idx = temp;
            }

            // unwrapped column type is a primitive
            template <typename C>
            void default_(std::stringstream & ss, C c) const
            {
                ss << 't' << join_idx << '.';
                output_column_name<Connection>(ss, c);
            }

            mutable unsigned int join_idx;
            mutable unsigned int alias_count;
        };

        template <typename Connection>
        struct output_join
            : switch_<
                output_join<Connection>,
                void (std::stringstream &),
                boost::mpl::vector<
                    is_entity_reference<boost::mpl::_1>
                >
            >
        {
            output_join()
                : alias_count(1)
                , base_join(0)
                , in_left_join(false)
            {}

            // is an entity reference
            template <typename C>
            void case_(std::stringstream & ss, C c, boost::mpl::int_<0>) const
            {
                typedef typename C::field_type field_type;
                typedef typename foreign_entity_type_of<C>::type foreign_entity;

                // output the join
                sql::dynamic::output_join<foreign_entity>(
                    ss,
                    extension::concrete_hooks<Connection>(),
                    is_nullable<C>::value || in_left_join,
                    alias_count,
                    base_join,
                    c
                );

                // recurse
                unsigned int temp = base_join;
                base_join = alias_count;

                bool temp2 = in_left_join;
                in_left_join = in_left_join || is_nullable<C>::value;

                ++alias_count;
                for_each(columns_of<foreign_entity>(), *this, ss);

                base_join = temp;
                in_left_join = temp2;
            }

            // everything else
            template <typename C>
            void default_(std::stringstream & ss, C) const
            {
                // do nothing
            }

            mutable unsigned int alias_count;
            mutable unsigned int base_join;
            mutable bool in_left_join;
        };

        template <typename Connection>
        struct output_select_expr
            : switch_<
                output_select_expr<Connection>,
                void (std::stringstream &),
                boost::mpl::vector<
                    is_column_terminal<boost::mpl::_1>,
                    boost::mpl::and_<
                        is_column_node<boost::mpl::_1>,
                        is_entity_reference<boost::mpl::_1>
                    >,
                    is_column_node<boost::mpl::_1>,
                    boost::mpl::and_<
                        is_column_descriptor<boost::mpl::_1>,
                        is_entity_reference<boost::mpl::_1>
                    >,
                    is_column_descriptor<boost::mpl::_1>
                >
            >
        {
            output_select_expr(unsigned int join_start)
                : join_count(join_start)
                , join_index(0)
            {}

            // C is a column terminal
            template <typename C>
            void case_(std::stringstream & ss, C const& col, boost::mpl::int_<0>) const
            {
                // process the column node held by c
                (*this)(ss, boost::proto::value(col));
            }

            // the column is a column terminal and an entity reference
            template <typename C>
            void case_(std::stringstream & ss, C const& col, boost::mpl::int_<1>) const
            {
                typedef typename foreign_entity_type_of<C>::type foreign_entity_type;

                join_index = dynamic::detail::join_idx_of(col);
                enumerate(columns_of<foreign_entity_type>(), *this, ss, ',');
            }

            // the column is a column terminal that does not reference an entity
            template <typename C>
            void case_(std::stringstream & ss, C const& col, boost::mpl::int_<2>) const
            {
                dynamic::output_column_reference()(ss, extension::concrete_hooks<Connection>(), col);
            }

            // C is a column descriptor and an entity reference
            template <typename C>
            void case_(std::stringstream & ss, C, boost::mpl::int_<3>) const
            {
                typedef typename foreign_entity_type_of<C>::type foreign_entity_type;

                ++join_count;

                unsigned int temp = join_index;
                join_index = join_count;
                enumerate(columns_of<foreign_entity_type>(), *this, ss, ',');
                join_index = temp;
            }

            // C is a column descriptor and is not an entity reference
            template <typename C>
            void case_(std::stringstream & ss, C c, boost::mpl::int_<4>) const
            {
                ss << 't' << join_index << '.';
                output_column_name<Connection>(ss, c);
            }

            // E is a proto expression and not a column terminal
            template <typename E>
            void default_(std::stringstream & ss, E const& xpr) const
            {
                extension::concrete_hooks<Connection> hooks;
                dynamic::output_sql(ss, hooks, xpr);
            }

            mutable unsigned int join_count;
            mutable unsigned int join_index;
        };

        template <typename Connection>
        struct output_extra_joins
            : switch_<
                output_extra_joins<Connection>,
                void (std::stringstream &),
                boost::mpl::vector<
                    is_column_terminal<boost::mpl::_1>,
                    boost::mpl::and_<
                        is_column_node<boost::mpl::_1>,
                        is_entity_reference<boost::mpl::_1>
                    >,
                    boost::mpl::and_<
                        is_column_descriptor<boost::mpl::_1>,
                        is_entity_reference<boost::mpl::_1>
                    >
                >
            >
        {
            output_extra_joins(unsigned int join_start_)
                : join_start(join_start_)
                , join_index(join_start_)
                , base_join(0)
                , in_left_join(false)
            {}

            // C is a column terminal
            template <typename C>
            void case_(std::stringstream & ss, C const& col, boost::mpl::int_<0>) const
            {
                // process the column node held by col
                (*this)(ss, boost::proto::value(col));
            }

            // C is a column node and an entity reference
            template <typename C>
            void case_(std::stringstream & ss, C const& cn, boost::mpl::int_<1>) const
            {
                typedef typename foreign_entity_type_of<C>::type foreign_entity;

                base_join = cn.join_idx;
                in_left_join = is_nullable<C>::value;
                for_each(columns_of<foreign_entity>(), *this, ss);
            }

            // C is a column descriptor and entity reference
            template <typename C>
            void case_(std::stringstream & ss, C c, boost::mpl::int_<2>) const
            {
                typedef typename C::field_type field_type;
                typedef typename foreign_entity_type_of<C>::type foreign_entity;

                ++join_index;

                unsigned int previous_alias = (join_index - 1 == join_start) ? base_join : (join_index - 1);

                // output the join
                sql::dynamic::output_join<foreign_entity>(
                    ss,
                    extension::concrete_hooks<Connection>(),
                    is_nullable<C>::value || in_left_join,
                    join_index,
                    previous_alias,
                    c
                );

                // recurse
                bool temp = in_left_join;
                in_left_join = in_left_join || is_nullable<C>::value;

                for_each(columns_of<foreign_entity>(), *this, ss);

                in_left_join = temp;

                --join_index;
            }

            // ignore anything else
            template <typename T>
            void default_(std::stringstream &, T const&) const
            {
                // do nothing
            }

            unsigned int join_start;
            mutable unsigned int join_index;
            mutable unsigned int base_join;
            mutable bool in_left_join;
        };

        template <typename Connection>
        struct output_group_by_single
        {
            template <typename C>
            void operator()(std::stringstream & ss, C const& x) const
            {
                output_column_reference()(ss, extension::concrete_hooks<Connection>(), x);
            }
        };

        template <typename Connection, typename Columns>
        inline typename boost::enable_if<
            has_group_by<Columns>, void
        >::type output_group_by(std::stringstream & ss, Columns const& cols)
        {
            typedef is_group_by<boost::remove_reference<boost::mpl::_1> > is_group_by_pred;

            ss << " GROUP BY ";
            enumerate(
                boost::fusion::filter_if<is_group_by_pred>(cols),
                output_group_by_single<Connection>(),
                ss,
                ',');
        }

        template <typename Connection, typename Columns>
        inline typename boost::disable_if<
            has_group_by<Columns>, void
        >::type output_group_by(std::stringstream & ss, Columns const& cols)
        {
            // empty
        }

    }

#if defined(DOXYGEN_INVOKED)
    //! \brief Creates a \c SELECT SQL statement that selects all the data required to
    //!        fill an \c Entity instance of the row whose primary key equals \c key.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \tparam Entity the entity whose mapped table is to be selected from.
    //! \param key a value convertible to the primary key type of \c Entity.
    //! \returns a \c SELECT statement that selects the entity whose primary key equals \c key.
    template <typename Connection, typename Entity, typename PrimaryKey>
    inline std::string create_find_sql(PrimaryKey const& key);

    //! \brief Creates a \c SELECT SQL statement that selects data specified by \c cols
    //!        of the row whose primary key equals \c key.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \tparam Entity the entity whose mapped table is to be selected from.
    //! \param key a value convertible to the primary key type of \c Entity.
    //! \param cols a Boost.Fusion random access sequence of query expressions.
    //! \returns a \c SELECT statement that selects the data specified by \c cols of the
    //!          entity whose primary key equals \c key.
    template <typename Connection, typename Entity, typename PrimaryKey, typename Columns>
    inline std::string create_find_sql(PrimaryKey const& key, Columns const& cols);

    //! \brief Creates a \c SELECT SQL statement that selects all the data required to
    //!        fill an \c Entity instance, for each row that \c q evaluates to \c true.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \tparam Entity the entity whose mapped table is to be selected from.
    //! \param q the condition that determines which rows will be selected. If this is a null
    //!          condition, every entity will be selected.
    //! \returns a \c SELECT statement that selects the entities for whom \c q evaluates
    //!          to \c true.
    template <typename Connection, typename Entity>
    inline std::string create_find_sql(dynamic::condition const& q);

    //! \brief Creates a \c SELECT SQL statement that selects the data specified by \c cols of
    //!        the rows for whom \c q evaluates to true.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \tparam Entity the entity whose mapped table is to be selected from.
    //! \param q the condition that determines which rows will be selected. If this is a null
    //!          condition, every entity will be selected.
    //! \param cols a Boost.Fusion random access sequence of query expressions.
    //! \returns a \c SELECT statement that selects the data specified by \c cols of each
    //!          entity for whom \c q evaluates to \c true.
    template <typename Connection, typename Entity, typename Columns>
    inline std::string create_find_sql(dynamic::condition const& q, Columns const& cols);
#endif

    template <typename Connection, typename Entity, typename PrimaryKey>
    inline typename boost::enable_if<
        is_simple_entity<Entity>,
        std::string
    >::type create_find_sql(PrimaryKey const& key)
    {
        std::stringstream ss;
        ss << "SELECT * FROM ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " WHERE ";
        output_column_name<Connection>(ss, primary_key_column_of<Entity>());
        ss << '=';
        output_column_value<Connection>(ss, key);
        ss << ';';
        return ss.str();
    }

    template <typename Connection, typename Entity, typename PrimaryKey>
    inline typename boost::disable_if<
        is_simple_entity<Entity>,
        std::string
    >::type create_find_sql(PrimaryKey const& key)
    {
        std::stringstream ss;
        ss << "SELECT ";
        enumerate(selectable_columns_of<Entity>(), detail::output_table_column_ref<Connection>(), ss, ',');
        ss << " FROM ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " t0";
        for_each(columns_of<Entity>(), detail::output_join<Connection>(), ss);
        ss << " WHERE t0.";
        output_column_name<Connection>(ss, primary_key_column_of<Entity>());
        ss << '=';
        output_column_value<Connection>(ss, key);
        ss << ';';
        return ss.str();
    }

    template <typename Connection, typename Entity, typename PrimaryKey, typename Columns>
    inline std::string create_find_sql(PrimaryKey const& key, Columns const& cols)
    {
        extension::concrete_hooks<Connection> hooks;

        // calculate join indices
        join_index_set indices;
        calc_join_indices(cols, indices);

        std::stringstream ss;
        ss << "SELECT ";
        enumerate(cols, detail::output_select_expr<Connection>(indices.size()), ss, ',');
        ss << " FROM ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " t0";

        // output joins
        for (join_index_set::const_iterator i = indices.begin(); i != indices.end(); ++i)
        {
            i->output_join(ss, hooks);
        }

        // output extra joins
        for_each(cols, detail::output_extra_joins<Connection>(indices.size()), ss);

        ss << " WHERE t0.";
        output_column_name<Connection>(ss, primary_key_column_of<Entity>());
        ss << '=';
        output_column_value<Connection>(ss, key);

        // output group by (if any are any group bys)
        detail::output_group_by<Connection>(ss, cols);

        ss << ';';
        return ss.str();
    }

    template <typename Connection, typename Entity>
    inline typename boost::enable_if<
        is_simple_entity<Entity>,
        std::string
    >::type create_find_sql(dynamic::condition const& q)
    {
        std::stringstream ss;
        ss << "SELECT *";
        if (q)
        {
            extension::concrete_hooks<Connection> hooks;
            q.output_boolean_aggregates(ss, hooks);

            ss << " FROM ";
            output_table_name<Connection>(ss, table_name_of<Entity>());
            ss << " t0 WHERE ";

            // NOTE: this works since each column's join_idx is initialized to 0 so table aliases will
            // all reference 'Entity'
            q.output_sql(ss, hooks);
        }
        else
        {
            ss << " FROM ";
            output_table_name<Connection>(ss, table_name_of<Entity>());
        }
        ss << ';';
        return ss.str();
    }

    template <typename Connection, typename Entity, typename Columns>
    inline std::string create_find_sql(dynamic::condition const& q, Columns const& cols)
    {
        extension::concrete_hooks<Connection> hooks;

        // calculate join indices
        dynamic::join_index_set indices;
        dynamic::calc_join_indices(cols, q, indices);

        std::stringstream ss;
        ss << "SELECT ";
        enumerate(cols, detail::output_select_expr<Connection>(indices.size()), ss, ',');

        // output extra select columns (for boolean aggregates)
        if (q)
        {
            q.output_boolean_aggregates(ss, hooks);
        }

        // output from
        ss << " FROM ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " t0";

        // output left joins
        for (join_index_set::const_iterator i = indices.begin(); i != indices.end(); ++i)
        {
            i->output_join(ss, hooks);
        }

        // output extra joins
        for_each(cols, detail::output_extra_joins<Connection>(indices.size()), ss);

        // output where clause
        if (q)
        {
            ss << " WHERE ";
            q.output_sql(ss, hooks);
        }

        // output group by (if any are any group bys)
        detail::output_group_by<Connection>(ss, cols);

        ss << ';';
        return ss.str();
    }

    template <typename Connection, typename Entity>
    inline typename boost::disable_if<
        is_simple_entity<Entity>,
        std::string
    >::type create_find_sql(dynamic::condition const& q)
    {
        return create_find_sql<Connection, Entity>(q, keyword_columns_of<Entity>());
    }

    // TODO: need test for this
    //! \brief Creates a \c SELECT SQL statement that selects every autogenerate column of
    //!        an entity.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \param x the entity whose autogenerate columns should be selected.
    //! \returns a \c SELECT statement that selects the autogenerate columns of an entity.
    template <typename Connection, typename Entity>
    inline std::string create_find_autogenerate_sql(Entity & x)
    {
        // filter out non-autogenerate columns
        typedef typename
            boost::mpl::remove_if<
                typename result_of::columns_of<Entity>::type,
                boost::mpl::not_<
                    is_autogenerate<boost::mpl::_1>
                >
            >::type
        filtered_columns;

        std::stringstream ss;
        ss << "SELECT ";
        enumerate(filtered_columns(), detail::output_table_column_ref<Connection>(), ss, ',');
        ss << " FROM ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " t0";
        for_each(columns_of<Entity>(), detail::output_join<Connection>(), ss);
        ss << " WHERE t0.";
        output_column_name<Connection>(ss, primary_key_column_of<Entity>());
        ss << '=';
        last_insert_id_hook<Connection>()(ss);
        ss << ';';
        return ss.str();
    }
}}

#endif // #if !defined( CRYO_SQL_CREATE_FIND_SQL_HPP )
