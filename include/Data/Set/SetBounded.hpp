#pragma once

// Internal includes
#include "Set.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

/**
 * @brief Abstract interface of an object that holds a
 * minimim and maximum value
 *
 * @tparam T Type of min and max
 */
template <typename T>
class BoundedObject {
public:
    /**
     * @brief Destructor
     *
     */
    virtual ~BoundedObject() = default;

    /**
     * @brief Get the stored minimum
     *
     * @return T the stored minimum
     */
    virtual T get_min(void) const;

    /**
     * @brief Get the stored maximum
     *
     * @return T the stored maximum
     */
    virtual T get_max(void) const;
};

/**
 * @brief SetDelegate that only sets the value if it is withing the range [min, max) and
 * it is different from the current value
 * @note Range is inclusive of min and exclusive of max
 *
 * @tparam T Type being set
 */
template <typename T>
class SetBounded : public SetDelegate<T>, public BoundedObject<T> {
public:
    /**
     * @brief Constructor
     *
     * @param min Minimum valid value inclusive
     * @param max Maximum valid value exclusive
     */
    SetBounded(const T min, const T max) :
        min(min), max(max) { }

    /**
     * @brief Destructor
     *
     */
    virtual ~SetBounded() = default;

    /**
     * @brief Returns whether or not min <= next < max and value != next
     *
     * @param value Reference to the current value
     * @param next Reference to the new value
     * @return true The new value is valid
     * @return false The new value is not valid
     */
    virtual bool verify(const T &value, const T &next) const override final {
        return value != next && min <= next && next < max;
    }

    /**
     * @brief Uses the '=' operator to copy over the value
     *
     * @param value Reference to the current value
     * @param next Reference to the new value
     */
    virtual void copy(T &value, const T &next) const override final {
        value = next;
    }

    /**
     * @brief Get the stored minimum
     *
     * @return T the stored minimum
     */
    virtual T get_min() const override final {
        return min;
    }

    /**
     * @brief Get the stored maximum
     *
     * @return T the stored maximum
     */
    virtual T get_max() const override final {
        return max;
    }

private:
    const T min;
    const T max;
};

};
