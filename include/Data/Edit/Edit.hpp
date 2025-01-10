#pragma once

// Internal includes

// bwl component includes

// Esp-idf component includes

// Standard library includes
#include <functional>

namespace Data {

/**
 * @brief Abstract interface of an object that can have it's internal
 * value be editited through the use of a callback function
 *
 * @tparam T Type being edited
 */
template <typename T>
class EditObject {
public:
    /**
     * @brief Callback function that is used to edit an object's internal value
     *
     * @param T Mutable reference to an object's internal value
     * @retval True if the objects' internal value was modified and subscribers should be updated
     */
    using edit_cb_t = std::function<bool(T &)>;

    /**
     * @brief Destructor
     *
     */
    virtual ~EditObject() = default;

    /**
     * @brief Function used to edit an object's internal value
     *
     * @param edit_cb Callback function that has a mutable reference of
     * an object's internal value passed into it
     */
    virtual void edit(edit_cb_t edit_cb) = 0;
};

};
