#pragma once

// Internal includes
#include "Subscribe.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes
#include <functional>
#include <vector>

namespace Data {

/**
 * @brief SubscribeDelegate that implements basic subscriber functionality
 *
 * @tparam T Type subscribers are notified with
 */
template <typename T>
class SubscribeBasic : public SubscribeDelegate<T> {
public:
    using sub_id_t = typename SubscribeDelegate<T>::sub_id_t;
    using sub_cb_t = typename SubscribeDelegate<T>::sub_cb_t;

    /**
     * @brief Constructor
     *
     */
    SubscribeBasic(void) :
        subs() { }

    /**
     * @brief Destructor
     *
     */
    virtual ~SubscribeBasic() = default;

    /**
     * @brief Add the new sub callback to a vector
     *
     * @param sub_cb The new callback to add
     * @return sub_id_t The index of the new callback in the vector
     */
    virtual sub_id_t sub(sub_cb_t sub_cb) override final {
        for (size_t i = 0; i < subs.size(); i++) {
            if (subs[i] == nullptr) {
                subs[i] = sub_cb;
                return i;
            }
        }

        size_t id = subs.size();
        subs.push_back(sub_cb);
        return id;
    }

    /**
     * @brief Remove a callback from the vector of callbacks
     *
     * @param sub_id The index of the callback to remove
     */
    virtual void unsub(sub_id_t sub_id) override final {
        if (sub_id < subs.size()) {
            subs[sub_id] = nullptr;
        }
    }

    /**
     * @brief Call all of the callback with 'value' as the parameter
     *
     * @param value The value to call all the callbacks with
     */
    virtual void notify(const T &value) const override final {
        for (size_t i = 0; i < subs.size(); i++) {
            if (subs[i]) {
                subs[i](value);
            }
        }
    }
private:
    std::vector<sub_cb_t> subs;
};

};
