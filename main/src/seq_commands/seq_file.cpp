

#include "../seq.h"

#include <dirent.h>
#include "esp_vfs.h"
#include "esp_spiffs.h"

typedef struct s_seq_file_entry
{
    char file_name[SEQ_CONFIG_MAX_FILE_NAME_SIZE];
    char size_str[6];
    uint8_t is_directory;
    uint32_t file_size;
} t_seq_file_entry;

uint32_t seq_file_count_files(char *dir_name)
{
    char mountpoint[SEQ_CONFIG_MAX_FILE_NAME_SIZE];
    struct dirent *entry;
    uint32_t files_listed;

    strlcpy(mountpoint, "/sd", SEQ_CONFIG_MAX_FILE_NAME_SIZE);
    strlcat(mountpoint, dir_name, SEQ_CONFIG_MAX_FILE_NAME_SIZE);
    DIR *dir = opendir(mountpoint);
    if (!dir)
        return 0;
    files_listed = 0;
    /* Iterate over all files / folders and fetch their names and sizes */
    while ((entry = readdir(dir)) != NULL)
        files_listed++;
    closedir(dir);
    return files_listed;
}

void seq_file_init_page(t_seq_file_entry *files)
{
    uint8_t file_count;

    file_count = 0;
    while (file_count < 6)
    {
        files[file_count].file_name[0] = '\0';
        files[file_count].size_str[0] = '\0';
        files[file_count].file_size = 0;
        file_count++;
    }
}

char *seq_file_filename(char *path)
{
    char *filename_start;
    char *filename_head;

    filename_start = path;
    filename_head = path;
    while (*filename_head)
    {
        if (*filename_head == '/')
            filename_start = filename_head + 1;
        filename_head++;
    }
    return (filename_start);
}

char *seq_file_folder_name(char *path)
{
    char *filename_start;
    char *filename_head;

    filename_start = path;
    filename_head = path;
    while (*filename_head)
    {
        if (*filename_head == '/')
            filename_start = filename_head;
        filename_head++;
    }
    if (filename_start != path)
        *filename_start = '\0';
    else
        *(filename_start + 1) = '\0';
    return (path);
}

void seq_file_file_size_string(char *size_str, uint32_t size)
{
    if (size < 1024)
        snprintf(size_str, 6, "%4uB", size);
    if (size > 1024 && size < 1024 * 1024)
        snprintf(size_str, 6, "%4uK", size / 1024);
    if (size > 1024 * 1024 && size < 1024 * 1024 * 1024)
        snprintf(size_str, 6, "%4uM", size / (1024 * 1024));
}
uint32_t seq_file_list_page(char *dir_name, uint8_t page, t_seq_file_entry *files, uint32_t max_files)
{
    
    seq_file_init_page(files);
    char entrypath[SEQ_CONFIG_MAX_FILE_NAME_SIZE];
    char mountpoint[SEQ_CONFIG_MAX_FILE_NAME_SIZE];
    char entrysize[16];
    const char *entrytype;

    struct dirent *entry;
    struct stat entry_stat;
    uint32_t files_listed;
    uint32_t files_shown;

    strlcpy(mountpoint, "/sd", SEQ_CONFIG_MAX_FILE_NAME_SIZE);
    strlcat(mountpoint, dir_name, SEQ_CONFIG_MAX_FILE_NAME_SIZE);
    DIR *dir = opendir(mountpoint);
    if (strlen(dir_name) > 1)
        strlcat(mountpoint, "/", SEQ_CONFIG_MAX_FILE_NAME_SIZE);
    const size_t dirpath_len = strlen(mountpoint);

    /* Retrieve the base path of file storage to construct the full path */
    strlcpy(entrypath, mountpoint, SEQ_CONFIG_MAX_FILE_NAME_SIZE);
    if (!dir)
    {
        return 0;
    }

    files_listed = files_shown = 0;
    /* Iterate over all files / folders and fetch their names and sizes */
    while ((entry = readdir(dir)) != NULL)
    {
        strlcpy(entrypath + dirpath_len, entry->d_name, sizeof(entrypath) - dirpath_len);

        files_listed++;
        if (files_listed > page * 6)
        {
            if (stat(entrypath, &entry_stat) == -1)
            {
                continue;
            }
            strlcpy(files[files_shown].file_name, entry->d_name, SEQ_CONFIG_MAX_FILE_NAME_SIZE);
            seq_file_file_size_string(files[files_shown].size_str, entry_stat.st_size);
            files[files_shown].file_size = entry_stat.st_size;
            files[files_shown].is_directory = (entry->d_type == DT_DIR);
            files_shown++;
        }
        if (files_shown >= 6)
            break;
    }
    closedir(dir);
    return files_shown;
}

