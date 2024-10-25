#include <stdio.h>
#include <math.h>
#include <string.h>
#define INPUT_SIZE_ROW 366
#define INPUT_SIZE_COL 8
typedef struct
{
    char name[20];
    double temp[366];
    int day[366];
    int year;
    int col_n;
} lake_t;

void get_yearly_avg(lake_t *current_lake, double *avg_temp_list, double *lowest, double *highest, int *lowest_index, int *highest_index, int row_in);

double get_avg_arr(double *avg_array, int size); // calculates and returns the average of a given array

void get_summer_yearly_avg(lake_t *lake, double *avgtemps, int *avgtemps_lake_i, int lower_bound, int higher_bound);

void get_winter_yearly_avg(lake_t *lake, double *avgtemps, int *avgtemps_lake_i, int lower_bound1, int higher_bound1, int lower_bound2, int higher_bound2);

int get_above_below_temp(lake_t *lake_to_search, int lake_idx, double target_temp, int row_in, int operator);

void get_min_arr(double file_matrix[][INPUT_SIZE_COL], int row_in, int *idx_arr, int lake_to_fetch);

void get_max_arr(double file_matrix[][INPUT_SIZE_COL], int row_in, int *idx_arr, int lake_to_fetch);

void populatelakes(lake_t *lake_to_process, double file_matrix[][INPUT_SIZE_COL], int row_in);

void convert_to_date(int day, int *MM, int *DD, int year);

