#include "parking.h"

int MAX_PARKS_NUMBER=20;
int globalday=1; 
int globalmonth=1;
int globalyear=2015;
int globalhour=0;
int globalminute=0;

CarStruct cars[BUFSIZ];
ParkStruct parks[BUFSIZ];
Park_PStruct parks_p[BUFSIZ];

//returns the number of cars
int count_cars() {
    int count=0;
    for (int i = 0; i < BUFSIZ; i++) {
        if (strlen(cars[i].park_name)>0){
            count+=1;
        }
        else{
            break;
        }
    }
    return count;
}

//returns the number of parks
int count_parks() {
    int count=0;
    for (int i = 0; i < MAX_PARKS_NUMBER; i++) {
        if (strlen(parks[i].park_name)>0){
            count+=1;
        }
        else{
            break;
        }
    }
    return count;   
}

//prints all the park in the format [name] [capacity] [number of available spots]
void list_parks(){
    int num=count_parks();
    for (int i = 0; i < num; i++) {
        if(strcmp(parks_p[i].park_name," ")!=0){
            printf("%s %d %d\n", parks_p[i].park_name, parks_p[i].capacity, parks_p[i].capacity-parks_p[i].number_parked);
        }
    }   
}                  

//returns 1 if park exists, otherwise returns 0
int check_if_park_exists(const char *new_park_name, int number_of_parks){
    for(int i=0;i<number_of_parks;i++){
        if (strcmp(parks[i].park_name,new_park_name) == 0) {
            return 1; 
        }
    }
    return 0; // Park does not exist
}

int checks_create_park(char *park_name,int number_of_parks, int capacity,float value_before1h, float value_after1h,float max_value){
    if(check_if_park_exists(park_name,number_of_parks)==1){
        printf("%s: parking already exists.\n",park_name);
        return 1;
    }
    else if(capacity<=0){
        printf("%d: invalid capacity.\n",capacity);
        return 1;
    }
    else if(value_before1h>=value_after1h||value_after1h<=0||value_before1h<=0||max_value<=0||
    max_value<=value_after1h||max_value<=value_before1h){
        printf("invalid cost.\n");
        return 1;
    }
    else if(number_of_parks==MAX_PARKS_NUMBER){
        printf("too many parks.\n");
        return 1;
    }
    return 0;
}

void order_alphabetically(int number_of_parks, char *park_name, int capacity, float value_before1h, float value_after1h, float max_value){
    int index_alphabetic=number_of_parks;
    for(int i=0;i<number_of_parks;i++){                                     //order the parks alphabetically
        int result = strcmp(parks[i].park_name, park_name);            
        if(result>0){                                                           
            for(int a=number_of_parks-1;a>=i;a--){                              
                parks[a+1]=parks[a];                                            
            }
            index_alphabetic=i;
            break;
        }
    }
    strcpy(parks[index_alphabetic].park_name, park_name);
    parks[index_alphabetic].capacity = capacity;
    parks[index_alphabetic].value_before1h = value_before1h;
    parks[index_alphabetic].value_after1h = value_after1h;
    parks[index_alphabetic].max_value = max_value;
    parks[index_alphabetic].number_of_dates = 0;
    strcpy(parks_p[number_of_parks].park_name, park_name);
    parks_p[number_of_parks].capacity = capacity;
}

