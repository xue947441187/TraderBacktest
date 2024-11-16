//
// Created by 94744 on 2024/9/9.
//

#ifndef TRADERBACKTEST_ITEM_H
#define TRADERBACKTEST_ITEM_H

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>


template <typename _index, typename _value>
struct Item {
    _index index;
    _value value;
};

struct index_tag {};  // 标签用于索引
struct value_tag {};  // 标签用于值

template <typename _index, typename _value>
using ItemContainer = boost::multi_index::multi_index_container<
        Item<_index, _value>,
        boost::multi_index::indexed_by<
                boost::multi_index::ordered_non_unique<
                        boost::multi_index::tag<index_tag>,
                        boost::multi_index::member<Item<_index, _value>, _index, &Item<_index, _value>::index>
                >,
                boost::multi_index::ordered_non_unique<
                        boost::multi_index::tag<value_tag>,
                        boost::multi_index::member<Item<_index, _value>, _value, &Item<_index, _value>::value>
                >
        >
>;

#endif //TRADERBACKTEST_ITEM_H
