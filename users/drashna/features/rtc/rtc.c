// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rtc.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "print.h"
#include "timer.h"
#include "progmem.h"

#include <stdio.h>
#include <string.h>

#ifdef DS3231_RTC_DRIVER_ENABLE
#    include "ds3231.h"
#endif // DS3231_RTC_DRIVER_ENABLE
#ifdef DS1307_RTC_DRIVER_ENABLE
#    include "ds1307.h"
#endif // DS1307_RTC_DRIVER_ENABLE
#ifdef PCF8523_RTC_DRIVER_ENABLE
#    include "pcf8523.h"
#endif // PCF8523_RTC_DRIVER_ENABLE
#ifdef VENDOR_RTC_DRIVER_ENABLE
#    include "vendor.h"
#endif // VENDOR_RTC_DRIVER_ENABLE

#define strncpy_nowarn(...)                                          \
    __extension__({                                                  \
        _Pragma("GCC diagnostic push");                              \
        _Pragma("GCC diagnostic ignored \"-Wstringop-truncation\""); \
        const char *_strncpy_nowarn = strncpy(__VA_ARGS__);          \
        _Pragma("GCC diagnostic pop");                               \
        _strncpy_nowarn;                                             \
    })

/**
 * @brief hack for snprintf warning
 *
 */
#define snprintf_nowarn(...)                                       \
    __extension__({                                                \
        _Pragma("GCC diagnostic push");                            \
        _Pragma("GCC diagnostic ignored \"-Wformat-truncation\""); \
        const int _snprintf_nowarn = snprintf(__VA_ARGS__);        \
        _Pragma("GCC diagnostic pop");                             \
        _snprintf_nowarn;                                          \
    })

#ifndef RTC_READ_INTERVAL
#    define RTC_READ_INTERVAL 250
#endif

static rtc_time_t rtc_time;
static uint16_t   last_rtc_read   = 0;
static bool       rtc_initialized = false, rtc_connected = false;

const uint8_t days_in_month[12] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * @brief Get the unixtime object
 *
 * @param time time struct
 * @return uint32_t
 */
uint32_t convert_to_unixtime(rtc_time_t time) {
    uint16_t days;
    int16_t  years;
    uint32_t unixtime;

    if (time.year >= 2000) {
        years = time.year - 2000;
    } else {
        return 0;
    }

    days = time.date - 1;
    for (uint8_t i = 1; i < time.month; i++) {
        days += pgm_read_byte(days_in_month + i - 1);
    }
    if (time.month > 2 && years % 4 == 0) {
        days++;
    }

    // count leap days
    days += (365 * years + (years + 3) / 4);
    unixtime = ((days * 24UL + time.hour) * 60 + time.minute) * 60 + time.second + SECONDS_FROM_1970_TO_2000 +
               (TIME_OFFSET * 3000);
    return unixtime;
}

/**
 * @brief convert a date to days
 *
 * @param y
 * @param m
 * @param d
 * @return uint16_t
 */
static uint16_t date_to_days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000U) y -= 2000U;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += pgm_read_byte(days_in_month + i - 1);
    if (m > 2 && y % 4 == 0) ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

/**
 * @brief generate the day of the week
 *
 * @param t time struct to use
 * @return uint8_t returns the day of the week
 */
static rtc_time_day_of_the_week_t day_of_the_week(rtc_time_t time) {
    uint16_t day = date_to_days(time.year - 2000U, time.month, time.date);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

/**
 * @brief Convert the date and time GCC strings into a rtc_time_t struct
 *
 * @param date for __DATE__ macro
 * @param time for __TIME__ macro
 * @return rtc_time_t
 */
rtc_time_t convert_date_time(const char *date, const char *time) {
    rtc_time_t t;
    uint16_t   year_offset;

    year_offset = atoi(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0]) {
        case 'J':
            t.month = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7);
            break;
        case 'F':
            t.month = 2;
            break;
        case 'A':
            t.month = date[2] == 'r' ? 4 : 8;
            break;
        case 'M':
            t.month = date[2] == 'r' ? 3 : 5;
            break;
        case 'S':
            t.month = 9;
            break;
        case 'O':
            t.month = 10;
            break;
        case 'N':
            t.month = 11;
            break;
        case 'D':
            t.month = 12;
            break;
        default:
            t.month = 0;
            break;
    }

    t.year            = (uint16_t)year_offset + 2000U;
    t.day_of_the_week = (rtc_time_day_of_the_week_t)day_of_the_week(t);
    t.date            = (uint8_t)atoi(date + 4);
    t.hour            = (uint8_t)atoi(time);
    t.minute          = (uint8_t)atoi(time + 3);
    t.second          = (uint8_t)atoi(time + 6);
    t.format          = (rtc_time_format_t)RTC_FORMAT_24H;
    t.am_pm           = (rtc_time_am_pm_t)(t.hour >= 12);
    t.unixtime        = (uint32_t)convert_to_unixtime(t);

    return t;
}