//creates a park
void create_park(char *rest_command) {
    char name[BUFSIZ]="";
    if (strcmp(rest_command,"")==0 || rest_command[0] == '0') {
        list_parks();
    } else{
        ParkStruct new_park;
        if (strchr(rest_command,'"') != NULL){ 
            for(int i=2;rest_command[i]!='\0';i++){ //the index from the first " is 1, so we start from index 2 until the last "
                if(rest_command[i]=='"'){
                    memmove(rest_command,rest_command+i+1,strlen(rest_command)-i); //when we get the index of the last ", we have the full name of the park, so we break the loop
                    break;
                }
                else{
                    strncat(name, &rest_command[i], 1); //while we haven't found the index from the last ", we are still looking at characters from the park's name, so we keep on adding them
                }
            }
            strcpy(new_park.park_name,name);       // add all the characters checked from the loop (except the ") to the name
            sscanf(rest_command, " %d %f %f %f", &new_park.capacity, // the rest of the characters are from other things such as the capacity, the values before and after 1 hour, and the maximum diary value
            &new_park.value_before1h, &new_park.value_after1h, &new_park.max_value);
        }     
        else{
            sscanf(rest_command, "%s %d %f %f %f", new_park.park_name, &new_park.capacity, 
            &new_park.value_before1h, &new_park.value_after1h, &new_park.max_value);
        }
        strcpy(new_park.already_parked,"");
        new_park.number_parked=0;
        int number_of_parks=count_parks();
        if(checks_create_park(new_park.park_name,number_of_parks,new_park.capacity,new_park.value_before1h,
        new_park.value_after1h,new_park.max_value)==1){
            return;
        }
        else{
            order_alphabetically(number_of_parks,new_park.park_name,new_park.capacity,
            new_park.value_before1h,new_park.value_after1h,new_park.max_value);
        }
    }
}

int check_format_plate(char *plate){
    if(strlen(plate)!=8){
        return 1;
    }
    if(plate[2]!='-'||plate[5] != '-'){
        return 1;
    } 
    return 0;
}

int check_quantity_plate(int letters, int numbers){
    if((letters==2&&numbers==1)||(numbers==2&&letters==1)){
        return 0;
    }
    else{
        return 1;
    }
}

//returns 1 if the plate is invalid, otherwise returns 0
int invalid_plate(char *plate_to_check){
    char *token;
    char plate[strlen(plate_to_check)+1];
    strcpy(plate,plate_to_check);
    int numbers=0,letters=0;
    if (check_format_plate(plate)==1){
        return 1;
    }
    token = strtok(plate, "-");
    while (token != NULL) {
        if(strlen(token)!=2){
            return 1;
        }
        if(isdigit(token[0])&&isdigit(token[1])){
            numbers+=1;
        }    
        else if(isalpha(token[0])&&isalpha(token[1])&&isupper(token[0])&&isupper(token[1])){
            letters+=1;
        }
        else{
            return 1;
        }
        token = strtok(NULL, "-");
    }
    if((check_quantity_plate(letters,numbers)==1)){
        return 1;
    }
    return 0;
}

//returns 1 if the car is already parked in some park, otherwise returns 0
int already_parked(char *plate){
    int num_parks = count_parks();
    for (int i=0;i<num_parks;i++){
        for(int checked=0;checked<parks[i].number_parked;checked++){
            char *token;
            char already_parked[BUFSIZ];
            strcpy(already_parked,parks[i].already_parked);
            token = strtok(already_parked, " ");
            while (token != NULL) {
                if(strcmp(plate,token)==0){
                    return 1;
                }
                token = strtok(NULL, " ");
            }
        }
    }
    return 0;
}

//returns the days in the month
int daysInMonth(int month) {
    const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return days[month];
}

int check_date_and_global(int year, int month, int day, int hour, int minute){
    if (year < globalyear ||
        (year == globalyear && month < globalmonth) ||
        (year == globalyear && month == globalmonth && day < globalday) ||
        (year == globalyear && month == globalmonth && day == globalday && hour < globalhour) ||
        (year == globalyear && month == globalmonth && day == globalday && hour == globalhour && minute < globalminute)) {
        return 1;    
    } else {
        return 0;
    }
}

//returns 1 if the date is invalid, otherwise returns 0
int invalid_date(char *date,char *time){
    if (strlen(date)!=10||strlen(time)<4||strlen(time)>5){
        return 1;
    }
    int day,month,year;
    sscanf(date,"%d-%d-%d",&day,&month,&year);
    if(daysInMonth(month-1)<day||day<1||month>12||month<1){
        return 1;
    }
    int hour,minute;  
    sscanf(time,"%d:%d",&hour,&minute);
    if(hour>23||minute>59){
        return 1;
    }
    if (check_date_and_global(year, month, day, hour, minute)==1){
        return 1;
    }
    else{
        return 0;
    }
}

