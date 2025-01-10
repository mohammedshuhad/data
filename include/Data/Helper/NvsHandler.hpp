#pragma once

// Internal includes

// bwl component includes

// Esp-idf component includes
#include "nvs_flash.h"

// Standard library includes
#include <map>

namespace Data {

namespace Helper {

/**
 * @brief Class to wrap nvs as well as maintain a map of Blocks that want to
 * do a save before commiting
 *
 */
class NvsHandler {
public:
    /**
     * @brief Abstract class used to subscribe to the handlers commits
     *
     */
    class Block {
    public:
        /**
         * @brief Function called when the handler has commit called
         *
         * @param handler The handler to use for any nvs operations
         * @param key The key to use for any nvs operations
         */
        virtual void commit(NvsHandler *handler, const char *key) const;
    };

    /**
     * @brief Constructor
     *
     * @param nvs_name the name to use when opening nvs
     */
    NvsHandler(const char *nvs_name);

    /**
     * @brief Destructor
     *
     */
    ~NvsHandler();

    /**
     * @brief Returns the size of an blob stored in nvs
     *
     * @param key The key associated with the blob to get the size of
     * @return size_t The size of the blob
     */
    size_t size(const char *key);

    /**
     * @brief Load a blob from nvs
     *
     * @param key The key associated with the blob to load
     * @param data Pointer to load the blob into
     * @param data_sz Size of the blob to load
     * @return true The blob was loaded
     * @return false The blob could not be loaded
     */
    bool load(const char *key, void *data, size_t data_sz);

    /**
     * @brief Store a blob to nvs
     *
     * @param key The key associated with the blob to store
     * @param data Pointer to the blob to store
     * @param data_sz Size of the blob to store
     */
    void store(const char *key, const void *data, size_t data_sz);

    /**
     * @brief Removes a key value pair from nvs
     *
     * @param key The key to remove
     */
    void reset(const char *key);

    /**
     * @brief Register an object to be saved when the commit function is called
     *
     * @param key The key associated with the object to register
     * @param block Pointer to a block that knows how to save itself
     */
    void sub(const char *key, Block *block);

    /**
     * @brief Unregister an object from being saved when the commit function is called
     *
     * @param key The key associated with the object to unregister
     */
    void unsub(const char *key);

    /**
     * @brief Save all subscribed objects to nvs and commit nvs
     *
     */
    void commit(void);

    /**
     * @brief Load a value from nvs
     *
     * @tparam T Type to load
     * @param key The key associated with the value to load
     * @param value Reference of the value to load into
     * @return true The value was loaded
     * @return false The value could not be loaded
     */
    template <typename T>
    bool load(const char *key, T &value);

    /**
     * @brief Store a value to nvs
     *
     * @tparam T Type to store
     * @param key The key associated with the value to store
     * @param value Reference of the value to store
     */
    template <typename T>
    void store(const char *key, const T &value);
private:
    const char *nvs_name;
    nvs_handle_t nvs_handle;

    class CmpKey {
    public:
        bool operator() (const char *k1, const char *k2) const;
    };

    std::map<const char *, Block*, CmpKey> subs;
};

template <typename T>
bool NvsHandler::load(const char *key, T &value) {
    return load(key, &value, sizeof(T));
}

template <typename T>
void NvsHandler::store(const char *key, const T &value) {
    store(key, &value, sizeof(T));
}

};

};