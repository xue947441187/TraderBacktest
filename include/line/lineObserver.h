//
// Created by 94744 on 2024/9/13.
//

#ifndef TRADERBACKTEST_LINEOBSERVER_H
#define TRADERBACKTEST_LINEOBSERVER_H
#include "base/mObserver.h"
#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"
#include "line.h"

namespace Line {
    template <typename _index, typename _value>
    class Line;


    template <typename T>
    class LineAddedObserver : public Observer {
    public:
        explicit LineAddedObserver(const T &line)
                : line(line)
        {}
        void update(EventType eventType) override {
            if (eventType == EventType::LineAdded) {
                line->getTotalDataCount();
//                std::cout << line->getColumnName() << "\t当前总数:\t" << line->getRowCount()<< std::endl;
            }
        }
    private:
        T line;
    };

    template <typename T>
    class LineDataUpdatedObserver : public Observer {
    public:
        explicit LineDataUpdatedObserver(const T &line)
                : line(line)
        {}
        void update(EventType eventType) override {
            if (eventType == EventType::DataUpdated) {
                line->getTotalDataCount();
//                std::cout << "A line has been added." << "当前总数:" << line->getRowCount()<< std::endl;
            }
        }
    private:
        T line;
    };

    class LineObserverManager {
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
        ~LineObserverManager()= default;
    private:
        std::vector<boost::shared_ptr<Observer>> observers;
    };


    template <typename T>
    class LineObserverBuilder {
    public:
        explicit LineObserverBuilder(const T& line)
                : lineAddedObserver(boost::make_shared<LineObserverManager>()),
                  line(line) {}

        // 添加行添加观察者
        LineObserverBuilder& withLineAddedObserver() {
            // 确保 LineAddedObserver 定义正确
            lineAddedObserver->addObserver(boost::make_shared<LineAddedObserver<T>>(line));
            return *this;
        }
        // 添加行添加观察者
        LineObserverBuilder& withLineDataUpdatedObserver() {
            // 确保 LineAddedObserver 定义正确
            lineAddedObserver->addObserver(boost::make_shared<LineDataUpdatedObserver<T>>(line));
            return *this;
        }

        // 最终构建并返回 ObserverManager
        boost::shared_ptr<LineObserverManager> build() {
            return lineAddedObserver;
        }

    private:
        boost::shared_ptr<LineObserverManager> lineAddedObserver;
        T line;
    };
}


#endif //TRADERBACKTEST_LINEOBSERVER_H