//returns the 1 if the park is full, otherwise returns 0
int check_capacity(char *park_name, int num_parks){
    for(int i=0;i<num_parks;i++){
        if (strcmp(parks[i].park_name,park_name) == 0) {
            if (parks[i].capacity==parks[i].number_parked){
                return 1;
            }
        }
    }
    return 0;
}

// returns 1 if the car already exists, otherwise returns 0
int already_exists(char *plate){
    int num_cars=count_cars();
    for(int i=0;i<num_cars;i++){
        if (strcmp(cars[i].plate,plate) == 0) {
            return 1;
        }
    }
    return 0;
}

int checks_new_vehicle(char *park_name,int number_of_parks,char *plate_to_check,char *plate,char *entrydate,char *entrytime){
    if (check_if_park_exists(park_name,number_of_parks)==0){
        printf("%s: no such parking.\n",park_name);
        return 1;
    }
    if  (check_capacity(park_name,number_of_parks)==1){
        printf("%s: parking is full.\n",park_name);
        return 1;
    }
    else if(invalid_plate(plate_to_check)==1){
        printf("%s: invalid licence plate.\n",plate);
        return 1;
    }
    else if(already_parked(plate)==1){
        printf("%s: invalid vehicle entry.\n",plate);
        return 1;
    }
    else if(invalid_date(entrydate,entrytime)==1){
        printf("invalid date.\n");
        return 1;
    }
    return 0;
}

void addcar(struct CarStruct new_car,int number_of_cars){
    if(already_exists(new_car.plate)==0){
        strcpy(cars[number_of_cars].park_name,new_car.park_name);
        strcpy(cars[number_of_cars].plate,new_car.plate);
        strcpy(cars[number_of_cars].entrydate,new_car.entrydate);
        strcpy(cars[number_of_cars].entrytime,new_car.entrytime);
        strcpy(cars[number_of_cars].leftdate,"");
        strcpy(cars[number_of_cars].lefttime,"");
        cars[number_of_cars].logsnumber=0;
        
    }
    else{
        for(int i=0;i<number_of_cars;i++){
            if (strcmp(cars[i].plate,new_car.plate)==0){
                strcpy(cars[i].park_name,new_car.park_name);
                strcpy(cars[i].entrydate,new_car.entrydate);
                strcpy(cars[i].entrytime,new_car.entrytime);
                strcpy(cars[i].leftdate,"");
                strcpy(cars[i].lefttime,"");
                break;
            }
        }
    }
}

void addplatetopark(char *park_name,char *plate,int number_of_parks){
    int flag=0;
    for(int i=0;i<number_of_parks;i++){
        if (strcmp(parks[i].park_name,park_name)==0){
            char platetoadd[10];
            strcpy(platetoadd,plate);
            strcat(platetoadd," ");
            strcat(parks[i].already_parked,platetoadd);
            parks[i].number_parked+=1;
            printf("%s %d\n",park_name,parks[i].capacity-parks[i].number_parked);
            flag+=1;
        }
        if (strcmp(parks_p[i].park_name,park_name)==0){
            parks_p[i].number_parked+=1;
            flag+=1;
        }
        if(flag==2){
            break;
        }
    }
}

