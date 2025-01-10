#pragma once

// Internal includes
#include "Storage.hpp"
#include "Data/Helper/NvsHandler.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

/**
 * @brief StorageDelegate to load and store values from nvs along with resetting
 * them to a saved default value
 * @note Also inherits from NvsHandler::Block allowing it to subscribe to the NvsHandler's commits
 *
 * @tparam T Type being stored
 */
template <typename T>
class StorageBasic : public StorageDelegate<T>, public Helper::NvsHandler::Block {
public:
    /**
     * @brief Constructor
     *
     * @param default_value The default value to use when resetting
     * @param nvs_handler Pointer to a NvsHandler to use for storing / loading
     * @param nvs_key Nvs key use to load / store the value
     */
    StorageBasic(const T default_value, Helper::NvsHandler *nvs_handler, const char *nvs_key) :
        default_value(default_value), nvs_handler(nvs_handler), nvs_key(nvs_key), object(nullptr) { }

    /**
     * @brief Destructor
     *
     */
    virtual ~StorageBasic() {
        nvs_handler->unsub(nvs_key);
    }

    /**
     * @brief Reset the value to the stored default value
     *
     * @param value Reference to the value being reset
     */
    virtual void set_default(T &value) const override final {
        value = default_value;
    }

    /**
     * @brief Try to load a value from an external source and if the load fails
     * then reset the value
     *
     * @param value Reference to the value being loaded / reset
     * @retval True if the value was potentially modified
     */
    virtual bool load_or_reset(T &value) const override final {
        if (nvs_handler->load(nvs_key, value)) {
            // DOES NOTHING
        } else {
            reset(value);
        }
        return true; // Always indicate that the value changed
    }

    /**
     * @brief Store the object's value to nvs
     *
     * @param object The object who's value is to be stored
     */
    virtual void store(const BaseData<T> &object) override final {
        this->object = &object;
        nvs_handler->sub(nvs_key, this);
    }

    /**
     * @brief Reset the value to the default value and clear the stored value
     *
     * @param value Reference to the value being reset
     */
    virtual void reset(T &value) const override final {
        nvs_handler->reset(nvs_key);
        value = default_value;
    }

    /**
     * @brief If store was previously called then write the object's value to nvs
     *
     * @param handler Pointer to the handler used for the store
     * @param key Nvs key to use for the store
     */
    virtual void commit(Helper::NvsHandler *handler, const char *key) const override final {
        handler->store(key, object->get());
    }
private:
    const T default_value;
    Helper::NvsHandler *nvs_handler;
    const char *nvs_key;
    const BaseData<T> *object;
};

};
