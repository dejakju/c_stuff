/*
 * Name:    dejakju@gmail.com
 * Date:    2023-08-03
 * Update:  2025-10-11
 * Desc.:   Display current month as a calendar
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define DATETIME_VERSION 1
#define DATETIME_REVISION 6

bool is_leap_year(int year) {
    if (year % 4 != 0) return false;
    else if (year % 100 != 0) return true;
    else if (year % 400 != 0) return false;
    else return true;
}

int main(int argc, char* argv[])
{

    if (argc > 1) {
        if ((argv[1][0] == '-' && argv[1][1] == '?') || (argv[1][0] == '-' && argv[1][1] == 'H') || (argv[1][0] == '-' && argv[1][1] == 'h')) {
            printf("\x1b[1;36m%s\x1b[0m %d.%d (compiled %s, %s)\n", argv[0], DATETIME_VERSION, DATETIME_REVISION, __TIME__, __DATE__);
            printf("\nUsage:\t%s [-option]\n", argv[0]);
            printf("\n\t-H, -h, -?  .... show this help");
            printf("\n\t-V, -v      .... show version information");
            printf("\n");
            exit(0);
        }
    }

    if (argc > 1) {
        if ((argv[1][0] == '-' && argv[1][1] == 'V') || (argv[1][0] == '-' && argv[1][1] == 'v')) {
            printf("\x1b[1;36m%s\x1b[0m %d.%d (compiled %s, %s)\n", argv[0], DATETIME_VERSION, DATETIME_REVISION, __TIME__, __DATE__);
            exit(0);
        }
    }

    char *month_names[] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
    int month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    time_t now = time(0);
    struct tm *datetime = localtime(&now);
   
    int wday = datetime->tm_wday;
    int mday = datetime->tm_mday;
    int yday = datetime->tm_yday;
    int dd = 0, dm = 0, dy = 0;

    char *wday_names[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    int wday_start = 0;

    if (argc > 1) {
        if ((argv[1][0] == '-' && argv[1][1] == 'W') || (argv[1][0] == '-' && argv[1][1] == 'w')) {
            if (argc > 2) {
                if ((atoi(argv[2]) >= 0) && (atoi(argv[2]) <= 6)) {
                    wday_start = atoi(argv[2]);
                }
            }
        }
    }

    struct tm *modified_time = localtime(&now);

    modified_time->tm_mday = (dd != 0) ? (dd) : 1;
    modified_time->tm_mon = (dm != 0) ? (dm - 1) : modified_time->tm_mon;
    modified_time->tm_year = (dy != 0) ? (dy - 1900) : modified_time->tm_year;

    int month = modified_time->tm_mon;
    int year = modified_time->tm_year + 1900;

    int last_day_of_month = month_days[month];
    last_day_of_month = (last_day_of_month == 28 && is_leap_year(year)) ? last_day_of_month + 1 : last_day_of_month;
    time_t first_of_current_month = mktime(modified_time);
    struct tm *first_of_the_month = localtime(&first_of_current_month);

    int max_rows = 5;
    if ((first_of_the_month->tm_mday && first_of_the_month->tm_wday == 5 && last_day_of_month == 31) || (first_of_the_month->tm_mday && first_of_the_month->tm_wday == 6 && last_day_of_month == 31)) {
        ++max_rows;
    } else if (first_of_the_month->tm_mday == 1 && first_of_the_month->tm_wday == 0 && last_day_of_month == 28) {
        --max_rows;
    }

    int start_col = first_of_the_month->tm_wday;
    int num_of_days = last_day_of_month;
    start_col += wday_start;

    printf("\x1b[1m%28s %d\x1b[0m\n", month_names[month], year);
    printf(".-------------------------------------------------------.\n");
    for (int i = wday_start; i < 7; ++i) {
        printf("|  %s  ", wday_names[i]);
    }
    if (wday_start > 0) {
        for (int i = 0; i < wday_start; ++i) {
            printf("|  %s  ", wday_names[i]);
        }
    }
    printf("|\n");
    printf("|-------+-------+-------+-------+-------+-------+-------|\n");

    for (int row = 0; row < max_rows; ++row) {
        for (int col = 0; col < 7; ++col) {
            int index = col + (row * 7) - start_col + 1;
            if (index <= 0) {
                printf("|  \x1b[32m%4c\x1b[0m ", ' ');
                continue;
            }

            if (index == mday) {
                printf("| >\x1b[32m%4d\x1b[0m ", index);
                continue;
            }

            if (index > last_day_of_month) {
                for (int i = 0; i < (7 - col); ++i) {
                    printf("|%7s", " ");
                }
                break;
            }

            if (row == 0 && col < first_of_the_month->tm_wday) {
                printf("|%7s ", " ");
            } else {
                printf("|%6d ", index);
            }
        }
        printf("|\n");
    }
    printf("+-------------------------------------------------------+\n");

    return EXIT_SUCCESS;
}