//adds a new vehicle
void new_vehicle(char *rest_command) {
    CarStruct new_car;
    int number_of_parks=count_parks();
    if (strchr(rest_command,'"') != NULL){ 
        char name[BUFSIZ]="";
        for(int i=2;rest_command[i]!='\0';i++){ //the index from the first " is 1, so we start from index 2 until the last "
            if(rest_command[i]=='"'){
                memmove(rest_command,rest_command+i+1,strlen(rest_command)-i); //when we get the index of the last ", 
                                                                               //we have the full name of the park, so we break the loop
                break;
            }
            else{
                strncat(name, &rest_command[i], 1); //while we haven't found the index from the last ", we are still looking
                                                    // at characters from the park's name, so we keep on adding them
            }
        }
        strcpy(new_car.park_name,name);       // add all the characters checked from the loop (except the ") to the name
        sscanf(rest_command, " %s %s %s", new_car.plate, //the rest of the characters are from other things such as the         
                                                         //plate and the date and time the vehicle entered the park
        new_car.entrydate,new_car.entrytime);
    }     
    else{
        sscanf(rest_command, "%s %s %s %s", new_car.park_name, new_car.plate, 
        new_car.entrydate,new_car.entrytime);
    }
    char plate_to_check[strlen(new_car.plate)+1];
    strcpy(plate_to_check,new_car.plate);      // just so it doesn't mess with the original plate, we create 
                                               //a copy of it that can be altered 
    if(checks_new_vehicle(new_car.park_name,number_of_parks,plate_to_check,new_car.plate,new_car.entrydate,new_car.entrytime)==1){
        return;
    }
    int number_of_cars=count_cars();
    addcar(new_car,number_of_cars);
    sscanf(new_car.entrydate,"%02d-%02d-%d",&globalday,&globalmonth,&globalyear);
    sscanf(new_car.entrytime,"%d:%d",&globalhour,&globalminute);
    addplatetopark(new_car.park_name,new_car.plate,number_of_parks);
}

//returns 1 if the exit of the vehicle is invalid, otherwise returns 0
int invalid_exit(const char *park_name, const char *plate){
    int num_parks = count_parks();
    for (int i=0;i<num_parks;i++){
        if (strcmp(parks[i].park_name, park_name) == 0) {
            char *to_check = strdup(parks[i].already_parked); 
            char *token;
            token = strtok(to_check," ");
            while (token != NULL) {
                if(strcmp(plate,token)==0){
                    free(to_check);
                    return 0;
                }
                token = strtok(NULL," ");
            }
            free(to_check);
            return 1;
        }
    }
    return 1;
}

//adds a log to a car (a log is a movement that the car makes)
void add_log(struct CarStruct *car, char log[6][BUFSIZ]) {
    if (car->logs == NULL) {
        car->logs = malloc(BUFSIZ * sizeof(char[6][BUFSIZ]));
    }
    memcpy(car->logs[car->logsnumber], log, sizeof(char[6][BUFSIZ]));
    car->logsnumber++;
}

//adds the bill of a car to a park
void add_value_to_day(int park_index,char *leftdate,float value){
    int number_of_dates=parks[park_index].number_of_dates;
    int found=0;
    for(int i=0;i<number_of_dates;i++){
        if(strcmp(parks[park_index].dates[i],leftdate)==0){
            parks[park_index].bill_per_day[i]+=value;
            found=1;
            break;
        }
    }
    if(found==0){
        parks[park_index].dates[number_of_dates] = (char*)malloc(strlen(leftdate)+1);
        strcpy(parks[park_index].dates[number_of_dates],leftdate);
        parks[park_index].bill_per_day[number_of_dates]=value;
        if(parks[park_index].number_of_dates!=0){
            parks[park_index].number_of_dates+=1;
        }
        else{
            parks[park_index].number_of_dates=1;
        }
    }
}

int differenceminutes(struct DateTime dt, struct DateTime df) {
    int minutes1=0;
    minutes1+=dt.tm_year*365*24*60;
    for(int i=0;i<dt.tm_mon;i++){
        minutes1+=daysInMonth(i)*24*60;
    }
    minutes1+=(dt.tm_mday-1)*24*60;
    minutes1+=dt.tm_hour*60;
    minutes1+=dt.tm_min;
    int minutes2=0;
    minutes2+=df.tm_year*365*24*60;
    for(int i=0;i<df.tm_mon;i++){
        minutes2+=daysInMonth(i)*24*60;
    }
    minutes2+=(df.tm_mday-1)*24*60;
    minutes2+=df.tm_hour*60;
    minutes2+=df.tm_min;
    return minutes2 - minutes1;
}

