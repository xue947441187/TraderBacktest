//
// Created by 94744 on 2024/9/13.
//

#ifndef TRADERBACKTEST_MOBSERVER_H
#define TRADERBACKTEST_MOBSERVER_H


// Observer 类，用于观察数据的变化
enum class EventType {
    RowCountChanged,  // 行数变化
    ColumnNameChanged,  // 列名变化
    DataInserted,  // 数据插入
    DataDeleted,  // 数据删除
    LineDeleted,  // 行删除
    LineAdded,  // 行添加
    DataUpdated  // 数据更新
};

class Observer {
public:
    virtual void update(EventType event_type) = 0;  // 纯虚函数，子类实现
};





#endif //TRADERBACKTEST_MOBSERVER_H