int main(void)
{
    FILE *in;
    in = fopen("data.txt", "r");

    double in_mat[INPUT_SIZE_ROW][INPUT_SIZE_COL];
    double avg_temp_arr[6]; // 6 lakes
    double avg_of_avg;      // average of the average lakes temp
    double lowest_avg_temp, highest_avg_temp;
    int lowest_avg_temp_i, highest_avg_temp_i;
    int avg_temps_idx[6];
    lake_t lakes[6];
    int row_in = 0;
    int month, day;

    lakes[0].col_n = 2; // superior
    lakes[1].col_n = 3; // michigan
    lakes[2].col_n = 4; // huron
    lakes[3].col_n = 5; // erie
    lakes[4].col_n = 6; // ontario
    lakes[5].col_n = 7; // st clair

    strcpy(lakes[0].name, "Superior");
    strcpy(lakes[1].name, "Michigan");
    strcpy(lakes[2].name, "Huron");
    strcpy(lakes[3].name, "Erie");
    strcpy(lakes[4].name, "Ontario");
    strcpy(lakes[5].name, "St.Clair");

    for (row_in = 0; !feof(in); row_in++) // save entire data file into a 2d matrix
    {
        for (int col_in = 0; col_in < INPUT_SIZE_COL; col_in++)
        {
            fscanf(in, "%lf", &in_mat[row_in][col_in]);
        }
    }
    // index of last actual populated rows with meaningful data == row_in - 1;
    fclose(in);

    int idx_arr[row_in];

    populatelakes(lakes, in_mat, row_in);

    get_yearly_avg(lakes, avg_temp_arr, &lowest_avg_temp, &highest_avg_temp, &lowest_avg_temp_i, &highest_avg_temp_i, row_in);

    printf("The lowest average temp is %lf, Belonging to lake named %s\n", lowest_avg_temp, lakes[lowest_avg_temp_i].name);
    printf("The highest average temp is %lf, Belonging to lake named %s\n", highest_avg_temp, lakes[highest_avg_temp_i].name);

    puts("The average temp for each lake is as follows:");
    for (int show_avg_i = 0; show_avg_i < 6; show_avg_i++)
    {
        printf("%s: %lf\n", lakes[show_avg_i].name, avg_temp_arr[show_avg_i]);
    }

    avg_of_avg = get_avg_arr(avg_temp_arr, 6);

    printf("\nthe following lakes have an average above the average (%lf)\n", avg_of_avg);
    for (int counter = 0; counter < 6; counter++)
    {
        if (avg_temp_arr[counter] > avg_of_avg)
        {
            printf("%s : %lf\n", lakes[counter].name, avg_temp_arr[counter]);
        }
    }
    printf("\nthe following lakes have an average below the average (%lf)\n", avg_of_avg);
    for (int counter = 0; counter < 6; counter++)
    {
        if (avg_temp_arr[counter] < avg_of_avg)
        {
            printf("%s : %lf\n", lakes[counter].name, avg_temp_arr[counter]);
        }
    }
    printf("\nThe minimum temperature for each lake is listed, if there are duplicates, they are also listed in sequence:\n");
    for (int lakes_i = 0; lakes_i < 6; lakes_i++)
    {
        get_min_arr(in_mat, row_in, idx_arr, lakes[lakes_i].col_n);
        for (int convert_i = 0; idx_arr[convert_i] != -1; convert_i++)
        {
            convert_to_date(idx_arr[convert_i], &month, &day, in_mat[0][0]);
            printf("Lake %s : MM:%d DD:%d : Temp: %lf\n", lakes[lakes_i].name, month, day, lakes[lakes_i].temp[(idx_arr[convert_i])-1]);
            //printf("\n Lake %s : %d : Temp: %lf\n", lakes[lakes_i].name, idx_arr[convert_i], lakes[lakes_i].temp[(idx_arr[convert_i])-1]);
        }
    }
    printf("\nThe maximum temperature for each lake is listed, if there are duplicates, they are also listed in sequence:\n");
    for (int lakes_i = 0; lakes_i < 6; lakes_i++)
    {
        get_max_arr(in_mat, row_in, idx_arr, lakes[lakes_i].col_n);
        for (int convert_i = 0; idx_arr[convert_i] != -1; convert_i++)
        {
            convert_to_date(idx_arr[convert_i], &month, &day, in_mat[0][0]);
            printf("Lake %s : MM:%d DD:%d : Temp: %lf\n", lakes[lakes_i].name, month, day, lakes[lakes_i].temp[(idx_arr[convert_i])-1]);
        }
    }

    get_summer_yearly_avg(lakes, avg_temp_arr, avg_temps_idx, 171, 264); // summer
    printf("\nSummer averages:\n");
    for (int counter = 5; counter >= 0; counter--)
    {
        printf("Lake %s : %lf\n", lakes[avg_temps_idx[counter]].name, avg_temp_arr[counter]);
    }

    get_winter_yearly_avg(lakes, avg_temp_arr, avg_temps_idx, 0, 78, 354, 365); // winter
    printf("\nWinter averages:\n");
    for (int counter = 5; counter >= 0; counter--)
    {
        printf("Lake %s : %lf\n", lakes[avg_temps_idx[counter]].name, avg_temp_arr[counter]);
    }
    // Printing above below 20C 0C number of days
    for (int lake_i = 0; lake_i < 6; lake_i++)
    {
        printf("The number of days above 20C in lake %s is: %d\n", lakes[lake_i].name, get_above_below_temp(lakes, lake_i, 20, row_in, 0)); // 0 for bigger than or equal to
    }
    puts("");
    for (int lake_i = 0; lake_i < 6; lake_i++)
    {
        printf("The number of days below 0C in lake %s is: %d\n", lakes[lake_i].name, get_above_below_temp(lakes, lake_i, 0, row_in, 1)); // 1 for less than or equal to
    }
    return 0;
}

void populatelakes(lake_t *lake_to_process, double file_matrix[][INPUT_SIZE_COL], int row_in)
{
    int counter = 0, counter1 = 0;

    for (int lakes_i = 0; lakes_i < 6; lakes_i++) // main loop, used to pupolate the temp array component of each lake structure
    {
        for (int row = 0; row < row_in; row++) // iterate the rows of the file matrix
        {
            for (int col = 0; col < INPUT_SIZE_COL; col++) // iterate the columns of the file matrix
            {
                if (col == lake_to_process->col_n) // check if current column is the matching column for a given lake[0-6]
                {
                    lake_to_process->temp[counter] = file_matrix[row][col]; // assign temp from file matrix to temp component of lake[counter], i.e lake[0]
                    counter++;
                    break;
                }
            }
        }
        // Populate the day array for every lake struct
        for (int row1 = 0; row1 < row_in; row1++) // iterate the rows of the file matrix
        {
            for (int col1 = 0; col1 < INPUT_SIZE_COL; col1++) // iterate the columns of the file matrix
            {
                if (col1 == 1) // check if current column is 1 (the column for day in the source file matrix)
                {
                    lake_to_process->day[counter1] = file_matrix[row1][col1]; // assign day from file matrix to day component of lake[counter1], i.e lake[0]
                    counter1++;
                    break;
                }
            }
        }

        lake_to_process = lake_to_process + 1; // iterates the lake_to_process POINTER to the next lake. i.e lake[0] => lake[1]
        counter = 0, counter1 = 0;             // reset the counters as these counters must be reset for every lake since they control the index of each struct's subarray
    }
}