int checks_old_vehicle(char *park_name,int number_of_parks,char *plate,char *plate_to_check,
char *leftdate,char*lefttime){
    if (check_if_park_exists(park_name,number_of_parks)==0){
        printf("%s: no such parking.\n",park_name);
        return 1;
    }
    else if(invalid_plate(plate_to_check)==1){
        printf("%s: invalid licence plate.\n",plate);
        return 1;
    }
    else if(invalid_exit(park_name,plate)==1){
        printf("%s: invalid vehicle exit.\n",plate);
        return 1;
    }
    else if(invalid_date(leftdate,lefttime)==1){
        printf("invalid date.\n");
        return 1;
    }
    return 0;
}

float calculatebill(int time,float max_value,float value_after1h,float value_before1h){
    float actualvalue=0,value=0;
    while (time>0){
        if(time>=24*60){
            actualvalue+=max_value;
            time-=24*60;
        }
        else{
            if(time>60){
                value+=value_after1h;
                time-=15;
            }
            else{
                value+=value_before1h;
                time-=15;
            }
        }
        if(value>max_value){
            value=max_value;
            break;
        }
    }
    return actualvalue+value;
}

//used when a car is leaving a park
void old_vehicle(char *rest_command) {
    CarStruct old_car;
    int number_of_parks=count_parks();
    int number_of_cars=count_cars();
    if (strchr(rest_command,'"') != NULL){ 
        char name[BUFSIZ]="";
        for(int i=2;rest_command[i]!='\0';i++){ //the index from the first " is 1, so we start from index 2 until the last "
            if(rest_command[i]=='"'){
                memmove(rest_command,rest_command+i+1,strlen(rest_command)-i); //when we get the index of the last ", 
                                                                               //we have the full name of the park, so we break the loop
                break;
            }
            else{
                strncat(name, &rest_command[i], 1); //while we haven't found the index from the last ", we are still looking at 
                                                    //characters from the park's name, so we keep on adding them
            }
        }
        strcpy(old_car.park_name,name);       // add all the characters checked from the loop (except the ") to the name
        sscanf(rest_command, " %s %s %s", old_car.plate, //the rest of the characters are from other things such as the plat
                                                         //and the date and time the vehicle left
        old_car.leftdate,old_car.lefttime);
    }  
    else{
        sscanf(rest_command, "%s %s %s %s", old_car.park_name, old_car.plate, 
        old_car.leftdate,old_car.lefttime);
    }
    char plate_to_check[BUFSIZ];
    strcpy(plate_to_check,old_car.plate);      // just so it doesn't mess with the original plate, we create a copy of it that can be altered 
    if(checks_old_vehicle(old_car.park_name,number_of_parks,old_car.plate,plate_to_check,old_car.leftdate,old_car.lefttime)==1){
        return;
    }
    
    sscanf(old_car.leftdate,"%d-%d-%d",&globalday,&globalmonth,&globalyear);
    sscanf(old_car.lefttime,"%d:%d",&globalhour,&globalminute);
    int flag=0;
    for(int i=0;i<number_of_parks;i++){
        if (strcmp(parks[i].park_name,old_car.park_name)==0){
            char *p;
            while ((p = strstr(parks[i].already_parked, old_car.plate)) != NULL) {
                memmove(p,p+9,strlen(p+9)+1);
            }
            parks[i].number_parked-=1;
            for(int a=0;a<number_of_cars;a++){
                if (strcmp(cars[a].plate,old_car.plate)==0){
                    struct DateTime tm_entry, tm_left;
                    memset(&tm_entry, 0, sizeof(struct DateTime));
                    sscanf(cars[a].entrydate, "%d-%d-%d", &tm_entry.tm_mday, &tm_entry.tm_mon, &tm_entry.tm_year);
                    tm_entry.tm_year -= 2015;
                    tm_entry.tm_mon -= 1;
                    sscanf(cars[a].entrytime, "%d:%d", &tm_entry.tm_hour, &tm_entry.tm_min);
                    memset(&tm_left, 0, sizeof(struct DateTime));
                    sscanf(old_car.leftdate, "%d-%d-%d", &tm_left.tm_mday, &tm_left.tm_mon, &tm_left.tm_year);
                    tm_left.tm_year -= 2015;
                    tm_left.tm_mon -= 1;
                    sscanf(old_car.lefttime, "%d:%d", &tm_left.tm_hour, &tm_left.tm_min);          
                    int time = differenceminutes(tm_entry,tm_left);
                    float actualvalue=0,value=0;
                    while (time>0){
                        if(time>=24*60){
                            actualvalue+=parks[i].max_value;
                            time-=24*60;
                            
                        }
                        else{
                            if(time>60){
                                value+=parks[i].value_after1h;
                                time-=15;
                            }
                            else{
                                value+=parks[i].value_before1h;
                                time-=15;
                            }
                        }
                        if(value>parks[i].max_value){
                            value=parks[i].max_value;
                            break;
                        }
                    }
                    actualvalue+=value; 
                    cars[a].value=actualvalue;
                    add_value_to_day(i,old_car.leftdate,actualvalue);
                    char newlog[6][BUFSIZ];
                    char entrytime[6],lefttime[6];
                    int hour,minute;
                    sscanf(old_car.lefttime, "%02d:%02d", &hour, &minute);
                    sprintf(lefttime, "%02d:%02d", hour, minute);
                    sscanf(cars[a].entrytime, "%02d:%02d", &hour, &minute);
                    sprintf(entrytime, "%02d:%02d", hour, minute);
                    strcpy(newlog[0], old_car.park_name);
                    strcpy(newlog[1], cars[a].entrydate);
                    strcpy(newlog[2], entrytime);
                    strcpy(newlog[3], old_car.leftdate);
                    strcpy(newlog[4], lefttime);
                    char actualvaluestr[15];
                    sprintf(actualvaluestr, "%.2f",actualvalue);
                    strcpy(newlog[5], actualvaluestr);
                    add_log(&cars[a], newlog);
                    printf("%s %s %02i:%02i %s %02i:%02i %.2f\n",old_car.plate,cars[a].entrydate,hour,minute,old_car.leftdate,globalhour,
                    globalminute,actualvalue);
                    break;
                }
            }
            flag+=1;
        }
        if (strcmp(parks_p[i].park_name,old_car.park_name)==0){
            parks_p[i].number_parked-=1;
            flag+=1;
        }
        if(flag==2){
            break;
        }
    }
}

