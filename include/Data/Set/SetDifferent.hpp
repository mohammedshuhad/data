#pragma once

// Internal includes
#include "Set.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

/**
 * @brief SetDelegate that only sets the value if it is different from the current value
 *
 * @tparam T Type being set
 */
template <typename T>
class SetDifferent : public SetDelegate<T> {
public:
    /**
     * @brief Constructor
     *
     */
    SetDifferent(void) { }

    /**
     * @brief Destructor
     *
     */
    ~SetDifferent() = default;

    /**
     * @brief Returns whether or not value != next
     *
     * @param value Reference to the current value
     * @param next Reference to the new value
     * @return true The new value is valid
     * @return false The new value is not valid
     */
    virtual bool verify(const T &value, const T &next) const override final {
        return value != next;
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
};

};