#pragma once

// Internal includes
#include "SetData.hpp"
#include "Set/SetBounded.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

/**
 * @brief SetData with enforced and queryable bounds on the value
 *
 * @tparam T Type of value to hold
 */
template <typename T>
class SetBoundedData : public SetData<T>, public BoundedObject<T> {
public:
    /**
     * @brief Constructor
     *
     * @param sub_d SubscribeDelegate to use for subscribing
     * @param store_d StorageDelegate to use for storing
     * @param min minimum valid value
     * @param max (one past)maximum valid value
     */
    SetBoundedData(SubscribeDelegate<T> *sub_d, StorageDelegate<T> *store_d, const T min, const T max, const char *name) :
        SetData<T>(sub_d, store_d, new SetBounded<T>(min, max), name) { }

    /**
     * @brief Deleted Copy Constructor
     *
     */
    SetBoundedData(const SetBoundedData<T> &) = delete;

    /**
     * @brief Default Move Constructor
     *
     */
    SetBoundedData(SetBoundedData<T> &&) = default;

    /**
     * @brief Destructor
     *
     */
    virtual ~SetBoundedData() = default;

    /**
     * @brief Get the minimum value of the bounded data
     */
    virtual T get_min() const override final {
        return static_cast<const SetBounded<T>*>(SetData<T>::getSetDelegate())->get_min();
    }

    /**
     * @brief Get the (one past)maximum value of the bounded data
     */
    virtual T get_max() const override final {
        return static_cast<const SetBounded<T>*>(SetData<T>::getSetDelegate())->get_max();
    }
};

};