//returns 0 if a car is parked or has ever parked, otherwise returns 1
int checked_parked_logs(char *licenceplate){
    int number_of_cars=count_cars();
    for(int i=0;i<number_of_cars;i++){
        if(strcmp(licenceplate,cars[i].plate)==0){
            if (cars[i].logsnumber!=0||strlen(cars[i].entrytime) != 0){
                return 0;
            }
        }
    }
    return 1;
}

int checks_for_check_vehicle(char *licenceplate){
    char plate_to_check[9]="";
    strcpy(plate_to_check,licenceplate);      // just so it doesn't mess with the original plate, we create a copy of it that can be altered
    if(invalid_plate(plate_to_check)==1){
        printf("%s: invalid licence plate.\n",licenceplate);
        return 1;
    }
    else if(checked_parked_logs(licenceplate)==1){
        printf("%s: no entries found in any parking.\n",licenceplate);
        return 1;
    }
    return 0;
}
//checks the logs of a car
void check_vehicle(char *rest_command) {
    char licenceplate[9]="";
    int number_of_cars=count_cars();
    int number_of_parks=count_parks();
    sscanf(rest_command," %s",licenceplate);
    if(checks_for_check_vehicle(licenceplate)==1){
        return;
    }
    for(int i=0;i<number_of_cars;i++){
        if(strcmp(licenceplate,cars[i].plate)==0){
            int number_of_logs= cars[i].logsnumber;
            for(int a=0;a<number_of_parks;a++){
                for (int b=0;b<number_of_logs;b++){
                    if (strcmp(cars[i].logs[b][0],parks[a].park_name)==0){
                        printf("%s %s %s %s %s\n",cars[i].logs[b][0],cars[i].logs[b][1],cars[i].logs[b][2],
                        cars[i].logs[b][3],cars[i].logs[b][4]);
                    }
                }
                if(already_parked(licenceplate)==1&&strcmp(cars[i].park_name,parks[a].park_name)==0&&cars[i].flag==0){
                    int hour,minute;
                    sscanf(cars[i].entrytime, "%02d:%02d", &hour, &minute);
                    printf("%s %s %02i:%02i\n",cars[i].park_name,cars[i].entrydate,hour,minute);
                }
            } 
        }
    }
}