void get_yearly_avg(lake_t *current_lake, double *avg_temp_list, double *lowest, double *highest, int *lowest_index, int *highest_index, int row_in) // current_lake is a pointer to the lakes[0-6] structure array
{
    int counter = 0;
    double sum_temp = 0;
    double average = 0;

    for (int lakes_i = 0; lakes_i < 6; lakes_i++)
    {
        for (int temp_i = 0; temp_i < row_in; temp_i++)
        {
            sum_temp = sum_temp + current_lake[lakes_i].temp[temp_i];
            counter++;
        }

        average = sum_temp / counter;
        avg_temp_list[lakes_i] = average;
        sum_temp = 0, counter = 0, average = 0;
    }
    // Finding the lowest average temp
    *lowest = avg_temp_list[0]; // Set provided pointer lowest

    for (int avg_counter = 0; avg_counter < 6; avg_counter++)
    {
        if (avg_temp_list[avg_counter] < *lowest)
        {
            *lowest = avg_temp_list[avg_counter];
            *lowest_index = avg_counter; // This index will be used by main function to show the name of the involved lake
        }
    }
    // Finding the highest average temp
    *highest = avg_temp_list[0];

    for (int avg_counter = 0; avg_counter < 6; avg_counter++)
    {
        if (avg_temp_list[avg_counter] > *highest)
        {
            *highest = avg_temp_list[avg_counter];
            *highest_index = avg_counter; // This index will be used by main function to show the name of the involved lake
        }
    }
}

double get_avg_arr(double *avg_array, int size)
{
    double sum = 0, average;
    int counter = 0;

    for (int i = 0; i < size; i++)
    {
        sum = sum + avg_array[i];
        counter++;
    }

    average = sum / ((double)counter);

    return average;
}

void get_min_arr(double file_matrix[][INPUT_SIZE_COL], int row_in, int *idx_arr, int lake_to_fetch)
{
    double minimum = file_matrix[0][lake_to_fetch];
    int counter = 0;

    for (int row = 0; row < row_in; row++)
    {
        if (file_matrix[row][lake_to_fetch] < minimum)
        {
            minimum = file_matrix[row][lake_to_fetch];
        }
    }
    for (int row = 0; row < row_in; row++)
    {
        if ((file_matrix[row + 1][lake_to_fetch] == minimum))
        {
            idx_arr[counter] = file_matrix[row + 1][1];
            counter++;
        }
    }
    for (int clean = counter; clean < row_in; clean++)
    {
        idx_arr[clean] = -1;
    }
}

void get_max_arr(double file_matrix[][INPUT_SIZE_COL], int row_in, int *idx_arr, int lake_to_fetch)
{
    double maximum = file_matrix[0][lake_to_fetch];
    int counter = 0;

    for (int row = 0; row < row_in; row++)
    {
        if (file_matrix[row][lake_to_fetch] > maximum)
        {
            maximum = file_matrix[row][lake_to_fetch];
        }
    }
    for (int row = 0; row < row_in; row++)
    {
        if (file_matrix[row + 1][lake_to_fetch] == maximum)
        {
            idx_arr[counter] = file_matrix[row + 1][1];
            counter++;
        }
    }
    for (int clean = counter; clean < row_in; clean++)
    {
        idx_arr[clean] = -1;
    }
}

