////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RTC Config

#ifdef RTC_ENABLE
#    include "features/rtc/rtc.h"

void display_handler_rtc_year(char *text_buffer, size_t buffer_len) {
    if (rtc_is_connected()) {
        snprintf(text_buffer, buffer_len - 1, "%04d", rtc_read_time_struct().year);
    } else {
        snprintf(text_buffer, buffer_len - 1, "Not Connected");
    }
}

static bool menu_handler_rtc_year(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rtc_year_decrease();
            return false;
        case menu_input_right:
            rtc_year_increase();
            return false;
        default:
            return true;
    }
}

void display_handler_rtc_month(char *text_buffer, size_t buffer_len) {
    if (rtc_is_connected()) {
        snprintf(text_buffer, buffer_len - 1, "%02d", rtc_read_time_struct().month);
    } else {
        snprintf(text_buffer, buffer_len - 1, "Not Connected");
    }
}

static bool menu_handler_rtc_month(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rtc_month_decrease();
            return false;
        case menu_input_right:
            rtc_month_increase();
            return false;
        default:
            return true;
    }
}

void display_handler_rtc_date(char *text_buffer, size_t buffer_len) {
    if (rtc_is_connected()) {
        snprintf(text_buffer, buffer_len - 1, "%02d", rtc_read_time_struct().date);
    } else {
        snprintf(text_buffer, buffer_len - 1, "Not Connected");
    }
}

static bool menu_handler_rtc_date(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rtc_date_decrease();
            return false;
        case menu_input_right:
            rtc_date_increase();
            return false;
        default:
            return true;
    }
}

void display_handler_rtc_hour(char *text_buffer, size_t buffer_len) {
    if (rtc_is_connected()) {
        rtc_time_t time = rtc_read_time_struct();
        if (time.is_dst) {
            if (time.format == RTC_FORMAT_12H) {
                if (time.hour == 12) {
                    time.hour  = 1;
                    time.am_pm = time.am_pm == RTC_AM ? RTC_PM : RTC_AM;
                } else {
                    time.hour++;
                }
            } else {
                if (time.hour == 23) {
                    time.hour = 0;
                } else {
                    time.hour++;
                }
            }
        }
        snprintf(text_buffer, buffer_len - 1, "%02d", time.hour);
    } else {
        snprintf(text_buffer, buffer_len - 1, "Not Connected");
    }
}

static bool menu_handler_rtc_hour(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rtc_hour_decrease();
            return false;
        case menu_input_right:
            rtc_hour_increase();
            return false;
        default:
            return true;
    }
}

void display_handler_rtc_minute(char *text_buffer, size_t buffer_len) {
    if (rtc_is_connected()) {
        snprintf(text_buffer, buffer_len - 1, "%02d", rtc_read_time_struct().minute);
    } else {
        snprintf(text_buffer, buffer_len - 1, "Not Connected");
    }
}

static bool menu_handler_rtc_minute(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rtc_minute_decrease();
            return false;
        case menu_input_right:
            rtc_minute_increase();
            return false;
        default:
            return true;
    }
}

void display_handler_rtc_second(char *text_buffer, size_t buffer_len) {
    if (rtc_is_connected()) {
        snprintf(text_buffer, buffer_len - 1, "%02d", rtc_read_time_struct().second);
    } else {
        snprintf(text_buffer, buffer_len - 1, "Not Connected");
    }
}

static bool menu_handler_rtc_second(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rtc_second_decrease();
            return false;
        case menu_input_right:
            rtc_second_increase();
            return false;
        default:
            return true;
    }
}

void display_handler_rtc_hour_format(char *text_buffer, size_t buffer_len) {
    if (rtc_is_connected()) {
        snprintf(text_buffer, buffer_len - 1, "%s", rtc_read_time_struct().format == RTC_FORMAT_24H ? "24H" : "12H");
    } else {
        snprintf(text_buffer, buffer_len - 1, "Not Connected");
    }
}

static bool menu_handler_rtc_hour_format(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            rtc_format_toggle();
            return false;
        default:
            return true;
    }
}

void display_handler_rtc_dst(char *text_buffer, size_t buffer_len) {
    if (rtc_is_connected()) {
        snprintf(text_buffer, buffer_len - 1, "%s", rtc_read_time_struct().is_dst ? "on" : "off");
    } else {
        snprintf(text_buffer, buffer_len - 1, "Not Connected");
    }
}

static bool menu_handler_rtc_dst(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            rtc_dst_toggle();
            return false;
        default:
            return true;
    }
}

menu_entry_t rtc_config_entries[] = {
    MENU_ENTRY_CHILD("Year", rtc_year),
    MENU_ENTRY_CHILD("Month", rtc_month),
    MENU_ENTRY_CHILD("Date", rtc_date),
    MENU_ENTRY_CHILD("Hour", rtc_hour),
    MENU_ENTRY_CHILD("Minute", rtc_minute),
    MENU_ENTRY_CHILD("Second", rtc_second),
    MENU_ENTRY_CHILD("12/24 Hour Format", rtc_hour_format),
    MENU_ENTRY_CHILD("DST", rtc_dst),
};
#endif // RTC_ENABLE
