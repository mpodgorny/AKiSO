#include <unistd.h>
#include <malloc.h>

/**
 * (10pt) Napisz w języku C własną implementacje funkcji printf i scanf (nazwijmy je myprintf i myscanf).
 * Funkcje nie mogą wykorzystywać, żadnych funkcji bibliotecznych (atoi, fprintf, fscanf itp.) oraz makr va_start, va_arg i va_end
 * (np. możesz skorzystać z wyjaśnienia tutaj) oraz mogą wykorzystać wywołania systemowe read i write z odpowiednim standardowym deskryptorem.
 * Program należy skompilować na maszynę 32-bitową tzn. gcc -m32 np. dla 64-bitowego systemu ArchLinux trzeba zainstalować pakiet gcc-multilib z repozytorium multilib.
 * W funkcjach wystarczy zaimplementować
 *
 * "%s" wyświetla ciąg znaków,
 * "%d" liczbę w systemie dziesiętnym,
 * "%x" liczbę w systemie szesnastkowych
 * "%b" liczbę w systemie binarnym.
 * @return
 */

/**
 * int printf(const char *format, ...);
 * const char *format = format, w jakim zostaną wypisane następne argumenty
 * Jeżeli funkcje zakończą się sukcesem zwracają liczbę znaków w tekście (wypisanym na standardowe wyjście, do podanego strumienia lub tablicy znaków) nie wliczając kończącego '\0'. W przeciwnym wypadku zwracana jest liczba ujemna.
 */
int nr_to_string(int number, int base) {

    char buff[128];
    char *values="0123456789ABCDEF";
    int i = 0, length =0;
    int is_negative=1;
    if (number < 0) {
        number = 0 - number;
        is_negative=0;
        length++;
    }
    do {
        buff[i] = values[number % base];
        i++;
        number = number / base;
    } while (number > 0);
    char *ptr = buff;
    ptr+=i-1;
    if(is_negative==0) {
        write(1, "-", 1);
    }
    while (i > 0) {
        write(1, ptr, 1);
        length++;
        i--;
        ptr--;
    }
    return length;
}

int string_to_int(char* string, int base) {
    int n = 0;

    while ( *string != '\0' && (*string >= '0' && *string <= '9') || (*string >= 'a' && *string <= 'f') ) {
        if(*string >= 'a' && *string <= 'f') {
            n = n * base + (*string - 'a' + 10);
        } else {
            n = n * base + (*string - '0');
        }
        string++;
    }

    return n;
}

int my_printf(const char * format, ... ) {
    int length = 0;
    //macro va_start
    char* p = (char *) &format + sizeof format;
    int tempInt;
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch(*format) {
                case 'x': {
                    tempInt = (*(int *) p);  //va_arg
                    p += sizeof(int);
                    length+=nr_to_string(tempInt, 16);
                    break;
                }
                case 'd': {
                    tempInt = (*(int *) p);
                    p += sizeof(int);
                    length+=nr_to_string(tempInt, 10);
                    break;
                }
                case 's': {
                    char *s = *((char **)p);
                    while(s[0] != '\0') {
                        write(1, s, sizeof(char));
                        s++;
                        length++;
                    }
                    p += sizeof(char*);
                    break;
                }
                case 'b': {
                    tempInt = (*(int *) p);
                    p += sizeof(int);
                    length+=nr_to_string(tempInt, 2);
                    break;
                }
            }
            format++;
        } else {
            write(1, format, 1);
            length++;
            format++;
        }
    }

    p = NULL; //va_end

    return length;
}

void my_scanf(char* format, ...) {
    char* string = malloc(1024);
    int* tempInt;
    char** tempStr = malloc(1024);
    //macro va_start
    char* p = (char *) &format + sizeof format;

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': {
                    tempInt = (int*) (*(int*) p);
                    p += sizeof(int*);
                    read(0, string, 1024);
                    *tempInt = string_to_int(string, 10);
                    break;
                }
                case 'b': {
                    tempInt = (int*) (*(int*) p);
                    p += sizeof(int*);
                    read(0, string, 1024);
                    *tempInt = string_to_int(string, 2);
                    break;
                }
                case 'x': {
                    tempInt = (int*) (*(int*) p);
                    p += sizeof(int*);
                    read(0, string, 1024);
                    *tempInt = string_to_int(string, 16);
                    break;
                }
                case 's': {
                    tempStr = (char**) (*(char**) p);
                    p += sizeof(*tempStr);
                    read(0, string, 1024);
                    *tempStr = string;
                    break;
                }
            }
        }
        format++;
    }
}

int main() {
    int a, b, c;
    char* s;

    my_printf("Podaj dwie liczby do pomnożenia oraz jedno zdanie: \n");
    my_scanf("%d%d%s",&a,&b,&s);
    c= a*b;
    my_printf("\n%d*%d=%d.\nWynik zapisany binarnie: %b\nszesnastkowo: %x\nTwoje zdanie: %s",a,b,c,c,c,s);

    return 0;
}