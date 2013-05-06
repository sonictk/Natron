//
//  singleton.h
//  PowiterOsX
//
//  Created by Alexandre on 2/15/13.
//  Copyright (c) 2013 Alexandre. All rights reserved.
//

#ifndef PowiterOsX_singleton_h
#define PowiterOsX_singleton_h
#include <cstdlib>
#include <QtCore/QMutex>

// Singleton pattern ( thread-safe) for the Controler class, to have 1 global ptr

template<class T> class Singleton {
    
protected:
    inline explicit Singleton() {
       // _lock = new QMutex;
        Singleton::instance_ = static_cast<T*>(this);
    }
    inline ~Singleton() {
        Singleton::instance_ = 0;
    }
public:
    friend class QMutex;
    
    static T* instance() ;
    static void Destroy();
    static QMutex* _lock;
    
    
private:
    static T* instance_;
    
    
    inline explicit Singleton(const Singleton&);
    inline Singleton& operator=(const Singleton&){return *this;};
    static T* CreateInstance();
    static void ScheduleForDestruction(void (*)());
    static void DestroyInstance(T*);
    
};

template<class T> T* Singleton<T>::instance() {
    if ( Singleton::instance_ == 0 ) {
        if(_lock){
            QMutexLocker guard(_lock);
        }
        if ( Singleton::instance_ == 0 ) {
            Singleton::instance_ = CreateInstance();
            ScheduleForDestruction(Singleton::Destroy);
        }
    }
    return (Singleton::instance_);
}
template<class T> void Singleton<T>::Destroy() {
    if ( Singleton::instance_ != 0 ) {
        DestroyInstance(Singleton::instance_);
        Singleton::instance_ = 0;
    }
}
template<class T> T* Singleton<T>::CreateInstance() {
    return new T();
}
template<class T> void Singleton<T>::ScheduleForDestruction(void (*pFun)()) {
    std::atexit(pFun);
}
template<typename T> void Singleton<T>::DestroyInstance(T* p) {
    delete p;
}

template<class T> T* Singleton<T>::instance_ = 0;
template<class T> QMutex* Singleton<T>::_lock = 0;


#endif
