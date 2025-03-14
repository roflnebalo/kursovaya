#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14 // ЧИСЛО П
#define n 10    // размер массива

double F1(double x);  // функция линейная
double F2(double x);  // кусочная функция
double F3(double x);  // ареа-синус (обратный гиперболический синус)

double* generating_values(double xmin, double dx);  // генерирует массив значений x
double* random_values(int N, double xmin, double xmax);  // генерирует уникальные случайные значения
double result(FILE* outputFile, double x, int func_num, double (*func)(double)) {
    double result_value = func(x);  // Вычисляем значение функции
    fprintf(outputFile, "|%.5lf\t|%.5lf\t|\n", x, result_value);  // Вывод в файл
    return result_value;  // Возвращаем результат вычисления функции
}
double find_min(double (*func)(double), double* values, int N);  // находит минимальное значение
double find_max(double (*func)(double), double* values, int N);  // находит максимальное значение
double calculate_abs_difference(double (*func)(double), double* values, int N);  // вычисляет модуль разности
double differentiate(double (*func)(double), double x, double epsilon);  // приближенное дифференцирование

void menu(FILE* outputFile);  // меню для выбора действий

int main() {
    setlocale(LC_CTYPE, "RUS");  // установка локали для русского языка
    srand(time(NULL));  // инициализация генератора случайных чисел
    FILE* outputFile = fopen("output.csv", "w");  // открытие файла для записи
    if (outputFile == NULL) {
        printf("Ошибка открытия файла!\n");
        return 1;
    }
    menu(outputFile);  // вызов меню
    fclose(outputFile);  // закрытие файла
    return 0;
}

void menu(FILE* outputFile) {
    int choice;
    double xmin, xmax, dx, x, epsilon = 1e-5;
    int func_choice, N;
    double* array;

    while (1) {
        printf("Главное меню:\n");
        printf("1. Вычисление функции\n");
        printf("2. Табуляция функции\n");
        printf("3. Генерация уникальных случайных значений\n");
        printf("4. Модуль разности максимального и минимального значений\n"); 
        printf("5. Дифференцирование функции в заданной точке\n");
        printf("6. Выход\n");
        scanf("%d", &choice);  // выбор пункта меню
        switch (choice) {
            case 1:
    printf("Выберите функцию:\n1 - F1 (линейная)\n2 - F2 (кусочная)\n3 - F3 (ареа-синус)\n");
    scanf("%d", &func_choice);  // выбор функции
    printf("Введите значение x:\n");
    scanf("%lf", &x);  // ввод значения x
    fprintf(outputFile, "|x\t\t|F%d(x)\t\t|\n", func_choice);  // вывод в файл
    fprintf(outputFile, "-----------------------\n");
    double result_value;
    switch (func_choice) {
    case 1:
        result_value = result(outputFile, x, func_choice, F1);  // Вызов функции и получение результата
        printf("F1(%.5lf) = %.5lf\n", x, result_value);  // Вывод результата на экран
        break;
    case 2:
        result_value = result(outputFile, x, func_choice, F2);  // Вызов функции и получение результата
        printf("F2(%.5lf) = %.5lf\n", x, result_value);  // Вывод результата на экран
        break;
    case 3:
        result_value = result(outputFile, x, func_choice, F3);  // Вызов функции и получение результата
        printf("F3(%.5lf) = %.5lf\n", x, result_value);  // Вывод результата на экран
        break;
    default:
        printf("Некорректный выбор функции.\n");
    }
    break;
            case 2:
                printf("Выберите функцию:\n1 - F1 (линейная)\n2 - F2 (кусочная)\n3 - F3 (ареа-синус)\n");
                scanf("%d", &func_choice);  // выбор функции
                printf("Введите начальное значение (xmin):\n");
                scanf("%lf", &xmin);  // ввод xmin
                printf("Введите шаг (dx):\n");
                scanf("%lf", &dx);  // ввод dx
                array = generating_values(xmin, dx);  // вызов функции генерации значений
                fprintf(outputFile, "|x\t\t|F%d(x)\t\t|\n", func_choice);  // вывод в файл
                fprintf(outputFile, "_________________________\n");
                for (int i = 0; i < n; i++) {  // перебор значений и вычисление функции
                    switch (func_choice) {
                    case 1:
                        fprintf(outputFile, "|%.5lf\t|%.5lf\t|\n", array[i], F1(array[i]));
                        break;
                    case 2:
                        fprintf(outputFile, "|%.5lf\t|%.5lf\t|\n", array[i], F2(array[i]));
                        break;
                    case 3:
                        fprintf(outputFile, "|%.5lf\t|%.5lf\t|\n", array[i], F3(array[i]));
                        break;
                    default:
                        printf("Некорректный выбор функции.\n");
                        break;
                    }
                }
                free(array);  // освобождение памяти
                break;
            case 3:
                printf("Введите начальное значение (xmin):\n");
                scanf("%lf", &xmin);  // ввод xmin
                printf("Введите конечное значение (xmax):\n");
                scanf("%lf", &xmax);  // ввод xmax
                printf("Введите количество значений N:\n");
                scanf("%d", &N);  // ввод N
                array = random_values(N, xmin, xmax);  // вызов функции генерации уникальных случайных значений
                fprintf(outputFile, "Сгенерированные уникальные значения:\n");
                for (int i = 0; i < N; i++) {
                    fprintf(outputFile, "%lf\n", array[i]);  // вывод значений в файл
                }
                free(array);  // освобождение памяти
                break;
            case 4:
                printf("Выберите функцию:\n1 - F1 (линейная)\n2 - F2 (кусочная)\n3 - F3 (ареа-синус)\n");
                scanf("%d", &func_choice);  // выбор функции
                printf("Введите начальное значение (xmin):\n");
                scanf("%lf", &xmin);  // ввод xmin
                printf("Введите конечное значение (xmax):\n");
                scanf("%lf", &xmax);  // ввод xmax
                printf("Введите количество значений N:\n");
                scanf("%d", &N);  // ввод N
                array = random_values(N, xmin, xmax);  // генерация уникальных значений

                double (*selected_func)(double) = NULL;  // указатель на выбранную функцию
                switch (func_choice) {
                case 1:
                    selected_func = F1;
                    break;  // выбор F1
                case 2:
                    selected_func = F2;
                    break;  // выбор F2
                case 3:
                    selected_func = F3;
                    break;  // выбор F3
                default:
                    printf("Некорректный выбор функции.\n");
                    free(array);
                    break;
                }

                if (selected_func != NULL) {
                    double* func_values = (double*)malloc(N * sizeof(double));  // выделение памяти для значений функции
                    if (!func_values) {
                        printf("Ошибка выделения памяти для массива значений функции.\n");
                        free(array);
                        break;
                    }

                    for (int i = 0; i < N; i++) {  // вычисление значений функции для каждого аргумента
                        func_values[i] = selected_func(array[i]);
                    }

                    double min_value = find_min(selected_func, func_values, N);  // нахождение минимального значения
                    double max_value = find_max(selected_func, func_values, N);  // нахождение максимального значения
                    double abs_diff = fabs(max_value - min_value);  // вычисление модуля разности

                    printf("Модуль разности максимального и минимального значений: %.5lf\n", abs_diff);  // вывод результата
                    fprintf(outputFile, "Модуль разности максимального и минимального значений: %.5lf\n", abs_diff);  // запись в файл

                    free(func_values);  // освобождение памяти
                }
                free(array);  // освобождение памяти
                break;
            case 5:
                printf("Введите точку для дифференцирования:\n");
                scanf("%lf", &x);  // ввод точки для дифференцирования
                printf("Производная функции F1 в точке %.5lf: %.5lf\n", x, differentiate(F1, x, epsilon));  // вычисление производной
                break;
            case 6:
                printf("Выход из программы.\n");
                return;
            default:
                printf("Некорректный выбор. Попробуйте снова.\n");
                break;
        }
    }
}

