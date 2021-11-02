#include "ng_utils.h"
#include "nema_core.h"

//Convert a float to string, with "precision" decimal digits
void float2str(char *str, int size, float f, int precision) {
    //precision must be [0, 4]
    //str size should be enough

    int tmp_prec = precision;
    int multiplier = 1;
    float f_round_bias = f < 0.f ? -0.5f : 0.5f;

    while(tmp_prec > 0) {
        multiplier *= 10;
        --tmp_prec;
    }

    int tmp = f*(float)multiplier+f_round_bias;
    if (tmp < 0) tmp = -tmp;

    str[size-1] = '\0';
    int i = size-2;

    if (precision > 0) {
        do {
            str[i--] = '0' + (tmp%10);
            tmp /= 10;
            --precision;
        } while(precision > 0);
        str[i--] = '.';
    }

    do {
        str[i--] = '0' + (tmp%10);
        tmp /= 10;
    } while (tmp != 0);

    if (i == size-2)
        str[i--] = '0';
    if (f < 0.f)
        str[i--] = '-';

    ++i;
    if (i > 0) {
        int j = 0;
        while(i <= size-1) {
            str[j] = str[i];
            ++j;
            ++i;
        }
    }
}

//concatenate strings str1 and str2 to str1
void concatenate_strings(char *str1, char *str2) {
    int i,j;
    i = 0;
    while ( str1[i] != '\0' ) {
        i++;
    }

    for ( j=0; str2[j]!='\0'; ++j, ++i ) {
       str1[i] = str2[j];
    }
}

// Round up the integer number "numToRound" to the closer multiple of "multiple"
int round_up(int numToRound, int multiple) {
    if ( multiple == 0 ) {
        return numToRound;
    }

    int remainder = numToRound % multiple;
    if ( remainder == 0 ) {
        return numToRound;
    }

    return numToRound + multiple - remainder;
}