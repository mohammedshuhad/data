#pragma once

// Internal includes
#include "Data/BaseData.hpp"
#include "Data/EditData.hpp"
#include "Data/SetData.hpp"
#include "Data/SetBoundedData.hpp"

#include "Model.hpp"

#include "Data/Subscribe/Subscribe.hpp"
#include "Data/Subscribe/SubscribeBasic.hpp"

#include "Data/Storage/Storage.hpp"
#include "Data/Storage/StorageNone.hpp"
#include "Data/Storage/StorageBasic.hpp"
#include "Data/Storage/StorageVectorNone.hpp"
#include "Data/Storage/StorageVectorBasic.hpp"

#include "Data/Set/Set.hpp"
#include "Data/Set/SetAlways.hpp"
#include "Data/Set/SetDifferent.hpp"

#include "Data/Edit/Edit.hpp"

// bwl component includes

// Esp-idf component includes

// Standard library includes

namespace Data {

namespace Factory {

Helper::NvsHandler *GetNvsHandler(void);

/**
 * @brief Logic to handle setting the name is an nvs_key is provided and a name is not,
 * to set the nvs key in this case.
 */
const char *get_name(const char *nvs_key, const char *name);

/**
 * @brief Make a StorageDelegate that is either StorageNone if the nvs key is NULL
 * or StorageBasic if the nvs key is not NULL
 *
 */
template <typename T>
StorageDelegate<T> *MakeStorageDelegate(const T default_value, const char *nvs_key) {
    if (nvs_key == NULL) {
        return new StorageNone<T>(default_value);
    } else {
        return new StorageBasic<T>(default_value, GetNvsHandler(), nvs_key);
    }
}

/**
 * @brief Make a StorageDelegate that is either StorageVectorNone if the nvs key is NULL
 * or StorageVectorBasic if the nvs key is not NULL
 *
 */
template <typename T>
StorageDelegate<std::vector<T>> *MakeStorageVectorDelegate(const char *nvs_key) {
    if (nvs_key == NULL) {
        return new StorageVectorNone<T>();
    } else {
        return new StorageVectorBasic<T>(GetNvsHandler(), nvs_key);
    }
}

/**
 * @brief Make a SetData that uses the SetAlways SetDelegate
 *
 */
template <typename T>
SetData<T> MakeSetAlways(const T default_value, const char *nvs_key, const char *name = nullptr) {
    return SetData<T>(
        new SubscribeBasic<T>(),
        MakeStorageDelegate(default_value, nvs_key),
        new SetAlways<T>(),
        get_name(nvs_key, name)
    );
}

/**
 * @brief Make a SetData that uses the SetDifferent SetDelegate
 *
 */
template <typename T>
SetData<T> MakeSetDifferent(const T default_value, const char *nvs_key, const char *name = nullptr) {
    return SetData<T>(
        new SubscribeBasic<T>(),
        MakeStorageDelegate(default_value, nvs_key),
        new SetDifferent<T>(),
        get_name(nvs_key, name)
    );
}

/**
 * @brief Make a SetData that uses the SetBounded SetDelegate
 *
 */
template <typename T>
SetBoundedData<T> MakeSetBounded(const T default_value, const T min, const T max, const char *nvs_key, const char *name = nullptr) {
    return SetBoundedData<T>(
        new SubscribeBasic<T>(),
        MakeStorageDelegate(default_value, nvs_key),
        min, max, get_name(nvs_key, name)

    );
}

/**
 * @brief Make an EditData
 *
 */
template <typename T>
EditData<T> MakeEditData(const T default_value, const char *nvs_key, const char *name = nullptr) {
    return EditData<T>(
        new SubscribeBasic<T>(),
        MakeStorageDelegate(default_value, nvs_key),
        get_name(nvs_key, name)
    );
}

/**
 * @brief Make an EditData that stores vectors of values
 *
 */
template <typename T>
EditData<std::vector<T>> MakeEditVector(const char *nvs_key, const char *name = nullptr) {
    return EditData<std::vector<T>>(
        new SubscribeBasic<std::vector<T>>(),
        MakeStorageVectorDelegate<T>(nvs_key),
        get_name(nvs_key, name)
    );
}

};

};
