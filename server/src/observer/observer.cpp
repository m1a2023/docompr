#include "observer.hpp"

namespace observer {

void Subject::attach(IObserver *obs) {
    observers.push_back(obs);
}

void Subject::detach(IObserver *obs) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (obs == *it) {
            observers.erase(it);
        }
    }
}

void Subject::notify() { 
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        (*it)->update();
    }
}


} // end of observer