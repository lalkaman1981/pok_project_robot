#ifndef SD_MMC_UTILS_H
#define SD_MMC_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_err.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "sdmmc_cmd.h"
#include "sys/dirent.h"

// Mount point for the SD card
#define MOUNT_POINT "/sdcard"


static const char *TAGI = "SD_MMC";


// Function declarations
void setUpSd(void);
void list_dir(const char *path, int levels);
void create_dir(const char *path);
void remove_dir(const char *path);
char* read_file(const char *path);
void write_file(const char *path, const char *message);
void append_file(const char *path, const char *message);
void rename_file(const char *src, const char *dest);
void delete_file(const char *path);
void test_file_io(const char *path);

#ifdef __cplusplus
}
#endif

#endif // SD_MMC_UTILS_H