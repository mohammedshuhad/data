#pragma once

// Internal includes

// bwl component includes

// Esp-idf component includes

// Standard library includes
#include <functional>

namespace Data {

/**
 * @brief Abstract interface who's implementations provide the means of subscribing,
 * unsubscribing and notifying subscribers
 *
 * @tparam T Type subscribers are notified with
 */
template <typename T>
class SubscribeDelegate {
public:
    /**
     * @brief Id returned from sub that is used to usub
     *
     */
    using sub_id_t = size_t;

    /**
     * @brief Callback function that is used when subscribers are notified
     *
     */
    using sub_cb_t = std::function<void(const T &)>;


    /**
     * @brief Destructor
     *
     */
    virtual ~SubscribeDelegate() = default;

    /**
     * @brief Subscribe to changes of a value
     *
     * @param sub_cb Callback function that is used when the value is changed
     * @return sub_id_t The id used to unsubscribe this function
     */
    virtual sub_id_t sub(sub_cb_t sub_cb) = 0;

    /**
     * @brief Unsubscribe from changes of a value
     *
     * @param sub_id The id returned from sub
     */
    virtual void unsub(sub_id_t sub_id) = 0;

    /**
     * @brief Notify all subscribers that the value has changed
     *
     * @param value The new value to send to the subscribers
     */
    virtual void notify(const T &value) const = 0;
};

};
