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

//template <typename T>
//class ISubject {
//public:
//    virtual ~ISubject() = default;
//
//    // 注册观察者
//    virtual void addObserver(std::shared_ptr<Observer<T>> observer) = 0;
//
//    // 注销观察者
//    virtual void removeObserver(std::shared_ptr<Observer<T>> observer) = 0;
//
//    // 通知所有观察者
//    virtual void notifyAll(EventType eventType) = 0;
//};
//
//
//
////template <typename T>
////class LineAddedObserver : public Observer<T> {
////public:
////    void update(EventType eventType) override {
////        if (eventType == EventType::LineAdded) {
////            std::cout << "A line has been added." << std::endl;
////        }
////    }
////};
//
//template <typename T>
//class ObserverManager {
//public:
//    void addObserver(const std::shared_ptr<Observer<T>>& observer) {
//        observers.push_back(observer);
//    }
//
//    void removeObserver(const std::shared_ptr<Observer<T>>& observer) {
//        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
//    }
//
//    void notifyAll(EventType eventType) {
//        for (const auto& observer : observers) {
//            observer->update(eventType);
//        }
//    }
//
//private:
//    std::vector<std::shared_ptr<Observer<T>>> observers;
//};
//
//template <typename T>
//class ObserverBuilder {
//public:
//    ObserverBuilder() : observerManager(std::make_shared<ObserverManager<T>>()) {}
//
////    // 添加行数观察者
////    ObserverBuilder& withRowCountObserver() {
////        observerManager->addObserver(std::make_shared<RowCountObserver<T>>());
////        return *this;
////    }
////
////    // 添加列名观察者
////    ObserverBuilder& withColumnNameObserver() {
////        observerManager->addObserver(std::make_shared<ColumnNameObserver<T>>());
////        return *this;
////    }
////
////    // 添加数据插入观察者
////    ObserverBuilder& withDataInsertedObserver() {
////        observerManager->addObserver(std::make_shared<DataInsertedObserver<T>>());
////        return *this;
////    }
////
////    // 添加行删除观察者
////    ObserverBuilder& withLineDeletedObserver() {
////        observerManager->addObserver(std::make_shared<LineDeletedObserver<T>>());
////        return *this;
////    }
//
//    // 添加行添加观察者
//    ObserverBuilder& withLineAddedObserver() {
//        observerManager->addObserver(std::make_shared<LineAddedObserver<T>>());
//        return *this;
//    }
//
//    // 最终构建并返回 ObserverManager
//    std::shared_ptr<ObserverManager<T>> build() {
//        return observerManager;
//    }
//
//private:
//    std::shared_ptr<ObserverManager<T>> observerManager;
//};




#endif //TRADERBACKTEST_MOBSERVER_H
