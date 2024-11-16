//
// Created by 94744 on 2024/9/15.
//

#ifndef TRADERBACKTEST_LINEMANAGEROBSERVER_H
#define TRADERBACKTEST_LINEMANAGEROBSERVER_H
#include "base/mObserver.h"
#include "iostream"
#include "boost/shared_ptr.hpp"
#include "vector"
#include "boost/make_shared.hpp"
// Observer 类，用于观察数据的变化
namespace LineManager {
//
//template <typename T>
//class ManagerBaseObserver{
//public:
//    explicit ManagerBaseObserver(const T& lineManager)
//    :lineManager(lineManager)
//    {}
//protected:
//    T getLineManager() const{
//        return lineManager;
//    }

//private:
//    T lineManager;
//};
    enum class LineManagerEventType{


    };

    template <typename T>
    class ManagerRowCountChangedObserver: public Observer{
    public:
        explicit ManagerRowCountChangedObserver(const T& lineManager)
                :lineManager(lineManager)
        {}
        void update(EventType event_type) override{
            if (event_type == EventType::RowCountChanged){
                std::cout << "A line has been added." << "当前总数:" << lineManager->getRowCount()<< std::endl;
            }
        }
    private:
        T lineManager;
    };

    template <typename T>
    class ManagerColumnNameChangedObserver: public Observer{
    public:
        explicit ManagerColumnNameChangedObserver(const T& lineManager)
                :lineManager(lineManager)
        {}
        void update(EventType event_type) override{
            if (event_type == EventType::RowCountChanged){
                lineManager->getAllColumnName();
                std::cout << "Current column name." << std::endl;
            }

        }

    private:
        T lineManager;
    };

    class LineManagerObserverManager {
    public:
        void addObserver(const boost::shared_ptr<Observer>& observer) {
            observers.push_back(observer);
        }

        void removeObserver(const boost::shared_ptr<Observer>& observer) {
            observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        }

        void notifyAll(EventType eventType) {
            for (const auto& observer : observers) {
                observer->update(eventType);
            }
        }
        ~LineManagerObserverManager()= default;
    private:
        std::vector<boost::shared_ptr<Observer>> observers;
    };


    template <typename T>
    class LineManagerObserverBuilder {
    public:
        explicit LineManagerObserverBuilder(const T& lineManager)
                : lineAddedObserver(boost::make_shared<LineManagerObserverManager>()),
                  lineManager(lineManager) {}

        // 添加行添加观察者
        LineManagerObserverBuilder& withLineAddedObserver() {
            // 确保 LineAddedObserver 定义正确
            lineAddedObserver->addObserver(boost::make_shared<ManagerRowCountChangedObserver<T>>(lineManager));
            return *this;
        }
        LineManagerObserverBuilder& withManagerColumnNameChangedObserver(){
            lineAddedObserver->addObserver(boost::make_shared<ManagerColumnNameChangedObserver<T>>(lineManager));
            return *this;
        }
//    // 添加行添加观察者
//    LineObserverBuilder& withLineDataUpdatedObserver() {
//        // 确保 LineAddedObserver 定义正确
//        lineAddedObserver->addObserver(boost::make_shared<LineManagerObserverManager<T>>(line));
//        return *this;
//    }

        // 最终构建并返回 ObserverManager
        boost::shared_ptr<LineManagerObserverManager> build() {
            return lineAddedObserver;
        }

    private:
        boost::shared_ptr<LineManagerObserverManager> lineAddedObserver;
        T lineManager;
    };

}

#endif //TRADERBACKTEST_LINEMANAGEROBSERVER_H
