#pragma once

// Internal includes

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

/**
 * @brief Abstract interface of an object that can have it's internal
 * value be set to a new value
 *
 * @tparam T Type being set
 */
template <typename T>
class SetObject {
public:
    /**
     * @brief Destructor
     *
     */
    virtual ~SetObject() = default;

    /**
     * @brief Optionally set the object's internal value to a new value
     *
     * @param next Reference to the potential new value
     */
    virtual void set(const T &next) = 0;

    /**
     * @brief Optionally set the object's internal value to a new value
     *
     * @param next Reference to the potential new value
     */
    void set(const T &&next) {
        set(next);
    }
};

/**
 * @brief Abstract interface who's implementations provide the means of
 * checking if a value should be changed as well as providing the means
 * to set the value
 *
 * @tparam T Type being set
 */
template <typename T>
class SetDelegate {
public:
    /**
     * @brief Destructor
     *
     */
    virtual ~SetDelegate() = default;

    /**
     * @brief Check if a value should be changed
     *
     * @param value Reference to the current value
     * @param next Reference to the potential new value
     * @return true The new value is valid
     * @return false The new value is not valid
     */
    virtual bool verify(const T &value, const T &next) const = 0;

    /**
     * @brief Assign next to value
     *
     * @param value Reference to the current value
     * @param next Reference to the new value
     */
    virtual void copy(T &value, const T &next) const = 0;
};

};