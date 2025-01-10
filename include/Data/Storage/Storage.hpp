#pragma once

// Internal includes

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

template<class T>
class BaseData;

/**
 * @brief Abstract interface who's implementations provide the means of storing,
 * loading, or resetting a value using an external source
 *
 * @tparam T Type being stored
 */
template <typename T>
class StorageDelegate {
public:
    /**
     * @brief Destructor
     *
     */
    virtual ~StorageDelegate() = default;

    /**
     * @brief Reset internal value, but not its stored value
     *
     * @param value Reference to the value being loaded / reset
     */
    virtual void set_default(T &value) const = 0;

    /**
     * @brief Try to load a value from an external source and if the load fails
     * then reset the value
     *
     * @param value Reference to the value being loaded / reset
     * @retval True if the value was potentially modified
     */
    virtual bool load_or_reset(T &value) const = 0;

    /**
     * @brief Store an object's value to an external source
     *
     * @param object The object who's value is to be stored
     */
    virtual void store(const BaseData<T> &object) = 0;

    /**
     * @brief Reset a value, including its stored value
     *
     * @param value Reference to the value being reset
     */
    virtual void reset(T &value) const = 0;
};

};