//to free the memory used in logs
void free_car_logs(CarStruct *car) {
    if (car->logs != NULL) {
        free(car->logs);
        car->logs = NULL; // Set the pointer to NULL after freeing to avoid dangling pointers
    }
}

//lists the bill for a park
void list_bill_day(char *park_name){
    int number_of_parks=count_parks();
    char name[BUFSIZ]="";
    for(int i=0;i<number_of_parks;i++){
        strcpy(name, " ");
        strcat(name,parks[i].park_name);;
        if (strcmp(name,park_name)==0){
            for(int a=0;a<parks[i].number_of_dates;a++){
                printf("%s %.2f\n",parks[i].dates[a],parks[i].bill_per_day[a]);
            }
        }
    }
}

//lists the bill for a specific date
void list_bill_specific_date(char *date,char *park_name){
    int days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (strlen(date)!=10){
        printf("invalid date.\n");
        return;
    }
    int day,month,year;
    sscanf(date,"%d-%d-%d",&day,&month,&year);
    if(days_per_month[month-1]<day||day<1||month>12||month<1){
        printf("invalid date.\n");
        return;
    }
    int number_of_cars=count_cars();
    for(int i=0;i<number_of_cars;i++){
        for(int a=0;a<cars[i].logsnumber;a++){
            if(strcmp(cars[i].logs[a][3],date)==0&&strcmp(cars[i].logs[a][0],park_name)==0){
                printf("%s %s %s\n",cars[i].plate,cars[i].logs[a][4],cars[i].logs[a][5]);
            }
        }
    }
}

//to check the bills of either a park or a specific date
void bill_day(char *rest_command) {
    char park_name[BUFSIZ];
    char date[11];
    int number_of_parks=count_parks();
    if (strchr(rest_command,'"') != NULL){
        char name[BUFSIZ]=""; 
        for(int i=2;rest_command[i]!='\0';i++){ //the index from the first " is 1, so we start from index 2 until the last "
            if(rest_command[i]=='"'){
                memmove(rest_command,rest_command+i+1,strlen(rest_command)-i); //when we get the index of the last ", we have the 
                                                                               //full name of the park, so we break the loop
                break;
            }
            else{
                strncat(name, &rest_command[i], 1); //while we haven't found the index from the last ", we are still looking at 
                                                    //characters from the park's name, so we keep on adding them
            }
        }
        strcpy(park_name,name);       // add all the characters checked from the loop (except the ") to the name
        if (check_if_park_exists(park_name,number_of_parks)==0){
                printf("%s: no such parking.\n",park_name);
                return;
            }
        else{
            if (strcmp(rest_command,"")==0 || rest_command[0] == '0') {
                list_bill_day(park_name);
            }    
            else{
                sscanf(rest_command, " %s",date);
                list_bill_specific_date(date,park_name);
                return;
            }
        }
    }else{
        if (sscanf(rest_command, "%*s %*d-%*d-%*d") == 0){
            sscanf(rest_command, "%s %s",park_name,date);
            if (check_if_park_exists(park_name,number_of_parks)==0){
                printf("%s: no such parking.\n",park_name);
                return;
            }
            list_bill_specific_date(date,park_name);
        }
        else{
            sscanf(rest_command, " %s",park_name);
            if (check_if_park_exists(park_name,number_of_parks)==0){
                printf("%s: no such parking.\n",park_name);
                return;
            }
            list_bill_day(rest_command);
        }
    }
}