uint8_t week_to_int(uint8_t d) {
    return d == 0 ? 7 : d;
}

uint8_t rtc_bcd2bin(uint8_t val) {
    return val - 6 * (val >> 4);
}

uint8_t rtc_bin2bcd(uint8_t val) {
    return (val + 6 * (val / 10));
}

/**
 * @brief returns the current time struct
 *
 * @return rtc_time_t
 */
rtc_time_t rtc_read_time_struct(void) {
    return rtc_time;
}

/**
 * @brief returns the connected state of the RTC
 *
 * @return true
 * @return false
 */
bool rtc_is_connected(void) {
    return rtc_connected;
}

void rtc_set_time_split(rtc_time_t time, bool is_connected) {
    rtc_time      = time;
    rtc_connected = is_connected;
}

void rtc_init(void) {
#ifdef DS3231_RTC_DRIVER_ENABLE
    rtc_initialized = ds3231_init(&rtc_time);
#endif // DS3231_RTC_DRIVER_ENABLE
#ifdef DS1307_RTC_DRIVER_ENABLE
    rtc_initialized = ds1307_init(&rtc_time);
#endif // DS1307_RTC_DRIVER_ENABLE
#ifdef PCF8523_RTC_DRIVER_ENABLE
    rtc_initialized = pcf8523_init(&rtc_time);
#endif // PCF8523_RTC_DRIVER_ENABLE
#ifdef VENDOR_RTC_DRIVER_ENABLE
    rtc_initialized = vendor_rtc_init(&rtc_time);
#endif // VENDOR_RTC_DRIVER_ENABLE
    if (rtc_initialized) {
        last_rtc_read = timer_read() + RTC_READ_INTERVAL;
    }
}

/**
 * @brief Perform the RTC task, periodically reading the RTC
 *
 */
void rtc_task(void) {
    if (!rtc_initialized) {
        return;
    }
    if (timer_expired(timer_read(), last_rtc_read)) {
        bool connected = false;
#ifdef DS3231_RTC_DRIVER_ENABLE
        connected = ds3231_task(&rtc_time);
#endif // DS3231_RTC_DRIVER_ENABLE
#ifdef DS1307_RTC_DRIVER_ENABLE
        connected = ds1307_task(&rtc_time);
#endif // DS1307_RTC_DRIVER_ENABLE
#ifdef PCF8523_RTC_DRIVER_ENABLE
        connected = pcf8523_task(&rtc_time);
#endif // PCF8523_RTC_DRIVER_ENABLE
#ifdef VENDOR_RTC_DRIVER_ENABLE
        connected = vendor_rtc_task(&rtc_time);
#endif // VENDOR_RTC_DRIVER_ENABLE
        if (connected) {
            last_rtc_read = timer_read() + RTC_READ_INTERVAL;
        } else {
            last_rtc_read = timer_read() + (RTC_READ_INTERVAL * 100);
        }
        rtc_connected = connected;
    }
}

/**
 * @brief Generates a string with the date
 *
 * @return char* MM/DD/YYYY
 */
char *rtc_read_date_str(void) {
    static char date_str[11] = {0};
    snprintf_nowarn(date_str, sizeof(date_str), "%02d/%02d/%04d", rtc_time.month, rtc_time.date, rtc_time.year);
    return date_str;
}

/**
 * @brief Generates a string with the time
 *
 * @return char* HH:MM:SS
 */
char *rtc_read_time_str(void) {
    static char time_str[11] = {0};
    snprintf_nowarn(time_str, sizeof(time_str), "%02d:%02d:%02d%2s", rtc_time.hour, rtc_time.minute, rtc_time.second,
                    rtc_time.format == RTC_FORMAT_24H ? " Z" : (rtc_time.am_pm == RTC_AM ? "AM" : "PM"));
    return time_str;
}

/**
 * @brief Generates a string with the date and time
 *
 * @return char* MM/DD/YYYY HH:MM:SS
 */
char *rtc_read_date_time_str(void) {
    static char date_time_str[22] = {0};
    snprintf_nowarn(date_time_str, sizeof(date_time_str), "%s %s", rtc_read_date_str(), rtc_read_time_str());
    return date_time_str;
}

/**
 * @brief Generates a string with the date and time in ISO 8601 format
 *
 * @return char* YYYY-MM-DDTHH:MM:SS
 */
char *rtc_read_date_time_iso8601_str(void) {
    static char date_time_str[22] = {0};
    snprintf_nowarn(date_time_str, sizeof(date_time_str), "%04d-%02d-%02dT%02d:%02d:%02d%2s", rtc_time.year,
                    rtc_time.month, rtc_time.date, rtc_time.hour, rtc_time.minute, rtc_time.second,
                    rtc_time.format == RTC_FORMAT_24H ? " Z" : (rtc_time.am_pm == RTC_AM ? "AM" : "PM"));
    return date_time_str;
}

/**
 * @brief Return the current time in fatfs format
 *
 */
