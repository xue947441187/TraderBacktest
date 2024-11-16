//
// Created by 94744 on 2024/9/9.
//

#ifndef TRADERBACKTEST_COMPARABLEITEM_H
#define TRADERBACKTEST_COMPARABLEITEM_H

#include <iostream>
#include <memory>
#include <functional>
#include "item.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>



using namespace boost::multi_index;
template <typename _index, typename _value>
class ComparableItem {
private:

protected:
    boost::shared_ptr<ItemContainer<_index, _value>> _line;
    explicit ComparableItem(boost::shared_ptr<ItemContainer<_index, _value>> _line)
            : _line(_line){};
//    // 声明子类为友元类，允许访问私有构造函数
//    template <typename, typename> friend class IndexComparableItem;
//    template <typename, typename> friend class ValueComparableItem;

public:

//    static boost::shared_ptr<ComparableItem<_index, _value>> createComparableItem(
//            boost::shared_ptr<ItemContainer<_index, _value>> _line){
//        return boost::shared_ptr<ComparableItem<_index, _value>>(new ComparableItem<_index, _value>(_line));
//    };

//    virtual ItemContainer<_index, _value> query(const std::function<bool(const _seach&)>& condition) const = 0;
//    {
//        ItemContainer<_index, _value> result;
//        auto& index_view = this->_line->template get<index_tag>();
//        for (const auto& item : index_view) {
//            if (condition(item.index)) {
//                result.insert(item); // 插入整个 Item 对象
//            }
//        }
//        return result;
//    }

    // 将操作符重载声明为纯虚函数
//    virtual ItemContainer<_index, _value> operator>(const _seach& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator<(const _seach& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator==(const _seach& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator>=(const _seach& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator<=(const _seach& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator!=(const _seach& idx) const = 0;


//    // 输出操作符重载，用于打印值
//    friend std::ostream& operator<<(std::ostream& os, const ItemContainer<_index, _value>& items) {
//        os << "[";
//        bool first = true;
//        for (const auto& item : items) {
//            if (!first) {
//                os << ", ";
//            }
//            os << "Index: " << item.index << ", Value: " << item.value;
//            first = false;
//        }
//        os << "]";
//        return os;
//    }
//    void get_key() {
//        std::cout << "查看 _key 的类型: " << typeid(_seach).name() << std::endl;
//    }
};





//
//template <typename _index, typename _value>
//class ComparableItem {
//private:
//    using _key = _index;
//
//protected:
//    std::shared_ptr<ItemContainer<_index, _value>> _line;
//    explicit ComparableItem(std::shared_ptr<ItemContainer<_index, _value>> _line)
//    : _line(_line) {}
//
//public:
//    static std::shared_ptr<ComparableItem<_index, _value>> createComparableItem(
//            std::shared_ptr<ItemContainer<_index, _value>> _line) {
//        return std::make_shared<ComparableItem<_index, _value>>(_line);
//    }
//
//    virtual ItemContainer<_index, _value> query(const std::function<bool(const _key&)>& condition) const {
//        ItemContainer<_index, _value> result;
//        auto& index_view = this->_line->get<index_tag>();
//        for (const auto& item : index_view) {
//            if (condition(item.index)) {
//                result.insert(item);
//            }
//        }
//        return result;
//    }
////
////    ItemContainer<_index, _value> operator>(const _key& idx) const {
////        return query([&idx](const _index& itemIdx) {
////            return itemIdx > idx;
////        });
////    }
////
////    ItemContainer<_index, _value> operator<(const _key& idx) const {
////        return query([&idx](const _index& itemIdx) {
////            return itemIdx < idx;
////        });
////    }
////
////    ItemContainer<_index, _value> operator==(const _key& idx) const {
////        return query([&idx](const _index& itemIdx) {
////            return itemIdx == idx;
////        });
////    }
////
////    ItemContainer<_index, _value> operator>=(const _key& idx) const {
////        return query([&idx](const _index& itemIdx) {
////            return itemIdx >= idx;
////        });
////    }
////
////    ItemContainer<_index, _value> operator<=(const _key& idx) const {
////        return query([&idx](const _index& itemIdx) {
////            return itemIdx <= idx;
////        });
////    }
////
////    ItemContainer<_index, _value> operator!=(const _key& idx) const {
////        return query([&idx](const _index& itemIdx) {
////            return itemIdx != idx;
////        });
////    }
//    // 将操作符重载声明为纯虚函数
//    virtual ItemContainer<_index, _value> operator>(const _key& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator<(const _key& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator==(const _key& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator>=(const _key& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator<=(const _key& idx) const = 0;
//    virtual ItemContainer<_index, _value> operator!=(const _key& idx) const = 0;
//
//
//    friend std::ostream& operator<<(std::ostream& os, const ItemContainer<_index, _value>& items) {
//        os << "[";
//        bool first = true;
//        for (const auto& item : items) {
//            if (!first) {
//                os << ", ";
//            }
//            os << "Index: " << item.index << ", Value: " << item.value;
//            first = false;
//        }
//        os << "]";
//        return os;
//    }
//
//    void get_key() const {
//        std::cout << "查看 _key 的类型: " << typeid(_key).name() << std::endl;
//    }
//};












#endif //TRADERBACKTEST_COMPARABLEITEM_H
