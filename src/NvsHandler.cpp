// Internal includes
#include "Data/Helper/NvsHandler.hpp"

// bwl component includes

// Esp-idf component includes
#include "esp_log.h"

// Standard library includes
#include <cstring>

#define TAG "NvsHandler"

using namespace Data::Helper;

NvsHandler::NvsHandler(const char *nvs_name) :
        nvs_name(nvs_name), nvs_handle(0)
{
    esp_err_t err = nvs_open(nvs_name, NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK) {
        // DOES NOTHING
    } else {
        ESP_LOGE(TAG, "Error opening %s: %s", nvs_name, esp_err_to_name(err));
    }
}

NvsHandler::~NvsHandler() {
    esp_err_t err = nvs_commit(nvs_handle);
    if (err == ESP_OK) {
        // DOES NOTHING
    } else {
        ESP_LOGE(TAG, "Error committing %s: %s", nvs_name, esp_err_to_name(err));
    }
    nvs_close(nvs_handle);
}

size_t NvsHandler::size(const char *key) {
    size_t length = 0;
    esp_err_t err = nvs_get_blob(nvs_handle, key, NULL, &length);
    if (err == ESP_OK) {
        return length;
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        return 0;
    } else {
        ESP_LOGE(TAG, "Error getting size of %s: %s", key, esp_err_to_name(err));
        return 0;
    }
}

bool NvsHandler::load(const char *key, void *data, size_t data_sz) {
    esp_err_t err = nvs_get_blob(nvs_handle, key, data, &data_sz);
    if (err == ESP_OK) {
        // printf("load succeed\n");
        return true;
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        // printf("load fail\n");
        return false;
    } else {
        ESP_LOGE(TAG, "Error getting %s: %s", key, esp_err_to_name(err));
        return false;
    }
}

void NvsHandler::store(const char *key, const void *data, size_t data_sz) {
    esp_err_t err = nvs_set_blob(nvs_handle, key, data, data_sz);
    if (err == ESP_OK) {
        // DOES NOTHING
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        // DOES NOTHING
    } else {
        ESP_LOGE(TAG, "Error setting %s: %s", key, esp_err_to_name(err));
    }
}

void NvsHandler::reset(const char *key) {
    subs.erase(key);

    esp_err_t err = nvs_erase_key(nvs_handle, key);
    if (err == ESP_OK) {
        // DOES NOTHING
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        // DOES NOTHING
    } else {
        ESP_LOGE(TAG, "Error erasing %s: %s", key, esp_err_to_name(err));
    }
}

void NvsHandler::sub(const char *key, Block *block) {
    subs[key] = block;
}

void NvsHandler::unsub(const char *key) {
    subs.erase(key);
}

void NvsHandler::commit(void) {
    for (auto it = subs.begin(); it != subs.end(); it++) {
        it->second->commit(this, it->first);
    }

    subs.clear();

    esp_err_t err = nvs_commit(nvs_handle);
    if (err == ESP_OK) {
        // DOES NOTHING
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        // DOES NOTHING
    } else {
        ESP_LOGE(TAG, "Error committing %s: %s", nvs_name, esp_err_to_name(err));
    }
}

bool NvsHandler::CmpKey::operator() (const char *k1, const char *k2) const {
    return std::strcmp(k1, k2) < 0;
}