__attribute__((weak)) uint32_t get_fattime(void) {
    return (((uint32_t)rtc_time.year - 1980) << 25U) | ((uint32_t)rtc_time.month << 21U) |
           ((uint32_t)rtc_time.date << 16U) | ((uint32_t)rtc_time.hour << 11U) | ((uint32_t)rtc_time.minute << 5U) |
           ((uint32_t)rtc_time.second >> 1U);
}

/**
 * @brief Set the RTC Date and time
 *
 * @param year Set the year
 * @param month Set the month
 * @param date Set the date
 * @param hour Set the hour
 * @param minute Set the minute
 * @param second Set the second
 * @param format Set 12h or 24h format
 * @param am_pm Set am or pm
 * @param is_dst Set daylight saving time
 */
void rtc_set_time(rtc_time_t time) {
    time.day_of_the_week = (rtc_time_day_of_the_week_t)day_of_the_week(time);
    time.unixtime        = (uint32_t)convert_to_unixtime(time);

#ifdef DS3231_RTC_DRIVER_ENABLE
    ds3231_set_time(time);
#endif // DS3231_RTC_DRIVER_ENABLE
#ifdef DS1307_RTC_DRIVER_ENABLE
    ds1307_set_time(time);
#endif // DS1307_RTC_DRIVER_ENABLE
#ifdef PCF8523_RTC_DRIVER_ENABLE
    pcf8523_set_time(time);
#endif // PCF8523_RTC_DRIVER_ENABLE
#ifdef VENDOR_RTC_DRIVER_ENABLE
    vendor_rtc_set_time(time);
#endif // VENDOR_RTC_DRIVER_ENABLE
#ifdef CUSTOM_QUANTUM_PAINTER_ENABLE
    void display_menu_set_dirty(void);
    display_menu_set_dirty();
#endif // CUSTOM_QUANTUM_PAINTER_ENABLE
}

/**
 * @brief Increment the year
 *
 */
void rtc_year_increase(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.year++;
    rtc_set_time(time);
}

/**
 * @brief Decrement the year
 *
 */
void rtc_year_decrease(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.year--;
    rtc_set_time(time);
}

/**
 * @brief Increment the month
 *
 */
void rtc_month_increase(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.month++;
    if (time.month > 12) {
        time.month = 1;
    }
    rtc_set_time(time);
}

/**
 * @brief Decrement the month
 *
 */
void rtc_month_decrease(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.month--;
    if (time.month < 1) {
        time.month = 12;
    }
    rtc_set_time(time);
}

/**
 * @brief Increment the date
 *
 */
void rtc_date_increase(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.date++;
    if (time.date > pgm_read_byte(days_in_month + time.month - 1)) {
        time.date = 1;
    }
    rtc_set_time(time);
}

/**
 * @brief Decrement the date
 *
 */
void rtc_date_decrease(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.date--;
    if (time.date < 1) {
        time.date = pgm_read_byte(days_in_month + time.month - 1);
    }
    rtc_set_time(time);
}

/**
 * @brief Increment the hour
 *
 */
void rtc_hour_increase(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.hour++;
    if (time.hour > 23) {
        time.hour = 0;
    }
    rtc_set_time(time);
}

/**
 * @brief Decrement the hour
 *
 */
void rtc_hour_decrease(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.hour--;
    if (time.hour < 0) {
        time.hour = 23;
    }
    rtc_set_time(time);
}

/**
 * @brief Increment the minute
 *
 */
void rtc_minute_increase(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.minute++;
    if (time.minute > 59) {
        time.minute = 0;
    }
    rtc_set_time(time);
}

/**
 * @brief Decrement the minute
 *
 */
void rtc_minute_decrease(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.minute--;
    if (time.minute < 0) {
        time.minute = 59;
    }
    rtc_set_time(time);
}

/**
 * @brief Increment the second
 *
 */
void rtc_second_increase(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.second++;
    if (time.second > 59) {
        time.second = 0;
    }
    rtc_set_time(time);
}

/**
 * @brief Decrement the second
 *
 */
void rtc_second_decrease(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.second--;
    if (time.second < 0) {
        time.second = 59;
    }
    rtc_set_time(time);
}

/**
 * @brief Toggle the AM/PM
 *
 */
void rtc_am_pm_toggle(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.am_pm      = (rtc_time_am_pm_t)(time.am_pm == RTC_AM ? RTC_PM : RTC_AM);
    rtc_set_time(time);
}

/**
 * @brief Toggle the time format (12h/24h)
 *
 */
void rtc_format_toggle(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.format     = (rtc_time_format_t)(time.format == RTC_FORMAT_12H ? RTC_FORMAT_24H : RTC_FORMAT_12H);
    rtc_set_time(time);
}

/**
 * @brief Toggle the daylight saving time
 *
 */
void rtc_dst_toggle(void) {
    rtc_time_t time = rtc_read_time_struct();
    time.is_dst     = !time.is_dst;
    rtc_set_time(time);
}