void get_summer_yearly_avg(lake_t *lake, double *avgtemps, int *avgtemps_lake_i, int lower_bound, int higher_bound)
{
    int counter = 0, sort_i, sort_j, min_index;
    double sum_temp = 0, temp;
    double average = 0;

    for (int lakes_i = 0; lakes_i < 6; lakes_i++)
    {
        avgtemps_lake_i[lakes_i] = lakes_i;
        for (int temp_i = lower_bound; temp_i < higher_bound; temp_i++)
        {
            sum_temp = sum_temp + lake[lakes_i].temp[temp_i];
            counter++;
        }

        average = sum_temp / counter;
        avgtemps[lakes_i] = average;
        sum_temp = 0, counter = 0, average = 0;
    }

    for (sort_i = 0; sort_i < 6; sort_i++)
    {
        min_index = sort_i;

        for (sort_j = sort_i + 1; sort_j < 6; sort_j++)
        {
            if (avgtemps[sort_j] < avgtemps[min_index])
            {
                min_index = sort_j;
            }
        }

        if (min_index != sort_i)
        {
            temp = avgtemps[min_index];
            avgtemps[min_index] = avgtemps[sort_i];
            avgtemps[sort_i] = temp;

            temp = avgtemps_lake_i[min_index];
            avgtemps_lake_i[min_index] = avgtemps_lake_i[sort_i];
            avgtemps_lake_i[sort_i] = temp;
        }
    }
}

void get_winter_yearly_avg(lake_t *lake, double *avgtemps, int *avgtemps_lake_i, int lower_bound1, int higher_bound1, int lower_bound2, int higher_bound2)
{
    int counter = 0, sort_i, sort_j, min_index;
    double sum_temp = 0, temp;
    double average = 0;

    for (int lakes_i = 0; lakes_i < 6; lakes_i++)
    {
        avgtemps_lake_i[lakes_i] = lakes_i;
        for (int temp_i = lower_bound1; temp_i < higher_bound1; temp_i++)
        {
            sum_temp = sum_temp + lake[lakes_i].temp[temp_i];
            counter++;
        }
        for (int temp_i = lower_bound2; temp_i < higher_bound2; temp_i++)
        {
            sum_temp = sum_temp + lake[lakes_i].temp[temp_i];
            counter++;
        }
        average = sum_temp / counter;
        avgtemps[lakes_i] = average;
        sum_temp = 0, counter = 0, average = 0;
    }

    for (sort_i = 0; sort_i < 6; sort_i++)
    {
        min_index = sort_i;

        for (sort_j = sort_i + 1; sort_j < 6; sort_j++)
        {
            if (avgtemps[sort_j] < avgtemps[min_index])
            {
                min_index = sort_j;
            }
        }

        if (min_index != sort_i)
        {
            temp = avgtemps[min_index];
            avgtemps[min_index] = avgtemps[sort_i];
            avgtemps[sort_i] = temp;

            temp = avgtemps_lake_i[min_index];
            avgtemps_lake_i[min_index] = avgtemps_lake_i[sort_i];
            avgtemps_lake_i[sort_i] = temp;
        }
    }
}

int get_above_below_temp(lake_t *lake_to_search, int lake_idx, double target_temp, int row_in, int operator)
{
    int counter = 0;
    switch (operator)
    {
    case 0: // bigger than or equal to

        for (int temp_i = 0; temp_i < row_in; temp_i++)
        {
            if (lake_to_search[lake_idx].temp[temp_i] >= target_temp)
            {
                counter++;
            }
        }
        break;

    case 1: // less than or equal to

        for (int temp_i = 0; temp_i < row_in; temp_i++)
        {
            if (lake_to_search[lake_idx].temp[temp_i] <= target_temp)
            {
                counter++;
            }
        }
        break;
    }

    return counter;
}

void convert_to_date(int day, int *m, int *d, int year)
{
    int sum = 0, i = 0;
    int daysofmonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Jan to Dec # of days

    if (year % 4 == 0)
    {
        daysofmonths[1]++; // Add a day to Feb if leap year
    }

    while (day > sum)
    {
        sum += daysofmonths[i];
        i++;
    }

    *m = i;
    *d = (daysofmonths[i - 1]) - (sum - day);
}