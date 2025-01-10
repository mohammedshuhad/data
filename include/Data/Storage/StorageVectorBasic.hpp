#pragma once

// Internal includes
#include "Storage.hpp"
#include "Data/Helper/NvsHandler.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes
#include <vector>

namespace Data {

/**
 * @brief StorageDelegate to load and store vectors of values from nvs along being
 * able to clear the vector
 *
 * @tparam T Type stored in vector
 */
template <typename T>
class StorageVectorBasic : public StorageDelegate<std::vector<T>>, public Helper::NvsHandler::Block {
public:
    /**
     * @brief Constructor
     *
     * @param nvs_handler Pointer to a NvsHandler to use for storing / loading
     * @param nvs_key Nvs key use to load / store the value
     */
    StorageVectorBasic(Helper::NvsHandler *nvs_handler, const char *nvs_key) :
        nvs_handler(nvs_handler), nvs_key(nvs_key), object(nullptr) { }

    /**
     * @brief Destructor
     *
     */
    virtual ~StorageVectorBasic() {
        nvs_handler->unsub(nvs_key);
    }

    /**
     * @brief Reset the value to the stored default value
     *
     * @param value Reference to the value being reset
     */
    virtual void set_default(std::vector<T> &value) const override final {
        value.clear();
    }

    /**
     * @brief Try to load a vector from nvs and if the load fails
     * then clear the vector
     *
     * @param value Reference to the vector being loaded / cleared
     */
    virtual bool load_or_reset(std::vector<T> &value) const override final {
        size_t size = nvs_handler->size(nvs_key);
        value.resize(size / sizeof(T));
        return (size > 0 && size % sizeof(T) == 0 && nvs_handler->load(nvs_key, &value[0], size));
    }

    /**
     * @brief Store the object's vector to nvs
     *
     * @param object The object who's vector is to be stored
     */
    virtual void store(const BaseData<std::vector<T>> &object) override final {
        this->object = &object;
        nvs_handler->sub(nvs_key, this);
    }

    /**
     * @brief Clears a vector
     *
     * @param value Reference to the vector being cleared
     */
    virtual void reset(std::vector<T> &value) const override final {
        nvs_handler->reset(nvs_key);
        value.clear();
    }

    virtual void commit(Helper::NvsHandler *handler, const char *key) const override final {
        const std::vector<T> &value = object->get();
        if (value.size() == 0) {
            handler->reset(nvs_key);
        } else {
            handler->store(key, &value[0], value.size() * sizeof(T));
        }
    }
private:
    Helper::NvsHandler *nvs_handler;
    const char *nvs_key;
    const BaseData<std::vector<T>> *object;
};

};
