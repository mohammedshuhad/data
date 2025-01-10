#pragma once

// Internal includes
#include "BaseData.hpp"
#include "Set/Set.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

/**
 * @brief BaseData that also can be set
 *
 * @tparam T Type of value to hold
 */
template <typename T>
class SetData : public BaseData<T>, public SetObject<T> {
public:
    /**
     * @brief Constructor
     *
     * @param sub_d SubscribeDelegate to use for subscribing
     * @param store_d StorageDelegate to use for storing
     * @param set_d SetDelegate to use for setting
     */
    SetData(SubscribeDelegate<T> *sub_d, StorageDelegate<T> *store_d, SetDelegate<T> *set_d, const char *name) :
        BaseData<T>(sub_d, store_d, name), set_d(set_d) { }

    /**
     * @brief Deleted Copy Constructor
     *
     */
    SetData(const SetData &) = delete;

    /**
     * @brief Default Move Constructor
     *
     */
    SetData(SetData &&) = default;

    /**
     * @brief Optionally set this object's internal value to a new value
     *
     * @param next Reference to the potential new value
     */
    virtual void set(const T &next) override final {
        if (set_d->verify(BaseData<T>::value, next)) {
            BaseData<T>::notify(next);
            set_d->copy(BaseData<T>::value, next);
            BaseData<T>::store();
        }
    }
protected:
    const SetDelegate<T> *getSetDelegate(void) const {
        return set_d;
    }
private:
    SetDelegate<T> *set_d;
};

};
