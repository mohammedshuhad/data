#pragma once

// Internal includes
#include "Storage.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes
#include <vector>

namespace Data {

/**
 * @brief StorageDelegate that does not store or load the value and only clears the vector
 *
 * @tparam T Type stored in vector
 */
template <typename T>
class StorageVectorNone : public StorageDelegate<std::vector<T>> {
public:
    /**
     * @brief Constructor
     *
     */
    StorageVectorNone(void) { }

    /**
     * @brief Destructor
     *
     */
    virtual ~StorageVectorNone() = default;

    /**
     * @brief Reset the value to the stored default value
     *
     * @param value Reference to the value being reset
     */
    virtual void set_default(std::vector<T> &value) const override final {
        value.clear();
    }

    /**
     * @brief StoregeVectorNone has no external storage to load from, and 
     * won't reset to default on load_or_reset()
     *
     * @param value Reference to the value being loaded / reset
     * @retval True if the value was potentially modified
     */
    virtual bool load_or_reset(std::vector<T> &value) const override final {
        return false;
    }

    /**
     * @brief Does nothing
     *
     * @param object unused
     */
    virtual void store(const BaseData<std::vector<T>> &object) override final {
        // DOES NOTHING
    }

    /**
     * @brief Clears a vector
     *
     * @param value Reference to the vector being cleared
     */
    virtual void reset(std::vector<T> &value) const override final {
        value.clear();
    }
};

};
