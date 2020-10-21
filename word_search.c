#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


typedef struct _Coordinate
{
    char character;
    int x;
    int y;
} Coordinate;

int random_range(int start, int end)
{
    return rand() % (end + 1 - start) + start;
}

Coordinate** make_coordinate_array(int width, int height)
{
    Coordinate** coords = (Coordinate**)malloc(sizeof(Coordinate*) * width * height);
    for (int i = 0; i < width * height; i++)
    {
        coords[i] = (Coordinate*) malloc(sizeof(Coordinate));
        coords[i]->x = -1;
        coords[i]->y = -1;
    }
    return coords;
}

void free_coordinate_array(Coordinate** coords, int width, int height)
{
    for (int i = 0; i < width * height; i++)
    {
        free(coords[i]);
    }
    free(coords);
}

bool check_overlap(Coordinate** coords, int width, int height, char chk_char, int chk_x, int chk_y)
{
    char coords_char;
    int coords_x, coords_y, i;
    for (i = 0; i < width * height; i++)
    {
        coords_char = coords[i]->character;
        coords_x = coords[i]->x;
        coords_y = coords[i]->y;
        if ((chk_x == coords_x) && (chk_y == coords_y))
        {
            if (chk_char == coords_char)
            {
                return true;
            }
            else
            {
                //printf("Overlapped At X: %d, Y: %d\n", coords_x, coords_y);
                return false;
            }
        }
    }
    return true;
}

void set_coordinate(Coordinate** coords, int width, int height, char set_char, int set_x, int set_y)
{
    for (int i = 0; i < width * height; i++)
    {
        if (coords[i]->x == -1)
        {
            coords[i]->character = set_char;
            coords[i]->x = set_x;
            coords[i]->y = set_y;
            break;
        }
    }
}

char** make_2d_char_array(int width, int height) {
    char** arr;
    arr = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++)
    {
        arr[i] = (char*)malloc(width * sizeof(char));
    }
    return arr;
}

void free_2d_char_array(char** arr, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

char** copy_2d_char_array(char** arr, int width, int height)
{
    char** cpy_arr = make_2d_char_array(width, height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cpy_arr[i][j] = arr[i][j];
        }
    }
    return cpy_arr;
}

char** write_horizontal(Coordinate** coords, char** word_array, char* word, short direction, int width, int height)
{
    char** copied_array = copy_2d_char_array(word_array, width, height);
    int start_pos, end_pos, vertical_pos, i, word_pos;
    bool loop = false;
    while (true) 
    {
        free_2d_char_array(word_array, width, height);
        word_array = copy_2d_char_array(copied_array, width, height);
        vertical_pos = random_range(0, height - 1);
        start_pos = random_range(0, width - strlen(word));
        end_pos = start_pos + strlen(word) - 1;
        if (direction == 1) // left -> right
        {
            word_pos = 0;
            for (i = start_pos; i <= end_pos; i++)
            {
                if (!check_overlap(coords, width, height, word[word_pos], i, vertical_pos))
                {
                    loop = true;
                    break;
                }
                word_array[vertical_pos][i] = word[word_pos];
                set_coordinate(coords, width, height, word[word_pos], i, vertical_pos);
                word_pos++;
            }
        }
        if (direction == 2) // left <- right
        {
            word_pos = strlen(word) - 1;
            for (i = start_pos; i <= end_pos; i++)
            {
                if (!check_overlap(coords, width, height, word[word_pos], i, vertical_pos))
                {
                    loop = true;
                    break;
                }
                word_array[vertical_pos][i] = word[word_pos];
                set_coordinate(coords, width, height, word[word_pos], i, vertical_pos);
                word_pos--;
            }
        }
        if (loop)
        {
            loop = false;
            continue;
        }
        else 
        {
            break;
        }
    }
    return word_array;
}

char** write_vertical(Coordinate** coords, char** word_array, char* word, short direction, int width, int height)
{
    char** copied_array = copy_2d_char_array(word_array, width, height);
    int start_pos, end_pos, horizontal_pos, i, word_pos;
    bool loop = false;
    while (true)
    {
        free_2d_char_array(word_array, width, height);
        word_array = copy_2d_char_array(copied_array, width, height);
        horizontal_pos = random_range(0, width - 1);
        start_pos = random_range(0, height - strlen(word));
        end_pos = start_pos + strlen(word) - 1;
        if (direction == 1) // top -> down
        {
            word_pos = 0;
            for (i = start_pos; i <= end_pos; i++)
            {
                if (!check_overlap(coords, width, height, word[word_pos], horizontal_pos, i))
                {
                    loop = true;
                    break;
                }
                word_array[i][horizontal_pos] = word[word_pos];
                set_coordinate(coords, width, height, word[word_pos], horizontal_pos, i);
                word_pos++;
            }
        }
        if (direction == 2) // top <- down
        {
            word_pos = strlen(word) - 1;
            for (i = start_pos; i <= end_pos; i++)
            {
                if (!check_overlap(coords, width, height, word[word_pos], horizontal_pos, i))
                {
                    loop = true;
                    break;
                }
                word_array[i][horizontal_pos] = word[word_pos];
                set_coordinate(coords, width, height, word[word_pos], horizontal_pos, i);
                word_pos--;
            }
        }
        if (loop)
        {
            loop = false;
            continue;
        }
        else
        {
            break;
        }
    }
    return word_array;
}

