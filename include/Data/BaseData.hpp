#pragma once

// Internal includes
#include "Subscribe/Subscribe.hpp"
#include "Storage/Storage.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes
#include <iostream>

namespace Data {

/**
 * @brief Abstract, non-templated interface of BaseData
 */
class BaseDataGeneric{
public:
    BaseDataGeneric(const char *name) : name(name) {}
    /**
     * @brief Print the BaseData
     */
    virtual void print(uint32_t indent_depth = 0) = 0;

    /**
     * @brief Print the BaseData everytime it notifies its subscribers
     */
    virtual void log_on_sub(bool set = true) = 0;

    /**
     * @brief Stop subscribers from being notified of changes
     */
    virtual void mute_sub() = 0;

    /**
     * @brief If mute has been previously called, then subscribers will now be notified
     */
    virtual void unmute_sub() = 0;

    /**
     * @brief Reset the object's internal value to its default state, maintaining its stored value
     *
     */
    virtual void set_default(void) = 0;

    /**
     * @brief Reset the object's internal value and stored value to its default state
     *
     */
    virtual void reset(void) = 0;

    /**
     * @brief Try to load a value from an external source and if the load fails
     * then reset the value
     *
     * @param value Reference to the value being loaded / reset
     */
    virtual void load_or_reset(void) = 0;
protected:
    const char *name;
};

/**
 * @brief Class to hold a value allowing it to be retrieved, saved,
 * and have objects subscribed to its changes
 *
 * @tparam T Type of value to hold
 */
template <typename T>
class BaseData : public BaseDataGeneric
{
public:
    using sub_id_t = typename SubscribeDelegate<T>::sub_id_t;
    using sub_cb_t = typename SubscribeDelegate<T>::sub_cb_t;

    /**
     * @brief Constructor
     *
     * @param sub_d SubscribeDelegate to use for subscribing
     * @param store_d StorageDelegate to use for storing
     */
    BaseData(SubscribeDelegate<T> *sub_d, StorageDelegate<T> *store_d, const char *name) :
        BaseDataGeneric(name), sub_d(sub_d), store_d(store_d)
    {
        if(!store_d->load_or_reset(value)){
            store_d->set_default(value);
        }
    }

    /**
     * @brief Deleted Copy Constructor
     *
     */
    BaseData(const BaseData<T> &) = delete;

    /**
     * @brief Default Move Constructor
     *
     */
    BaseData(BaseData<T> &&) = default;

    /**
     * @brief Print the object's value. Object's type must provide it's own printing ability
     * via overriding the ostream << operator
     */
    virtual void print(uint32_t indent_depth = 0) override{
        for(int i = 0; i < indent_depth; i++)
            std::cout << "  ";
        if(name)
            std::cout << name << ": ";
        std::cout << value << '\n';
    }

    virtual void log_on_sub(bool set = true) override{
        en_logging = set;
    }

    /**
     * @brief Return a constant reference to the stored value
     *
     * @return const T& a constant reference to the stored value
     */
    virtual const T &get(void) const final {
        return value;
    }

    virtual void set_default(void) override final {
        store_d->set_default(value);
        notify(value);
    }

    /**
     * @brief Use the StorageDelegate to reset the value
     *
     */
    virtual void reset(void) override final {
        store_d->reset(value);
        notify(value);
    }

    virtual void load_or_reset(void) override final {
        if(store_d->load_or_reset(value)){
            notify(value);
        }
    }

    /**
     * @brief Use the SubscribeDelegate to subscribe to changes in the internal value
     *
     * @param sub_cb Callback to be called when the internal value changes
     * @param immediate Whether or not to call the callback immediately
     * @return sub_id_t id to be used to unsub
     */
    virtual sub_id_t sub(sub_cb_t sub_cb, bool immediate = false) final {
        if (immediate) {
            sub_cb(value);
        }
        return sub_d->sub(sub_cb);
    }

    /**
     * @brief Use the SubscribeDelegate to unsubscribe from changes to the internal data
     *
     * @param sub_id id returned by sub
     */
    virtual void unsub(sub_id_t sub_id) final {
        sub_d->unsub(sub_id);
    }

    /**
     * @brief Stop subscribers from being notified of changes
     */
    virtual void mute_sub() override final{
        muted = true;
    }

    /**
     * @brief If mute has been previously called, then unmute and notify subscribers
     */
    virtual void unmute_sub() override final{
        if(muted){
            muted = false;
            notify(value);
        }
    }

protected:
    /**
     * @brief Use the SubscribeDelegate to notify all subscribers
     *
     * @param next
     */
    void notify(const T &next) const {
        if(muted) return;
        if(en_logging){
            if(name)
                std::cout << name << ": ";
            std::cout << value << "->" << next << '\n';
        }
        sub_d->notify(next);
    }

    /**
     * @brief Use the StorageDelegate to store the internal value
     *
     */
    void store(void) {
        store_d->store(*this);
    }

    T value;
private:
    bool muted = false;
    bool en_logging = false;

    SubscribeDelegate<T> *sub_d;
    StorageDelegate<T> *store_d;
};

};
