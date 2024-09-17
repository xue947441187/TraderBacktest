//
// Created by 94744 on 2024/9/9.
//

#ifndef TRADERBACKTEST_VALUECOMPARABLEITEM_H
#define TRADERBACKTEST_VALUECOMPARABLEITEM_H
#include "ComparableItem.h"


template <typename _index, typename _value>
class ValueComparableItem : public ComparableItem<_index,_value>{
private:
    // 私有构造函数
    explicit ValueComparableItem(boost::shared_ptr<ItemContainer<_index, _value>> _line)
            : ComparableItem<_index, _value>(_line) {}
public:

//    using ComparableItem<_index, _value>::ComparableItem;

    // 查询
    static boost::shared_ptr<ValueComparableItem<_index, _value>> createComparableItem(
            boost::shared_ptr<ItemContainer<_index, _value>> _line) {
        // 调用父类的构造函数来创建ValueComparableItem实例
        return boost::shared_ptr<ValueComparableItem<_index, _value>>(new ValueComparableItem<_index,_value>(_line));
    }

    virtual ItemContainer<_index, _value> query(const std::function<bool(const _value&)>& condition) const {
        ItemContainer<_index, _value> result;
        auto& index_view = this->_line->template get<index_tag>();
        for (const auto& item : index_view) {
            if (condition(item.value)) {
                result.insert(item); // 插入整个 Item 对象
            }
        }
        return result;
    }
    // 重载操作符 >
    virtual ItemContainer<_index, _value> operator>(const _value& idx) const  {
        return query([&idx](const _value& itemIdx) {
            return itemIdx > idx;
        });
    }

    // 重载操作符 <
    virtual ItemContainer<_index, _value> operator<(const _value& idx) const {
        return query([&idx](const _value& itemIdx) {
            return itemIdx < idx;
        });
    }
    // 重载操作符 ==
    virtual ItemContainer<_index, _value> operator==(const _value& idx) const {
        return query([&idx](const _value& itemIdx) {
            return itemIdx == idx;
        });
    }
// 重载操作符 >=
    virtual ItemContainer<_index, _value> operator>=(const _value& idx) const  {
        return query([&idx](const _value& itemIdx) {
            return itemIdx >= idx;
        });
    }

// 重载操作符 <=
    virtual ItemContainer<_index, _value> operator<=(const _value& idx) const  {
        return query([&idx](const _value& itemIdx) {
            return itemIdx <= idx;
        });
    }
// 重载操作符 !=
    virtual ItemContainer<_index, _value> operator!=(const _value& idx) const {
        return query([&idx](const _value& itemIdx) {
            return itemIdx != idx;
        });
    }
        void get_key() const {
        std::cout << "查看 _key 的类型: " << typeid(_value).name() << std::endl;
    }
};

#endif //TRADERBACKTEST_VALUECOMPARABLEITEM_H
