/*
    $VER:       datetime.c 1.8
    $DATE:      2023-08-03 (2025-11-15)
    $AUTHOR:    Goran (dejakju@gmail.com)

** Description: Display current month as a calendar
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define DATETIME_VERSION    1
#define DATETIME_REVISION   8

bool
is_leap_year(int year) {
    if (year % 4 != 0) return false;
    else if (year % 100 != 0) return true;
    else if (year % 400 != 0) return false;
    else return true;
}

void
show_usage(const char *argv0)
{
    printf("Usage: ");
    printf("%s [opt]", argv0);
    printf("\n\t -H, -h, -? \t.......... show this help");
    printf("\n\t -V, -v     \t.......... show version information");
    printf("\n");
    exit(1);
}

int
main(int argc, char* argv[])
{

    if (argc > 1) {
        if ((argv[1][0] == '/' && argv[1][1] == '?') || 
            (argv[1][0] == '-' && argv[1][1] == '?') || 
            (argv[1][0] == '-' && argv[1][1] == 'H') || 
            (argv[1][0] == '-' && argv[1][1] == 'h')) {
            show_usage(argv[0]);
        }
    }

    if (argc > 1) {
        if ((argv[1][0] == '-' && argv[1][1] == 'V') || (argv[1][0] == '-' && argv[1][1] == 'v')) {
            printf("\x1b[1;36m%s\x1b[0m %d.%d (compiled %s, %s)\n", argv[0], DATETIME_VERSION, DATETIME_REVISION, __TIME__, __DATE__);
            exit(0);
        }
    }

    int month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    char *month_names[] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
    char *weekday_names[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    
    time_t now = time(0);
    struct tm *datetime = localtime(&now);

    int current_hour = datetime->tm_hour;
    int current_minute = datetime->tm_min;
    int current_second = datetime->tm_sec;

    int current_day = datetime->tm_mday; // day of the month
    int yday = datetime->tm_yday; // days since January 1

    struct tm *modified_time = localtime(&now);
    // years since 1900
    int year = modified_time->tm_year + 1900;
    // account for leap years in months since January
    int month = modified_time->tm_mon;
    int last_day_of_the_month = month_days[month];
    last_day_of_the_month = (last_day_of_the_month == 28 && is_leap_year(year)) ? last_day_of_the_month + 1 : last_day_of_the_month;
    // set the date to the first of the current month
    modified_time->tm_mday = 1;


    time_t first_of_current_month = mktime(modified_time);
    struct tm *first_of_the_month = localtime(&first_of_current_month);

    // assign the true (real) start of the weekday of the 1st day of the current month
    int real_weekday_start = first_of_the_month->tm_wday;

    // calculate the number of the rows for the current month, so it's either 4, 5 or 6 - starting with 5 - which is by far
    // the most common variation
    int max_rows = 5;
    // only in months with 30 or 31 days AND where the weekday is either Fri or Sat, than the table must expand to 6 rows to fit the data
    if ((first_of_the_month->tm_mday == 1 && first_of_the_month->tm_wday == 5 && last_day_of_the_month == 30) ||
        (first_of_the_month->tm_mday == 1 && first_of_the_month->tm_wday == 6 && last_day_of_the_month == 30) ||
        (first_of_the_month->tm_mday == 1 && first_of_the_month->tm_wday == 5 && last_day_of_the_month == 31) ||
        (first_of_the_month->tm_mday == 1 && first_of_the_month->tm_wday == 6 && last_day_of_the_month == 31)) {
        ++max_rows;
    // if - and only if - the 1st of february starts on a Sun, than just 4 rows are ever enough
    } else if (first_of_the_month->tm_mday == 1 && first_of_the_month->tm_wday == 0 && last_day_of_the_month == 28) {
        --max_rows;
    }

    printf("\x1b[1m%28s %d\x1b[0m\t %02d:%02d:%02d\n", month_names[month], year, current_hour, current_minute, current_second);
    printf(".-------------------------------------------------------.\n");
    for (int i = 0; i < 7; ++i) printf("|  %s  ", weekday_names[i]);
    printf("|\n");
    printf("|-------+-------+-------+-------+-------+-------+-------|\n");

    // print the date table
    for (int row = 0; row < max_rows; ++row) {
        for (int col = 0; col < 7; ++col) {

            // index is shifted by +1 for display reasons as the first day is 1 and not 0
            // then it's adjusted for the true weekday (-real_weekday_start), so the index-ing begins at that point =)
            int index = col + (row * 7) + 1 - real_weekday_start;

            if (index <= 0) {
                printf("|  \x1b[32m%4c\x1b[0m ", ' ');
                continue;
            }

            if (index == current_day) {
                printf("| >\x1b[32m%4d\x1b[0m ", index);
                continue;
            }

            if (index > last_day_of_the_month) {
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
    printf("+-------------------------------------------------------+");

    return EXIT_SUCCESS;
}
