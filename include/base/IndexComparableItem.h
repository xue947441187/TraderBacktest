//
// Created by 94744 on 2024/9/9.
//

#ifndef TRADERBACKTEST_INDEXCOMPARABLEITEM_H
#define TRADERBACKTEST_INDEXCOMPARABLEITEM_H
#include "ComparableItem.h"

template <typename _index, typename _value>
class IndexComparableItem : public ComparableItem<_index,_value>{
private:
    // 私有构造函数
    explicit IndexComparableItem(boost::shared_ptr<ItemContainer<_index, _value>> _line)
            : ComparableItem<_index, _value>(_line) {}
public:

//    using ComparableItem<_index, _value>::ComparableItem;

    // 查询
    static boost::shared_ptr<IndexComparableItem<_index, _value>> createComparableItem(
            boost::shared_ptr<ItemContainer<_index, _value>> _line) {
        // 调用父类的构造函数来创建IndexComparableItem实例
        return boost::shared_ptr<IndexComparableItem<_index, _value>>(new IndexComparableItem<_index,_value>(_line));
    }

    virtual ItemContainer<_index, _value> query(const std::function<bool(const _index&)>& condition) const {
        ItemContainer<_index, _value> result;
        auto& index_view = this->_line->template get<index_tag>();
        for (const auto& item : index_view) {
            if (condition(item.index)) {
                result.insert(item); // 插入整个 Item 对象
            }
        }
        return result;
    }
    // 重载操作符 >
    virtual ItemContainer<_index, _value> operator>(const _index& idx) const  {
        return query([&idx](const _index& itemIdx) {
            return itemIdx > idx;
        });
    }

    // 重载操作符 <
    virtual ItemContainer<_index, _value> operator<(const _index& idx) const {
        return query([&idx](const _index& itemIdx) {
            return itemIdx < idx;
        });
    }
    // 重载操作符 ==
    virtual ItemContainer<_index, _value> operator==(const _index& idx) const {
        return query([&idx](const _index& itemIdx) {
            return itemIdx == idx;
        });
    }
// 重载操作符 >=
    virtual ItemContainer<_index, _value> operator>=(const _index& idx) const  {
        return query([&idx](const _index& itemIdx) {
            return itemIdx >= idx;
        });
    }

// 重载操作符 <=
    virtual ItemContainer<_index, _value> operator<=(const _index& idx) const  {
        return query([&idx](const _index& itemIdx) {
            return itemIdx <= idx;
        });
    }

// 重载操作符 !=
    virtual ItemContainer<_index, _value> operator!=(const _index& idx) const {
        return query([&idx](const _index& itemIdx) {
            return itemIdx != idx;
        });
    }
};


#endif //TRADERBACKTEST_INDEXCOMPARABLEITEM_H
