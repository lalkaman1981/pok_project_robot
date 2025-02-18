#include "sd_card.h"

void setUpSd(void) {
    esp_err_t ret;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.slot = SDMMC_HOST_SLOT_1;

    // Configure SDMMC pins
    host.flags = SDMMC_HOST_FLAG_8BIT;
    host.max_freq_khz = SDMMC_FREQ_DEFAULT;

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.clk = GPIO_NUM_39;   // CLK pin
    slot_config.cmd = GPIO_NUM_38;  // CMD pin
    slot_config.d0 = GPIO_NUM_40;   // D0 pin

    // Mount SD card
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        ESP_LOGE(TAGI, "Failed to mount filesystem. Error: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAGI, "Filesystem mounted");

    // Card info
    sdmmc_card_print_info(stdout, card);

    // List root directory
    list_dir(MOUNT_POINT, 0);

    // Create and manipulate files/directories

    // Unmount SD card
    ESP_LOGI(TAGI, "Card unmounted");
}

void list_dir(const char *path, int levels) {
    ESP_LOGI(TAGI, "Listing directory: %s", path);

    DIR *dir = opendir(path);
    if (!dir) {
        ESP_LOGE(TAGI, "Failed to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            ESP_LOGI(TAGI, "  DIR : %s", entry->d_name);
            if (levels > 0) {
                char subpath[512];
                snprintf(subpath, sizeof(subpath), "%s/%s", path, entry->d_name);
                list_dir(subpath, levels - 1);
            }
        } else {
            ESP_LOGI(TAGI, "  FILE: %s", entry->d_name);
        }
    }
    closedir(dir);
}

void create_dir(const char *path) {
    ESP_LOGI(TAGI, "Creating directory: %s", path);
    if (mkdir(path, 0777) != 0) {
        ESP_LOGE(TAGI, "Failed to create directory");
    } else {
        ESP_LOGI(TAGI, "Directory created");
    }
}

void remove_dir(const char *path) {
    ESP_LOGI(TAGI, "Removing directory: %s", path);
    if (rmdir(path) != 0) {
        ESP_LOGE(TAGI, "Failed to remove directory");
    } else {
        ESP_LOGI(TAGI, "Directory removed");
    }
}

char* read_file(const char *path) {
    ESP_LOGI(TAGI, "Reading file: %s", path);

    FILE *file = fopen(path, "r");
    if (!file) {
        ESP_LOGE(TAGI, "Failed to open file for reading");
        return NULL;
    }

    char *buffer = (char *)malloc(10000);
    if (!buffer) {
        ESP_LOGE(TAGI, "Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    size_t total_length = 0;
    size_t read_length;
    while ((read_length = fread(buffer + total_length, 1, 10000 - total_length - 1, file)) > 0) {
        total_length += read_length;
    }

    buffer[total_length] = '\0';
    fclose(file);

    return buffer;
}


void write_file(const char *path, const char *message) {
    ESP_LOGI(TAGI, "Writing file: %s", path);

    FILE *file = fopen(path, "w");
    if (!file) {
        ESP_LOGE(TAGI, "Failed to open file for writing");
        return;
    }

    if (fprintf(file, "%s", message) < 0) {
        ESP_LOGE(TAGI, "Failed to write to file");
    } else {
        ESP_LOGI(TAGI, "File written");
    }
    fclose(file);
}

void append_file(const char *path, const char *message) {
    ESP_LOGI(TAGI, "Appending to file: %s", path);

    FILE *file = fopen(path, "a");
    if (!file) {
        ESP_LOGE(TAGI, "Failed to open file for appending");
        return;
    }

    if (fprintf(file, "%s", message) < 0) {
        ESP_LOGE(TAGI, "Failed to append to file");
    } else {
        ESP_LOGI(TAGI, "Message appended");
    }
    fclose(file);
}

void rename_file(const char *src, const char *dest) {
    ESP_LOGI(TAGI, "Renaming file %s to %s", src, dest);

    if (rename(src, dest) != 0) {
        ESP_LOGE(TAGI, "Failed to rename file");
    } else {
        ESP_LOGI(TAGI, "File renamed");
    }
}

void delete_file(const char *path) {
    ESP_LOGI(TAGI, "Deleting file: %s", path);

    if (unlink(path) != 0) {
        ESP_LOGE(TAGI, "Failed to delete file");
    } else {
        ESP_LOGI(TAGI, "File deleted");
    }
}

void test_file_io(const char *path) {
    ESP_LOGI(TAGI, "Testing file I/O: %s", path);

    FILE *file = fopen(path, "w+");
    if (!file) {
        ESP_LOGE(TAGI, "Failed to open file for I/O test");
        return;
    }

    const size_t buf_size = 512;
    uint8_t *buf = malloc(buf_size);
    if (!buf) {
        ESP_LOGE(TAGI, "Failed to allocate buffer");
        fclose(file);
        return;
    }

    for (int i = 0; i < 2048; i++) {
        fwrite(buf, 1, buf_size, file);
    }
    ESP_LOGI(TAGI, "Write test completed");

    fseek(file, 0, SEEK_SET);
    while (fread(buf, 1, buf_size, file) == buf_size) {
    }
    ESP_LOGI(TAGI, "Read test completed");

    free(buf);
    fclose(file);
}
