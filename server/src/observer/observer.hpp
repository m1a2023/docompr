#pragma once
#include <string>
#include <list>

#include "../queue/RequestQueue.hpp"
#include "../processor/QueueProcessor.hpp"

namespace observer {

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update() = 0;
};

class ISubject {
public:
    virtual ~ISubject() = default;
    virtual void attach(IObserver *obs) = 0;
    virtual void detach(IObserver *obs) = 0;
    virtual void notify() = 0;
};

class Subject: public ISubject {
public:
    virtual ~Subject() override = default;
    void attach(IObserver *obs);
    void detach(IObserver *obs);
    void notify();

private:
    std::list<IObserver *> observers;
};

class QueueObserver: public IObserver {
public:
    explicit QueueObserver(requests::RequestQueue &queue);
    virtual ~QueueObserver() override = default;
    void update() override;

private: 
    requests::RequestQueue &_queue;
};

}