uint32_t seq_file_list_page2(char *dir_name, uint8_t page, t_seq_file_entry *files, uint32_t max_files)
{
    return seq_file_list_page(dir_name, page, files, max_files);
    File dir = SD.open(dir_name);
    uint32_t file_count;
    const char *name_buffer;
    uint32_t files_shown;

    files_shown = 0;
    seq_file_init_page(files);
    file_count = 0;
    while (1)
    {
        File entry = dir.openNextFile();
        if (!entry)
            break;
        if (file_count >= (page + 1) * 6)
            break;
        if (file_count >= page * 6)
        {
            files[file_count - page * 6].is_directory = entry.isDirectory();
            name_buffer = seq_file_filename((char *)entry.name());
            strlcpy(files[file_count - page * 6].file_name, name_buffer, SEQ_CONFIG_MAX_FILE_NAME_SIZE);
            files_shown++;
            if (!files[file_count - page * 6].is_directory)
            {
                seq_file_file_size_string(files[file_count - page * 6].size_str, entry.size());
                files[file_count - page * 6].file_size = entry.size();
            }
            else
            {
                files[file_count - page * 6].size_str[0] = '\0';
            }
        }
        entry.close();
        file_count++;
    }
    dir.close();
    return (files_shown);
}

void seq_file_draw_status_bar(char *dirname, uint8_t page_idx, uint8_t total_pages)
{
    uint16_t h;
    uint16_t scroll_bar_height;
    uint16_t scroll_bar_start;

    scroll_bar_height = (uint16_t)(44. / total_pages);
    scroll_bar_start = 10 + (uint16_t)((44. / total_pages) * page_idx);
    if (page_idx == total_pages - 1)
        scroll_bar_height = 54 - scroll_bar_start;
    h = 62;
    u8g2.setColorIndex(1);
    u8g2.drawBox(0, 0, 128, 8);
    u8g2.setColorIndex(0);
    u8g2.drawBox(0, 8, 128, 1);
    u8g2.setColorIndex(1);
    u8g2.drawFrame(124, 9, 4, 46);
    u8g2.drawBox(125, scroll_bar_start, 2, scroll_bar_height);
    u8g2.drawBox(0, 56, 128, 8);
    u8g2.setColorIndex(0);
    u8g2.drawStr(2, 6, dirname);
    u8g2.setColorIndex(1);
}

void seq_gui_roll_text(char *dst, char *src, uint8_t size, uint8_t step)
{
    uint8_t src_length;
    char *src_start;

    src_start = src;
    src_length = strlen(src);
    if (src_length > size && step > 1 + src_length - size)
        step = 1 + src_length - size;
    if (src_length >= size)
        src_start = src + step;
    strlcpy(dst, src_start, size);
}

void seq_file_draw_file_names(t_seq_file_entry *files, uint8_t cursor, uint8_t step)
{
    uint8_t x;
    uint8_t y;
    uint8_t file_count;
    char name_buffer[25];

    file_count = 0;
    x = 0;
    y = 0;
    while (file_count < 6)
    {
        if (cursor == file_count)
            seq_gui_roll_text(name_buffer, files[file_count].file_name, 25, step);
        else
            strlcpy(name_buffer, files[file_count].file_name, 25);
        u8g2.drawStr(25 * 4 + 1 + x * 62, 15 + y * 7, files[file_count].size_str);
        u8g2.drawStr(1 + x * 62, 15 + y * 7, name_buffer);
        y++;
        file_count++;
    }
}

