#pragma once

// Internal includes
#include "Set.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

/**
 * @brief SetDelegate that always sets the value
 *
 * @tparam T Type being set
 */
template <typename T>
class SetAlways : public SetDelegate<T> {
public:
    /**
     * @brief Constructor
     *
     */
    SetAlways(void) { }

    /**
     * @brief Destructor
     *
     */
    virtual ~SetAlways() = default;

    /**
     * @brief Always returns true
     *
     * @param value usused
     * @param next unused
     * @return true yes
     */
    virtual bool verify(const T &value, const T &next) const override final {
        return true;
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