void remove_park_from_struct_Park(int park_index){
    strcpy(parks[park_index].park_name," ");
    strcpy(parks[park_index].already_parked," ");
    parks[park_index].max_value=0;
    parks[park_index].capacity=0;
    parks[park_index].number_of_dates=0;
    parks[park_index].number_parked=0;
    parks[park_index].value_after1h=0;
    parks[park_index].value_before1h=0;
    MAX_PARKS_NUMBER+=1;
}
void remove_park_from_struct_Park_P(int park_index){
    strcpy(parks_p[park_index].park_name," ");
    parks_p[park_index].capacity=0;
    parks_p[park_index].number_parked=0;
}

void remove_cars(int number_of_cars,char *park_name){
    for(int a=0;a<number_of_cars;a++){
        if(strcmp(park_name,cars[a].park_name)==0){
            strcpy(cars[a].park_name," ");
            strcpy(cars[a].entrydate,"");
            strcpy(cars[a].entrytime,"");
            strcpy(cars[a].leftdate,"");
            strcpy(cars[a].lefttime,"");
            cars[a].flag=1;
        }
        for(int b=0;b<cars[a].logsnumber;b++){
            if(strcmp(park_name,cars[a].logs[b][0])==0){
                for (int c=b; c<cars[a].logsnumber-1;c++) {
                    memcpy(cars[a].logs[c], cars[a].logs[c+1], sizeof(cars[a].logs[c]));
                }
                cars[a].logsnumber-=1;
                b=-1;
            }
        }
    }
}

//removes a park and its contents, as well as the movements (logs) that were made in it
void remove_park(char *rest_command) {
    char park_name[strlen(rest_command)];
    if (strchr(rest_command,'"') != NULL){ 
        char name[BUFSIZ]="";
        for(int i=2;rest_command[i]!='\0';i++){ //the index from the first " is 1, so we start from index 2 until the last "
            if(rest_command[i]=='"'){
                memmove(rest_command,rest_command+i+1,strlen(rest_command)-i); //when we get the index of the last ", 
                                                                               //we have the full name of the park, so we break the loop
                break;
            }
            else{
                strncat(name, &rest_command[i], 1); //while we haven't found the index from the last ", we are 
                                                    //still looking at characters from the park's name, so we keep on adding them
            }
        }
        strcpy(park_name,name);       // add all the characters checked from the loop (except the ") to the name
    }     
    else{
        sscanf(rest_command," %s",park_name);
    }
    int number_of_parks=count_parks();
    if(check_if_park_exists(park_name,number_of_parks)==0){
        printf("%s: no such parking.\n",park_name);
        return;
    }
    for(int i=0;i<number_of_parks;i++){
        if(strcmp(park_name,parks[i].park_name)==0||strcmp(" ",parks[i].park_name)==0){
            remove_park_from_struct_Park(i);
        }
        else{
            printf("%s\n",parks[i].park_name);
        }
        if(strcmp(park_name,parks_p[i].park_name)==0||strcmp(" ",parks_p[i].park_name)==0){
            remove_park_from_struct_Park_P(i);
        }
        int number_of_cars=count_cars();
        remove_cars(number_of_cars,park_name);
    }
}

void free_all_cars(){
    int number_of_cars=count_cars();
    for (int i = 0;i<number_of_cars;i++) {
        free_car_logs(&cars[i]); // Free logs memory for each car
    }
}

void free_all_parks(){
    int number_of_parks=count_parks();
    for (int i = 0;i<number_of_parks;i++) {
        for (int j=0; j<parks[i].number_of_dates; j++) {
            free(parks[i].dates[j]);
        }
    }
}