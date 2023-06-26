#pragma once

#include <map>
#include <string>
#include <functional>

namespace ImForge::Core {

template <typename... Args> 
struct Callback {

    Callback(std::function<void(Args...)> function)
        : function(function) {};

    virtual inline void operator ()(Args... args){
        this->function(args...);
    }

protected: 
    std::function<void(Args...)> function;
};

template <size_t N, typename... Args>
struct NCallback : Callback<Args...> {
    NCallback(std::function<void(Args...)> function)
        : Callback<Args...>(function), times(0) {};
    
    inline void operator ()(Args... args){
        if(this->times++ > N) return;
        this->function(args...);
    }
private:
    size_t times; 
};

template <typename... Args>
struct NCallback<1, Args...> : Callback<Args...> {
    NCallback(std::function<void(Args...)> function)
        : Callback<Args...>(function), used(false) {};
    
    inline void operator ()(Args... args){
        if(this->used) return;
        this->function(args...);
        this->used = true;
    }
private:
    bool used;
};

template <typename... Args>
using OnceCallback = NCallback<1, Args...>;

template <typename... Args>
struct OnFirstCallback : Callback<Args...> {
    OnFirstCallback(
        std::function<void(Args...)> function,
        std::function<void(Args...)> on_first
    ) : Callback(function), on_first(on_first), used(false) {};
    
    virtual void operator ()(Args... args){
        if(!this->used){
            this->used = true;
            return this->on_first();
        }
        return this->function();
    }
private:
    std::function<void(Args...)> on_first;
    bool used; 
};

template <typename... Args>
struct Callbacks {
    inline void invoke(std::string id, Args... args) {
        if(!this->callbacks.contains(id) || !this->callbacks.at(id))
            return;
        this->callbacks.at(id)->operator()(args...);
    }

    inline void on(std::string id, Callback<Args...>* callback){
        this->callbacks.insert_or_assign(id, callback);
    }

private:
    std::map<std::string, Callback<Args...>*> callbacks;
};

}