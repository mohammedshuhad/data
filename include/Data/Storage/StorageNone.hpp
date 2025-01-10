#pragma once

// Internal includes
#include "Storage.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

/**
 * @brief StorageDelegate that does not store or load the value and only resets it
 * to a saved default value
 *
 * @tparam T Type being reset
 */
template <typename T>
class StorageNone : public StorageDelegate<T> {
public:
    /**
     * @brief Constructor
     *
     * @param default_value The default value to use when resetting
     */
    StorageNone(const T default_value) :
        default_value(default_value) { }

    /**
     * @brief Destructor
     *
     */
    virtual ~StorageNone() = default;

    /**
     * @brief Reset the value to the stored default value
     *
     * @param value Reference to the value being reset
     */
    virtual void set_default(T &value)const override final {
        value = default_value;
    }

    /**
     * @brief StoregeNone has no external storage to load from, and 
     * won't reset to default on load_or_reset()
     *
     * @param value Reference to the value being loaded / reset
     * @retval True if the value was potentially modified
     */
    virtual bool load_or_reset(T &value) const override final {
        return false;
    }

    /**
     * @brief Does nothing
     *
     * @param object unused
     */
    virtual void store(const BaseData<T> &object) override final {
        // DOES NOTHING
    }

    /**
     * @brief Reset a value to the stored default value
     *
     * @param value Reference to the value being reset
     */
    virtual void reset(T &value) const override final {
        value = default_value;
    }
private:
    const T default_value;
};

};
