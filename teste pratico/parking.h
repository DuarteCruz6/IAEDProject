#ifndef PARKING_H
#define PARKING_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFSIZ 1024

typedef struct CarStruct{
    char park_name[BUFSIZ];
    char plate[BUFSIZ];
    char entrydate[BUFSIZ];
    char entrytime[BUFSIZ];
    char leftdate[BUFSIZ];
    char lefttime[BUFSIZ];
    int value;
    char (*logs)[6][BUFSIZ];
    int logsnumber;
    int flag;
}CarStruct;

typedef struct ParkStruct{                       //ordered alphabetically
    char park_name[BUFSIZ];
    int capacity;
    float value_before1h;
    float value_after1h;
    float max_value;
    int number_parked;
    char already_parked[BUFSIZ];
    float bill_per_day[BUFSIZ];
    char *dates[BUFSIZ];
    int number_of_dates;
    float totalbill;
}ParkStruct;

typedef struct Park_PStruct{                     //chronologically ordered
    char park_name[BUFSIZ];
    int capacity;
    int number_parked;
}Park_PStruct;

struct DateTime {
    int tm_year;
    int tm_mon;
    int tm_mday;
    int tm_hour;
    int tm_min;
};

int count_cars();
int count_parks();
void list_parks();
int check_if_park_exists(const char *new_park_name, int number_of_parks);
int checks_create_park(char *park_name,int number_of_parks, int capacity,float value_before1h, float value_after1h,float max_value);
void order_alphabetically(int number_of_parks, char *park_name, int capacity, float value_before1h, float value_after1h, float max_value);
void create_park(char *rest_command);

int check_format_plate(char *plate);
int check_quantity_plate(int letters, int numbers);
int invalid_plate(char *plate_to_check);

int already_parked(char *plate);

int daysInMonth(int month);
int check_date_and_global(int year, int month, int day, int hour, int minute);
int invalid_date(char *date, char *time);

int check_capacity(char *park_name, int num_parks);

int already_exists(char *plate);

int invalid_exit(const char *park_name, const char *plate);

void add_log(struct CarStruct *car, char log[6][BUFSIZ]);

void add_value_to_day(int park_index,char *leftdate,float value);

int differenceminutes(struct DateTime dt, struct DateTime df);

int checks_new_vehicle(char *park_name,int number_of_parks,char *plate_to_check,char *plate,char *entrydate,char *entrytime);
void addcar(struct CarStruct new_car,int number_of_cars);
void addplatetopark(char *park_name,char *plate,int number_of_parks);
void new_vehicle(char *rest_command);

int checks_old_vehicle(char *park_name,int number_of_parks,char *plate,char *plate_to_check,char *leftdate,char*lefttime);
float calculatebill(int time,float max_value,float value_after1h,float value_before1h);
void old_vehicle(char *rest_command);

int checked_parked_logs(char *licenceplate);

int checks_for_check_vehicle(char *licenceplate);
void check_vehicle(char *rest_command);

void free_car_logs(struct CarStruct *car);

void list_bill_day(char *park_name);
void list_bill_specific_date(char *date, char *park_name);
void bill_day(char *rest_command);

void remove_park_from_struct_Park(int park_index);
void remove_park_from_struct_Park_P(int park_index);
void remove_cars(int number_of_cars,char *park_name);
void remove_park(char *rest_command);

void bill_park(char *rest_command);

void free_all_parks();
void free_all_cars();

#endif /* PARKING_H */