char** write_diagonal(Coordinate** coords, char** word_array, char* word, short direction, int width, int height)
{
    char** copied_array = copy_2d_char_array(word_array, width, height);
    int horizontal_start_pos, horizontal_end_pos, vertical_start_pos, vertical_end_pos, i, word_pos;
    bool loop = false;
    while (true)
    {
        free_2d_char_array(word_array, width, height);
        word_array = copy_2d_char_array(copied_array, width, height);
        vertical_start_pos = random_range(0, height - strlen(word));
        vertical_end_pos = vertical_start_pos + strlen(word) - 1;
        horizontal_start_pos = random_range(0, width - strlen(word));
        horizontal_end_pos = horizontal_start_pos + strlen(word) - 1;
        if (direction == 1) // left -> right , top -> down
        {
            word_pos = 0;
            for (i = vertical_start_pos; i <= vertical_end_pos; i++)
            {
                if (!check_overlap(coords, width, height, word[word_pos], horizontal_start_pos + word_pos, i))
                {
                    loop = true;
                    break;
                }
                word_array[i][horizontal_start_pos + word_pos] = word[word_pos];
                set_coordinate(coords, width, height, word[word_pos], horizontal_start_pos + word_pos, i);
                word_pos++;
            }
        }
        if (direction == 2) // left -> right, top <- down
        {
            word_pos = 0;
            for (i = vertical_end_pos; i >= vertical_start_pos; i--)
            {
                if (!check_overlap(coords, width, height, word[word_pos], horizontal_start_pos + word_pos, i))
                {
                    loop = true;
                    break;
                }
                word_array[i][horizontal_start_pos + word_pos] = word[word_pos];
                //printf("%d\n", word_pos);
                set_coordinate(coords, width, height, word[word_pos], horizontal_start_pos + word_pos, i);
                word_pos++;
            }
        }
        if (direction == 3) // left <- right, top -> down
        {
            word_pos = 0;
            for (i = horizontal_end_pos; i >= horizontal_start_pos; i--)
            {
                if (!check_overlap(coords, width, height, word[word_pos], i, word_pos))
                {
                    loop = true;
                    break;
                }
                word_array[word_pos][i] = word[word_pos];
                set_coordinate(coords, width, height, word[word_pos], i, word_pos);
                word_pos++;
            }
        }
        if (direction == 4) // left <- right, top <- down
        {
            word_pos = 0;
            int mutable_start_pos = horizontal_start_pos < vertical_start_pos ? horizontal_start_pos : vertical_start_pos;
            int mutable_end_pos = mutable_start_pos + strlen(word) - 1;
            for (i = mutable_end_pos; i >= mutable_start_pos; i--)
            {
                if (!check_overlap(coords, width, height, word[word_pos], i, i))
                {
                    loop = true;
                    break;
                }
                word_array[i][i] = word[word_pos];
                set_coordinate(coords, width, height, word[word_pos], i, i);
                word_pos++;
            }
        }
        if (loop)
        {
            loop = false;
            continue;
        }
        else
        {
            break;
        }
    }
    return word_array;
}

void make_board(char** word_array, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) {
            word_array[i][j] = (char)random_range(65, 90);
        }
    }
}

void print_board(char** word_array, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) {
            printf("%c ", word_array[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    srand(time(NULL));
    int minimum = 7;
    int maximum = 500;
    int width;
    int height;
    int slope;
    int list_length;
    int direction;
    char** word_list;
    char* input_word;
    char** word_array;
    Coordinate** coords;
    printf("WORD SEARCH 퍼즐 제작기\n");
    while (true)
    {
        printf("가로 길이를 입력하세요(최소 %d, 최대 %d) >> ", minimum, maximum);
        scanf("%d", &width);
        if ((width < minimum) || (width > maximum))
        {
            printf("값이 너무 작거나 큽니다");
            continue;
        }
        printf("세로 길이를 입력하세요(최대 %d, 최대 %d) >> ", minimum, maximum);
        scanf("%d", &height);
        if ((height < minimum) || (height > maximum))
        {
            printf("값이 너무 작거나 큽니다");
            continue;
        }
        int min_length = width < height ? width : height;
        word_list = malloc(sizeof(char) * min_length);
        input_word = malloc(sizeof(char) * min_length);
        for (int i = 0; i < min_length; i++)
        {
            printf("단어를 입력하세요 (%d개 입력 가능, 최대 길이는 %d, 없으면 QQUIT) >> ", min_length - i, min_length);
            scanf("%s", input_word);
            if (strcmp(input_word, "QQUIT") == 0)
            {
                list_length = i;
                break;
            }
            if (strlen(input_word) > min_length)
            {
                printf("단어가 너무 깁니다\n");
                list_length = i;
                break;
            }
            word_list[i] = malloc(sizeof(char) * (min_length + 1));
            strcpy(word_list[i], input_word);
        }
        free(input_word);
        word_array = make_2d_char_array(width, height);
        make_board(word_array, width, height);
        coords = make_coordinate_array(width, height);
        for (int j = 0; j < list_length; j++)
        {
            slope = random_range(1, 3);
            direction = random_range(1, 2);
            switch (slope)
            {
            case 1:
                word_array = write_horizontal(coords, word_array, word_list[j], direction, width, height);
                break;
            case 2:
                word_array = write_vertical(coords, word_array, word_list[j], direction, width, height);
                break;
            case 3:
                direction = random_range(1, 4);
                word_array = write_diagonal(coords, word_array, word_list[j], direction, width, height);
                break;
            }
            free(word_list[j]);
        }
        free(word_list);
        print_board(word_array, width, height);
        free_2d_char_array(word_array, width, height);
        free_coordinate_array(coords, width, height);
    }
    return 0;
}
