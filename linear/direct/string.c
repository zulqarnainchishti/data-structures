#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    char *word;
    int length;
} String;

void swap(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

String init(char *string)
{
    String new;
    int len = 0;
    while (string[len])
        len++;
    new.word = (char *)malloc((len + 1) * sizeof(char));
    char *temp = new.word;
    while (*string)
        *temp++ = *string++;
    *temp = '\0';
    new.length = len;
    return new;
}

String copy(String string)
{
    String new;
    new.length = string.length;
    new.word = (char *)malloc((new.length + 1) * sizeof(char));
    char *temp = new.word;
    while (*string.word)
        *temp++ = *string.word++;
    *temp = '\0';
    return new;
}

void clear(String *string)
{
    free(string->word);
    string->word = (char *)malloc(sizeof(char));
    *string->word = '\0';
    string->length = 0;
}

bool isEqual(String string1, String string2)
{
    if (string1.length != string2.length)
        return false;
    while (*string1.word)
    {
        if (*string1.word != *string2.word)
            return false;
        string1.word++;
        string2.word++;
    }
    return true;
}

bool isSimilar(String string1, String string2)
{
    if (string1.length != string2.length)
        return false;
    while (*string1.word)
    {
        if (*string1.word != *string2.word && abs(*string1.word - *string2.word) != 32)
            return false;
        string1.word++;
        string2.word++;
    }
    return true;
}

// int compare(String string1, String string2)
// {
//     if (string1.length != string2.length)
//         return false;
//     while (*string1.word)
//     {
//         if (*string1.word != *string2.word && abs(*string1.word - *string2.word) != 32)
//             return false;
//         string1.word++;
//         string2.word++;
//     }
//     return true;
// }

int find(String string, String substring, int index)
{
    if (string.length == 0 || substring.length == 0)
        return -1;
    if (index < 0 || index > string.length || string.length - index < substring.length)
        return -1;
    for (int i = 0; i <= string.length - substring.length - index; i++)
    {
        int j;
        for (j = 0; j < substring.length; j++)
        {
            if (string.word[index + i + j] != substring.word[j])
                break;
        }
        if (j == substring.length)
            return index + i;
    }
    return -1;
}

int count(String string, String substring)
{
    if (string.length == 0 || substring.length == 0)
        return -1;
    if (string.length < substring.length)
        return -1;
    int freq = 0;
    for (int i = 0; i <= string.length - substring.length; i++)
    {
        int j;
        for (j = 0; j < substring.length; j++)
        {
            if (string.word[i + j] != substring.word[j])
                break;
        }
        if (j == substring.length)
            freq++;
    }
    return freq;
}

bool contains(String string, String substring)
{
    if (string.length == 0 || substring.length == 0)
        return false;
    if (string.length < substring.length)
        return false;
    for (int i = 0; i <= string.length - substring.length; i++)
    {
        int j;
        for (j = 0; j < substring.length; j++)
        {
            if (string.word[i + j] != substring.word[j])
                break;
        }
        if (j == substring.length)
            return true;
    }
    return false;
}

char get(String string, int index)
{
    if (index < 0 || index >= string.length)
        return '\0';
    return string.word[index];
}

void set(String *string, char letter, int index)
{
    if (index < 0 || index >= string->length)
        return;
    string->word[index] = letter;
}

void replace(String *string, char old, char new)
{
    char *temp = string->word;
    while (*temp)
    {
        if (*temp == old)
        {
            *temp = new;
            return;
        }
        temp++;
    }
}

String insert(String string1, String string2, int index)
{
    if (index < 0)
        index = 0;
    if (index > string1.length)
        index = string1.length;
    String new;
    new.length = string1.length + string2.length;
    new.word = (char *)malloc((new.length + 1) * sizeof(char));
    char *temp = new.word;
    while (*string1.word && index-- > 0)
        *temp++ = *string1.word++;
    while (*string2.word)
        *temp++ = *string2.word++;
    while (*string1.word)
        *temp++ = *string1.word++;
    *temp = '\0';
    return new;
}

String concat(String string1, String string2)
{
    String new;
    new.length = string1.length + string2.length;
    new.word = (char *)malloc((new.length + 1) * sizeof(char));
    char *temp = new.word;
    while (*string1.word)
        *temp++ = *string1.word++;
    while (*string2.word)
        *temp++ = *string2.word++;
    *temp = '\0';
    return new;
}

String discard(String string, int index, int count)
{
    if (index < 0)
        index = 0;
    else if (index >= string.length)
        index = string.length;
    if (index + count > string.length)
        count = string.length - index;
    String new;
    new.length = string.length - count;
    new.word = (char *)malloc((new.length + 1) * sizeof(char));
    char *temp = new.word;
    while (*string.word && index-- > 0)
        *temp++ = *string.word++;
    string.word += count;
    while (*string.word)
        *temp++ = *string.word++;
    *temp = '\0';
    return new;
}

String slice(String string, int start, int end)
{
    if (start < 0)
        start = 0;
    if (end > string.length)
        end = string.length;
    String new;
    if (start >= end)
    {
        new.word = (char *)malloc(sizeof(char));
        *new.word = '\0';
        new.length = 0;
        return new;
    }
    int len = end - start;
    new.length = len;
    new.word = (char *)malloc((len + 1) * sizeof(char));
    char *temp = new.word;
    string.word += start;
    while (*string.word && len-- > 0)
        *temp++ = *string.word++;
    *temp = '\0';
    return new;
}

bool startsWith(String string, String substring)
{
    if (string.length == 0 || substring.length == 0 || substring.length > string.length)
        return false;
    while (*substring.word)
        if (*string.word++ != *substring.word++)
            return false;
    return true;
}

bool endsWith(String string, String substring)
{
    if (string.length == 0 || substring.length == 0 || substring.length > string.length)
        return false;
    int diff = string.length - substring.length;
    string.word += diff;
    while (*substring.word)
        if (*string.word++ != *substring.word++)
            return false;
    return true;
}

String reverse(String string)
{
    String new;
    new.length = string.length;
    new.word = (char *)malloc((new.length + 1) * sizeof(char));
    for (int i = 0; i < new.length; i++)
        new.word[i] = string.word[new.length - 1 - i];
    new.word[new.length] = '\0';
    return new;
}

String repeat(String string, int times)
{
    if (times <= 0)
        times = 1;
    String new;
    new.length = string.length * times;
    new.word = (char *)malloc((new.length + 1) * sizeof(char));
    char *newTemp = new.word;
    for (int i = 0; i < times; i++)
    {
        char *oldTemp = string.word;
        while (*oldTemp)
            *newTemp++ = *oldTemp++;
    }
    *newTemp = '\0';
    return new;
}

bool isUpper(String string)
{
    while (*string.word)
    {
        if (*string.word >= 'a' && *string.word <= 'z')
            return false;
        string.word++;
    }
    return true;
}

String toUpper(String string)
{
    String new;
    new.length = string.length;
    new.word = (char *)malloc((new.length + 1) * sizeof(char));
    char *temp = new.word;
    while (*string.word)
    {
        if (*string.word >= 'a' && *string.word <= 'z')
            *temp = *string.word - 32;
        else
            *temp = *string.word;
        temp++;
        string.word++;
    }
    *temp = '\0';
    return new;
}

bool isLower(String string)
{
    while (*string.word)
    {
        if (*string.word >= 'A' && *string.word <= 'Z')
            return false;
        string.word++;
    }
    return true;
}

String toLower(String string)
{
    String new;
    new.length = string.length;
    new.word = (char *)malloc((new.length + 1) * sizeof(char));
    char *temp = new.word;
    while (*string.word)
    {
        if (*string.word >= 'A' && *string.word <= 'Z')
            *temp = *string.word + 32;
        else
            *temp = *string.word;
        temp++;
        string.word++;
    }
    *temp = '\0';
    return new;
}

bool isAlphabet(String string)
{
    while (*string.word)
    {
        if (!(*string.word >= 'A' && *string.word <= 'Z' || *string.word >= 'a' && *string.word <= 'z'))
            return false;
        string.word++;
    }
    return true;
}

bool isNumeric(String string)
{
    while (*string.word)
    {
        if (!(*string.word >= '0' && *string.word <= '9'))
            return false;
        string.word++;
    }
    return true;
}

bool isAlphanum(String string)
{
    while (*string.word)
    {
        if (!(*string.word >= 'A' && *string.word <= 'Z' || *string.word >= 'a' && *string.word <= 'z' || *string.word >= '0' && *string.word <= '9'))
            return false;
        string.word++;
    }
    return true;
}

bool isWhitespace(String string)
{
    while (*string.word)
    {
        if (*string.word == ' ' || *string.word == '\t' || *string.word == '\n' || *string.word == '\v' || *string.word == '\f' || *string.word == '\r')
            return false;
        string.word++;
    }
    return true;
}

String leftAlign(String string, char filler, int length)
{
    if (length < string.length)
        length = string.length;
    String new;
    new.length = length;
    new.word = (char *)malloc((length + 1) * sizeof(char));
    char *temp = new.word;
    while (*string.word && length-- > 0)
        *temp++ = *string.word++;
    while (length-- > 0)
        *temp++ = filler;
    *temp = '\0';
    return new;
}

String centerAlign(String string, char filler, int length)
{
    if (length < string.length)
        length = string.length;
    String new;
    new.length = length;
    new.word = (char *)malloc((length + 1) * sizeof(char));
    char *temp = new.word;
    int len2 = string.length;
    int len1 = (length - len2) / 2;
    int len3 = length - len1 - len2;
    while (len1-- > 0)
        *temp++ = filler;
    while (len2-- > 0)
        *temp++ = *string.word++;
    while (len3-- > 0)
        *temp++ = filler;
    *temp = '\0';
    return new;
}

String rightAlign(String string, char filler, int length)
{
    if (length < string.length)
        length = string.length;
    String new;
    new.length = length;
    new.word = (char *)malloc((length + 1) * sizeof(char));
    char *temp = new.word;
    int len2 = string.length;
    int len1 = length - len2;
    while (len1-- > 0)
        *temp++ = filler;
    while (len2-- > 0)
        *temp++ = *string.word++;
    *temp = '\0';
    return new;
}

String trim(String string, char letter)
{
    int start = 0;
    int end = string.length - 1;
    while (start <= end && string.word[start] == letter)
        start++;
    while (start <= end && string.word[end] == letter)
        end--;
    int len = end - start + 1;
    String new;
    new.length = len;
    new.word = (char *)malloc((len + 1) * sizeof(char));
    char *temp = new.word;
    string.word += start;
    while (len-- > 0)
        *temp++ = *string.word++;
    *temp = '\0';
    return new;
}

String *split(String string, char delimiter, int *size)
{
    String *array = NULL;
    *size = 0;
    int start = 0;
    for (int i = 0; i <= string.length; i++)
    {
        if (string.word[i] == delimiter || string.word[i] == '\0')
        {
            if (start != i)
            {
                int len = i - start;
                String new;
                new.length = len;
                new.word = (char *)malloc((len + 1) * sizeof(char));
                for (int j = 0; j < len; j++)
                    new.word[j] = string.word[start + j];
                new.word[len] = '\0';
                array = (String *)realloc(array, (*size + 1) * sizeof(String));
                array[*size] = new;
                (*size)++;
            }
            start = i + 1;
        }
    }
    return array;
}

String join(String *array, char delimiter, int size)
{
    String new;
    int len = 0;
    for (int i = 0; i < size; i++)
        len += array[i].length;
    new.length = len + size - 1;
    new.word = (char *)malloc((new.length + 1) * sizeof(char));
    char *tempNew = new.word;
    for (int i = 0; i < size; i++)
    {
        char *tempOld = array[i].word;
        while (*tempOld)
            *tempNew++ = *tempOld++;
        if (i < size - 1)
            *tempNew++ = delimiter;
    }
    *tempNew = '\0';
    return new;
}

int numericEquivalent(char c)
{
    if (c >= '0' && c <= '9')
        return (int)(c - '0');
    if (c >= 'A' && c <= 'Z')
        return (int)(c - 'A' + 10);
    if (c >= 'a' && c <= 'z')
        return (int)(c - 'a' + 10);
    return -1;
}

int parse(String string, int base)
{
    if (base < 2 || base > 36)
        return -1;
    int num = 0;
    for (int i = 0; i < string.length; i++)
    {
        int temp = numericEquivalent(string.word[i]);
        if (temp == -1)
            continue;
        else if (temp >= base)
            return -1;
        num = num * base + temp;
    }
    return num;
}

char characterEquivalent(int n)
{
    if (n >= 0 && n <= 9)
        return (char)(n + '0');
    if (n >= 10 && n <= 35)
        return (char)(n - 10 + 'A');
    return '\0';
}

String convert(int num, int base)
{
    String result;
    if (num < 0 || base < 2 || base > 36)
    {
        result.length = -1;
        result.word = NULL;
        return result;
    }
    if (num == 0)
    {
        result.length = 1;
        result.word = (char *)malloc(2 * sizeof(char));
        result.word[0] = '0';
        result.word[1] = '\0';
        return result;
    }
    int len = 0;
    int temp = num;
    while (temp)
    {
        len++;
        temp /= base;
    }
    result.length = len;
    result.word = (char *)malloc((len + 1) * sizeof(char));
    result.word[len] = '\0';
    for (int i = len - 1; i >= 0; i--)
    {
        result.word[i] = characterEquivalent(num % base);
        num /= base;
    }
    return result;
}

int main() {
    // Initialize strings
    String s1 = init("Hello");
    String s2 = init("World");
    String s3 = init("hello");

    // Test isEqual
    printf("isEqual(s1, s2): %d\n", isEqual(s1, s2)); // 0
    printf("isEqual(s1, s1): %d\n", isEqual(s1, s1)); // 1

    // Test isSimilar (case-insensitive comparison)
    printf("isSimilar(s1, s3): %d\n", isSimilar(s1, s3)); // 1

    // Test find
    String s4 = init("lo");
    printf("find(s1, s4, 0): %d\n", find(s1, s4, 0)); // 3

    // Test count
    String s5 = init("l");
    printf("count(s1, s5): %d\n", count(s1, s5)); // 2

    // Test contains
    printf("contains(s1, s5): %d\n", contains(s1, s5)); // 1

    // Test get and set
    printf("get(s1, 1): %c\n", get(s1, 1)); // e
    set(&s1, 'a', 1);
    printf("After set: %s\n", s1.word); // Hallo

    // Test concat
    String combined = concat(s1, s2);
    printf("concat: %s\n", combined.word); // HalloWorld

    // Test reverse
    String rev = reverse(s1);
    printf("reverse: %s\n", rev.word); // ollaH

    // Test repeat
    String repeated = repeat(s5, 3);
    printf("repeat 'l' 3 times: %s\n", repeated.word); // lll

    // Test toUpper and toLower
    String upper = toUpper(s3);
    printf("toUpper: %s\n", upper.word); // HELLO

    String lower = toLower(upper);
    printf("toLower: %s\n", lower.word); // hello

    // Test startsWith and endsWith
    printf("startsWith(s1, s5): %d\n", startsWith(s1, s5)); // 0
    printf("endsWith(s1, s5): %d\n", endsWith(s1, s5)); // 1

    // Test trim
    String padded = init("xxxHelloxxx");
    String trimmed = trim(padded, 'x');
    printf("trim: %s\n", trimmed.word); // Hello

    // Test split and join
    String sentence = init("A,B,C");
    int size;
    String *parts = split(sentence, ',', &size);
    printf("split parts:\n");
    for (int i = 0; i < size; i++)
        printf("  %s\n", parts[i].word); // A B C

    String joined = join(parts, '-', size);
    printf("join: %s\n", joined.word); // A-B-C

    // Test parse and convert
    String numString = init("1A");
    int num = parse(numString, 16);
    printf("parse: %d\n", num); // 26

    String converted = convert(26, 16);
    printf("convert: %s\n", converted.word); // 1A

    return 0;
}
