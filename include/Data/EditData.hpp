#pragma once

// Internal includes
#include "BaseData.hpp"
#include "Edit/Edit.hpp"

// bwl component includes

// Esp-idf component includes
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/semphr.h"

// Standard library includes

namespace Data {

/**
 * @brief BaseData that can also be edited
 *
 * @tparam T Type of value to hold
 */
template <typename T>
class EditData : public BaseData<T>, public EditObject<T> {
public:
    using edit_cb_t = typename EditObject<T>::edit_cb_t;

    /**
     * @brief Constructor
     *
     * @param sub_d Delegate to use for subscribing
     * @param store_d Delegate to use for storing
     */
    EditData(SubscribeDelegate<T> *sub_d, StorageDelegate<T> *store_d, const char *name) :
        BaseData<T>(sub_d, store_d, name)
    {
        sem_h = xSemaphoreCreateBinary();
        xSemaphoreGive(sem_h);
    }

    /**
     * @brief Deleted Copy Constructor
     *
     */
    EditData(const EditData<T> &) = delete;

    /**
     * @brief Default Move Constructor
     *
     */
    EditData(EditData<T> &&) = default;

    /**
     * @brief Destructor
     *
     */
    virtual ~EditData() {
        vSemaphoreDelete(sem_h);
    }

    /**
     * @brief edit this object's internal value
     *
     * @param edit_cb Callback function that has a mutable reference of
     * an object's internal value passed into it
     */
    virtual void edit(edit_cb_t edit_cb) override final {
        xSemaphoreTake(sem_h, portMAX_DELAY);
        if (edit_cb(BaseData<T>::value)) {
            BaseData<T>::notify(BaseData<T>::value);
            BaseData<T>::store();
        }
        xSemaphoreGive(sem_h);
    }
private:
    SemaphoreHandle_t sem_h;
};


};