void seq_file_gui_cursor(uint16_t x, uint16_t y)
{
    u8g2.setDrawColor(2);

    if (y < 6)
    {
        if (x == 0)
            u8g2.drawBox(0, 9 + 7 * y, 123, 8);
    }

    u8g2.setDrawColor(1);
}

void seq_file_gui_control(char *dir_name, char *dst, uint32_t dst_size)
{
    t_seq_file_entry files[6];
    uint32_t total_files;
    uint8_t total_pages;
    uint32_t files_in_page;
    uint8_t page;
    t_seq_console *console;
    uint8_t input;
    uint8_t x, y;
    uint8_t steps;

    page = 0;
    x = y = 0;
    steps = 0;
    strlcpy(dst, dir_name, dst_size);
    console = seq_system.console;
    total_files = seq_file_count_files(dst);
    total_pages = total_files / 6;
    if (total_pages * 6 < total_files)
        total_pages++;
    files_in_page = seq_file_list_page(dst, page, files, total_files);
    while (1)
    {
        while (!console->input_buffer->isEmpty())
        {
            console->input_buffer->pop(&input);
            if (input == 27)
            {
                return;
            }
            else if (input == 18)
            {
                if (y == 0 && page > 0)
                {
                    files_in_page = seq_file_list_page(dst, --page, files, total_files);
                    y = 6;
                }
                if (y > 0)
                    y--;
                steps = 0;
            }
            else if (input == 19)
            {
                y++;
                if (y >= files_in_page)
                {
                    if (page + 1 < total_pages)
                    {
                        files_in_page = seq_file_list_page(dst, ++page, files, total_files);
                        y = 0;
                    }
                    else
                        y = files_in_page - 1;
                }
                steps = 0;
            }
            else if (input == '\n')
            {
                // strlcpy(dst, dir_name, dst_size);
                if (strlen(dst) > 1)
                    strlcat(dst, "/", dst_size);
                strlcat(dst, files[y].file_name, dst_size);
                if (!files[y].is_directory)
                    return;
                page = 0;
                x = y = 0;
                steps = 0;
                total_files = seq_file_count_files(dst);
                total_pages = (total_files) / 6;
                if (total_pages * 6 < total_files)
                    total_pages++;
                files_in_page = seq_file_list_page(dst, page, files, total_files);
            }

            else if (input == 28)
            {
                seq_file_folder_name(dst);
                page = 0;
                x = y = 0;
                steps = 0;
                total_files = seq_file_count_files(dst);
                total_pages = (total_files) / 6;
                if (total_pages * 6 < total_files)
                    total_pages++;
                files_in_page = seq_file_list_page(dst, page, files, total_files);
            }
            else if (input == 17)
            {
                if (page > 0)
                {
                    files_in_page = seq_file_list_page(dst, --page, files, total_files);
                    y = 0;
                }
            }
            else if (input == 20)
            {
                if (page < total_pages - 1)
                {
                    files_in_page = seq_file_list_page(dst, ++page, files, total_files);
                    y = 0;
                }
            }
        }
        u8g2.clearBuffer();
        seq_file_draw_status_bar(strlen(dst) < 32 ? dst : (dst + strlen(dst) - 32), page, total_pages);
        seq_file_draw_file_names(files, y, steps * 5);
        seq_file_gui_cursor(x, y);
        u8g2.sendBuffer();
        steps++;
        ulTaskNotifyTake(pdTRUE, 1000);
    }
}

BaseType_t seq_file_command(char *pcWriteBuffer, size_t xWriteBufferLen, const char *command_string)
{
    seq_file_gui_control("/", pcWriteBuffer, xWriteBufferLen);
    return pdFALSE;
}