// функция F1
double F1(double x) {
    return cos(sqrt(fabs(x))) / atan(x * x + 5);
}

// функция F2
double F2(double x) {
    if (x < -1) {
        return 3 * x * x;  // 3x^2 для x < -1
    }
    else if (x >= -1 && x <= 1) {
        return exp(-x);  // e^{-x} для -1 <= x <= 1
    }
    else {
        return -cos(2 * x) * cos(2 * x);  // -cos^2(2x) для x > 1
    }
}

// функция F3. ареа-синус 
double F3(double x) {
    return log(x + x * x + 1);
}

// генерация массива значений с шагом dx
double* generating_values(double xmin, double dx) {
    double* values = (double*)malloc(n * sizeof(double));  // выделение памяти для массива
    if (values == NULL) {
        printf("Ошибка выделения памяти для массива значений.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        values[i] = xmin + i * dx;
    }
    return values;
}

// генерация уникальных случайных значений
double* random_values(int N, double xmin, double xmax) {
    double* array = (double*)malloc(N * sizeof(double));  // выделение памяти для массива
    if (array == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }

    int count = 0;
    while (count < N) {
        double num = xmin + (double)rand() / RAND_MAX * (xmax - xmin);  // генерация случайного числа
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (array[i] == num) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            array[count++] = num;
        }
    }
    return array;
}

// нахождение минимального значения
double find_min(double (*func)(double), double* values, int N) {
    double min_value = func(values[0]);
    for (int i = 1; i < N; i++) {
        double value = func(values[i]);
        if (value < min_value) {
            min_value = value;
        }
    }
    return min_value;
}

// нахождение максимального значения
double find_max(double (*func)(double), double* values, int N) {
    double max_value = func(values[0]);
    for (int i = 1; i < N; i++) {
        double value = func(values[i]);
        if (value > max_value) {
            max_value = value;
        }
    }
    return max_value;
}

// вычисление модуля разности
double calculate_abs_difference(double (*func)(double), double* values, int N) {
    double min_value = find_min(func, values, N);
    double max_value = find_max(func, values, N);
    return fabs(max_value - min_value);
}

// дифференцирование функции в точке
double differentiate(double (*func)(double), double x, double epsilon) {
    return (func(x + epsilon) - func(x - epsilon)) / (2 * epsilon);
}
