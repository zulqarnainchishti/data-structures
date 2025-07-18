#ifndef STRING_LIB_H
#define STRING_LIB_H

#include <stdio.h>   // Required for perror and EXIT_FAILURE
#include <stdlib.h>  // Required for malloc, realloc, free, and exit
#include <stdbool.h> // Required for bool type
#include <string.h>  // Required for strlen
#include <math.h>    // Required for abs in isSimilar (though stdlib.h provides abs for ints)

/**
 * @brief Represents a dynamically allocated string.
 * @param word A pointer to the character array storing the string.
 * @param length The current length of the string (excluding the null terminator).
 */
typedef struct
{
    char *word;
    int length;
} String;

/**
 * @brief Initializes a new String object from a C-style string.
 * Allocates memory for the new string and copies the content.
 * @param input The C-style string to initialize from.
 * @return A new String object.
 */
String init(const char *input)
{
    if (input == NULL)
    {
        perror("Invalid input for String");
        exit(EXIT_FAILURE);
    }
    String result;
    result.length = strlen(input);
    result.word = (char *)malloc((result.length + 1) * sizeof(char));
    if (result.word == NULL)
    {
        perror("Failed to initialize String");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < result.length; i++)
        result.word[i] = input[i];
    result.word[result.length] = '\0';
    return result;
}

/**
 * @brief Creates a deep copy of an existing String object.
 * Allocates new memory and copies the content.
 * @param string The String object to copy.
 * @return A new String object that is a copy of the input.
 */
String copy(String string)
{
    String copied;
    copied.length = string.length;
    copied.word = (char *)malloc((copied.length + 1) * sizeof(char));
    if (copied.word == NULL)
    {
        perror("Failed to copy String");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < copied.length; i++)
        copied.word[i] = string.word[i];
    copied.word[copied.length] = '\0';
    return copied;
}

/**
 * @brief Clears the content of a String object, making it an empty string.
 * Frees the existing memory and allocates new memory for a null terminator.
 * @param string A pointer to the String object to clear.
 */
void destroy(String *string)
{
    if (string == NULL)
        return;
    free(string->word);
    string->word = NULL;
    string->length = 0;
}

/**
 * @brief Retrieves the character at a specified index in the string.
 * @param string The String object to access.
 * @param index The index of the character to retrieve.
 * @return The character at the given index, or '\0' if the index is out of bounds.
 */
char get(const String string, int index)
{
    if (index < 0 || index >= string.length)
        return '\0';
    return string.word[index];
}

/**
 * @brief Replaces a portion of a string with a new substring.
 * Creates a new String object with the modified content.
 * @param string The original String object.
 * @param substring The substring to insert.
 * @param start The starting index of the portion to replace.
 * @param end The ending index (exclusive) of the portion to replace.
 * @return A new String object with the replaced content.
 */
String set(const String string, const String substring, int start, int end)
{
    if (start < 0)
        start = 0;
    if (end > string.length)
        end = string.length;
    if (start >= end)
        return copy(string);

    int newLength = string.length - (end - start) + substring.length;
    char *newWord = (char *)malloc((newLength + 1) * sizeof(char));
    if (newWord == NULL)
    {
        perror("Failed to allocate memory in set");
        exit(EXIT_FAILURE);
    }

    char *temp = newWord;
    for (int i = 0; i < start; i++)
        *temp++ = string.word[i];
    for (int i = 0; i < substring.length; i++)
        *temp++ = substring.word[i];
    for (int i = end; i < string.length; i++)
        *temp++ = string.word[i];
    *temp = '\0';

    String result;
    result.word = newWord;
    result.length = newLength;
    return result;
}

/**
 * @brief Replaces the first occurrence of an 'oldstring' with a 'newstring' starting from a given index.
 * Creates a new String object with the modified content.
 * @param string The original String object.
 * @param oldstring The substring to find and replace.
 * @param newstring The substring to replace with.
 * @param index The starting index to begin the search for 'oldstring'.
 * @return A new String object with the replaced content, or a copy of the original if no match is found.
 */
String replace(const String string, const String oldstring, const String newstring, int index)
{
    if (index < 0)
        index = 0;
    if (index > string.length)
        index = string.length;

    int match = -1;
    for (int i = index; i <= string.length - oldstring.length; i++)
    {
        bool found = true;
        for (int j = 0; j < oldstring.length; j++)
        {
            if (string.word[i + j] != oldstring.word[j])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            match = i;
            break;
        }
    }

    if (match == -1)
        return copy(string);

    int newLength = string.length - oldstring.length + newstring.length;
    char *newWord = (char *)malloc((newLength + 1) * sizeof(char));
    if (newWord == NULL)
    {
        perror("Failed to allocate memory in replace");
        exit(EXIT_FAILURE);
    }

    char *temp = newWord;
    for (int i = 0; i < match; i++)
        *temp++ = string.word[i];
    for (int i = 0; i < newstring.length; i++)
        *temp++ = newstring.word[i];
    for (int i = match + oldstring.length; i < string.length; i++)
        *temp++ = string.word[i];
    *temp = '\0';

    String result;
    result.word = newWord;
    result.length = newLength;
    return result;
}

/**
 * @brief Inserts a substring into another string at a specified index.
 * Creates a new String object with the inserted content.
 * @param string1 The original String object.
 * @param string2 The substring to insert.
 * @param index The index at which to insert string2.
 * @return A new String object with string2 inserted into string1.
 */
String insert(const String string1, const String string2, int index)
{
    if (index < 0)
        index = 0;
    if (index > string1.length)
        index = string1.length;

    int newLength = string1.length + string2.length;
    char *newWord = (char *)malloc((newLength + 1) * sizeof(char));
    if (newWord == NULL)
    {
        perror("Failed to allocate memory in insert");
        exit(EXIT_FAILURE);
    }

    char *temp = newWord;
    for (int i = 0; i < index; i++)
        *temp++ = string1.word[i];
    for (int i = 0; i < string2.length; i++)
        *temp++ = string2.word[i];
    for (int i = index; i < string1.length; i++)
        *temp++ = string1.word[i];
    *temp = '\0';

    String result;
    result.word = newWord;
    result.length = newLength;
    return result;
}

/**
 * @brief Concatenates two String objects.
 * Creates a new String object containing the combined content.
 * @param string1 The first String object.
 * @param string2 The second String object.
 * @return A new String object that is the concatenation of string1 and string2.
 */
String concat(const String string1, const String string2)
{
    int newLength = string1.length + string2.length;
    char *newWord = (char *)malloc((newLength + 1) * sizeof(char));
    if (newWord == NULL)
    {
        perror("Failed to allocate memory in concat");
        exit(EXIT_FAILURE);
    }

    char *temp = newWord;
    for (int i = 0; i < string1.length; i++)
        *temp++ = string1.word[i];
    for (int i = 0; i < string2.length; i++)
        *temp++ = string2.word[i];
    *temp = '\0';

    String result;
    result.word = newWord;
    result.length = newLength;
    return result;
}

/**
 * @brief Discards a specified quantity of characters from a string starting at an index.
 * Creates a new String object with the discarded content.
 * @param string The original String object.
 * @param index The starting index from which to discard characters.
 * @param quantity The number of characters to discard.
 * @return A new String object with the characters removed.
 */
String discard(const String string, int index, int quantity)
{
    if (index < 0)
        index = 0;
    else if (index > string.length)
        index = string.length;

    if (index + quantity > string.length)
        quantity = string.length - index;

    int newLength = string.length - quantity;
    char *newWord = (char *)malloc((newLength + 1) * sizeof(char));
    if (newWord == NULL)
    {
        perror("Failed to allocate memory in discard");
        exit(EXIT_FAILURE);
    }

    char *temp = newWord;
    for (int i = 0; i < index; i++)
        *temp++ = string.word[i];
    for (int i = index + quantity; i < string.length; i++)
        *temp++ = string.word[i];
    *temp = '\0';

    String result;
    result.word = newWord;
    result.length = newLength;
    return result;
}

/**
 * @brief Extracts a slice (substring) from a String object.
 * Creates a new String object representing the slice.
 * @param string The original String object.
 * @param start The starting index of the slice (inclusive).
 * @param end The ending index of the slice (exclusive).
 * @return A new String object containing the sliced portion.
 */
String slice(const String string, int start, int end)
{
    if (start < 0)
        start = 0;
    if (end > string.length)
        end = string.length;
    if (start >= end)
        return init(""); // Return an empty string if the slice is invalid or empty

    int newLength = end - start;
    char *newWord = (char *)malloc((newLength + 1) * sizeof(char));
    if (newWord == NULL)
    {
        perror("Failed to allocate memory in slice");
        exit(EXIT_FAILURE);
    }

    char *temp = newWord;
    for (int i = start; i < end; i++)
        *temp++ = string.word[i];
    *temp = '\0';

    String result;
    result.word = newWord;
    result.length = newLength;
    return result;
}

/**
 * @brief Compares two String objects for exact equality (case-sensitive).
 * @param string1 The first String object.
 * @param string2 The second String object.
 * @return True if the strings are identical, false otherwise.
 */
bool isEqual(const String string1, const String string2)
{
    if (string1.length != string2.length)
        return false;
    const char *ptr1 = string1.word;
    const char *ptr2 = string2.word;
    while (*ptr1)
    {
        if (*ptr1 != *ptr2)
            return false;
        ptr1++;
        ptr2++;
    }
    return true;
}

/**
 * @brief Compares two String objects for similarity (case-insensitive for alphabetic characters).
 * Allows for a difference of 32 in ASCII value for 'similar' characters (e.g., 'a' and 'A').
 * @param string1 The first String object.
 * @param string2 The second String object.
 * @return True if the strings are similar, false otherwise.
 */
bool isSimilar(const String string1, const String string2)
{
    if (string1.length != string2.length)
        return false;
    const char *ptr1 = string1.word;
    const char *ptr2 = string2.word;
    while (*ptr1)
    {
        // Check for exact match or difference of 32 (ASCII for case difference)
        if (*ptr1 != *ptr2 && abs(*ptr1 - *ptr2) != 32)
            return false;
        ptr1++;
        ptr2++;
    }
    return true;
}

/**
 * @brief Compares two String objects lexicographically.
 * @param string1 The first String object.
 * @param string2 The second String object.
 * @return A negative value if string1 is less than string2,
 * a positive value if string1 is greater than string2,
 * and 0 if the strings are equal.
 */
int compare(const String string1, const String string2)
{
    int i = 0;
    while (i < string1.length && i < string2.length)
    {
        if (string1.word[i] != string2.word[i])
            return string1.word[i] - string2.word[i];
        i++;
    }
    return string1.length - string2.length;
}

/**
 * @brief Checks if a string starts with a given substring.
 * @param string The main String object.
 * @param substring The substring to check for at the beginning.
 * @return True if the string starts with the substring, false otherwise.
 */
bool startsWith(const String string, const String substring)
{
    if (substring.length == 0) // An empty substring always "starts" with any string
        return true;
    if (substring.length > string.length)
        return false;
    const char *ptr1 = string.word;
    const char *ptr2 = substring.word;
    while (*ptr2)
        if (*ptr1++ != *ptr2++)
            return false;
    return true;
}

/**
 * @brief Checks if a string ends with a given substring.
 * @param string The main String object.
 * @param substring The substring to check for at the end.
 * @return True if the string ends with the substring, false otherwise.
 */
bool endsWith(const String string, const String substring)
{
    if (substring.length == 0) // An empty substring always "ends" with any string
        return true;
    if (substring.length > string.length)
        return false;
    int offset = string.length - substring.length;
    const char *ptr1 = string.word + offset;
    const char *ptr2 = substring.word;
    while (*ptr2)
        if (*ptr1++ != *ptr2++)
            return false;
    return true;
}

/**
 * @brief Checks if a string contains a specified substring.
 * @param string The main String object.
 * @param substring The substring to search for.
 * @return True if the string contains the substring, false otherwise.
 */
bool contains(const String string, const String substring)
{
    if (substring.length == 0)
        return true;
    if (string.length < substring.length)
        return false;

    for (int i = 0; i <= string.length - substring.length; i++)
    {
        bool found = true;
        for (int j = 0; j < substring.length; j++)
        {
            if (string.word[i + j] != substring.word[j])
            {
                found = false;
                break;
            }
        }
        if (found)
            return true;
    }
    return false;
}

/**
 * @brief Finds the first occurrence of a substring within a string, starting from a given index.
 * @param string The main String object.
 * @param substring The substring to search for.
 * @param index The starting index for the search.
 * @return The starting index of the first occurrence, or -1 if not found.
 */
int find(const String string, const String substring, int index)
{
    if (string.length == 0 || substring.length == 0)
        return -1;
    if (index < 0 || index > string.length - substring.length)
        return -1;

    for (int i = index; i <= string.length - substring.length; i++)
    {
        bool found = true;
        for (int j = 0; j < substring.length; j++)
        {
            if (string.word[i + j] != substring.word[j])
            {
                found = false;
                break;
            }
        }
        if (found)
            return i;
    }
    return -1;
}

/**
 * @brief Counts the number of non-overlapping occurrences of a substring within a string.
 * @param string The main String object.
 * @param substring The substring to count.
 * @return The number of occurrences, or -1 if inputs are invalid.
 */
int count(const String string, const String substring)
{
    if (string.length == 0 || substring.length == 0)
        return -1;
    if (string.length < substring.length)
        return -1;

    int freq = 0;
    for (int i = 0; i <= string.length - substring.length; i++)
    {
        bool found = true;
        for (int j = 0; j < substring.length; j++)
        {
            if (string.word[i + j] != substring.word[j])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            freq++;
            i += substring.length - 1; // Move past the found substring to avoid overlapping counts
        }
    }
    return freq;
}

/**
 * @brief Checks if all characters in the string are uppercase alphabets.
 * @param string The String object to check.
 * @return True if all characters are uppercase, false otherwise.
 */
bool isUpper(const String string)
{
    const char *ptr = string.word;
    while (*ptr)
    {
        if (*ptr >= 'a' && *ptr <= 'z') // If any lowercase found
            return false;
        ptr++;
    }
    return true;
}

/**
 * @brief Checks if all characters in the string are lowercase alphabets.
 * @param string The String object to check.
 * @return True if all characters are lowercase, false otherwise.
 */
bool isLower(const String string)
{
    const char *ptr = string.word;
    while (*ptr)
    {
        if (*ptr >= 'A' && *ptr <= 'Z') // If any uppercase found
            return false;
        ptr++;
    }
    return true;
}

/**
 * @brief Checks if all characters in the string are alphabetic (uppercase or lowercase).
 * @param string The String object to check.
 * @return True if all characters are alphabetic, false otherwise.
 */
bool isAlphabet(const String string)
{
    const char *ptr = string.word;
    while (*ptr)
    {
        if (!((*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z')))
            return false;
        ptr++;
    }
    return true;
}

/**
 * @brief Checks if all characters in the string are numeric digits ('0'-'9').
 * @param string The String object to check.
 * @return True if all characters are numeric, false otherwise.
 */
bool isNumeric(const String string)
{
    const char *ptr = string.word;
    while (*ptr)
    {
        if (!(*ptr >= '0' && *ptr <= '9'))
            return false;
        ptr++;
    }
    return true;
}

/**
 * @brief Checks if all characters in the string are alphanumeric (alphabetic or numeric).
 * @param string The String object to check.
 * @return True if all characters are alphanumeric, false otherwise.
 */
bool isAlphanum(const String string)
{
    const char *ptr = string.word;
    while (*ptr)
    {
        if (!((*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z') || (*ptr >= '0' && *ptr <= '9')))
            return false;
        ptr++;
    }
    return true;
}

/**
 * @brief Checks if all characters in the string are whitespace characters.
 * @param string The String object to check.
 * @return True if all characters are whitespace, false otherwise.
 */
bool isWhitespace(const String string)
{
    const char *ptr = string.word;
    while (*ptr)
    {
        if (!(*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\v' || *ptr == '\f' || *ptr == '\r'))
            return false;
        ptr++;
    }
    return true;
}

/**
 * @brief Reverses a String object.
 * Creates a new String object with the reversed content.
 * @param string The String object to reverse.
 * @return A new String object that is the reverse of the input.
 */
String reverse(const String string)
{
    String reversed;
    reversed.length = string.length;
    reversed.word = (char *)malloc((reversed.length + 1) * sizeof(char));
    if (reversed.word == NULL)
    {
        perror("Failed to allocate memory in reverse");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < reversed.length; i++)
        reversed.word[i] = string.word[reversed.length - 1 - i];
    reversed.word[reversed.length] = '\0';
    return reversed;
}

/**
 * @brief Repeats a String object a specified number of times.
 * Creates a new String object containing the repeated content.
 * @param string The String object to repeat.
 * @param times The number of times to repeat the string. If <= 0, defaults to 1.
 * @return A new String object with the repeated content.
 */
String repeat(const String string, int times)
{
    if (times <= 0)
        times = 1;

    String repeated;
    repeated.length = string.length * times;
    repeated.word = (char *)malloc((repeated.length + 1) * sizeof(char));
    if (repeated.word == NULL)
    {
        perror("Failed to allocate memory in repeat");
        exit(EXIT_FAILURE);
    }

    char *newTemp = repeated.word;
    for (int i = 0; i < times; i++)
    {
        const char *oldTemp = string.word;
        while (*oldTemp)
            *newTemp++ = *oldTemp++;
    }
    *newTemp = '\0';
    return repeated;
}

/**
 * @brief Converts all lowercase alphabetic characters in a string to uppercase.
 * Creates a new String object with the uppercased content.
 * @param string The String object to convert.
 * @return A new String object with all alphabetic characters in uppercase.
 */
String toUpper(const String string)
{
    String uppercased;
    uppercased.length = string.length;
    uppercased.word = (char *)malloc((uppercased.length + 1) * sizeof(char));
    if (uppercased.word == NULL)
    {
        perror("Failed to allocate memory in toUpper");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < string.length; i++)
    {
        char ch = string.word[i];
        uppercased.word[i] = (ch >= 'a' && ch <= 'z') ? (ch - 32) : ch;
    }
    uppercased.word[uppercased.length] = '\0';
    return uppercased;
}

/**
 * @brief Converts all uppercase alphabetic characters in a string to lowercase.
 * Creates a new String object with the lowercased content.
 * @param string The String object to convert.
 * @return A new String object with all alphabetic characters in lowercase.
 */
String toLower(const String string)
{
    String lowercased;
    lowercased.length = string.length;
    lowercased.word = (char *)malloc((lowercased.length + 1) * sizeof(char));
    if (lowercased.word == NULL)
    {
        perror("Failed to allocate memory in toLower");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < string.length; i++)
    {
        char ch = string.word[i];
        lowercased.word[i] = (ch >= 'A' && ch <= 'Z') ? (ch + 32) : ch;
    }
    lowercased.word[lowercased.length] = '\0';
    return lowercased;
}

/**
 * @brief Left-aligns a string within a specified total length, padding with a given character.
 * If the string is longer than the desired length, it is not truncated.
 * Creates a new String object with the aligned content.
 * @param string The String object to align.
 * @param padding The character to use for padding.
 * @param length The total desired length of the aligned string.
 * @return A new String object that is left-aligned.
 */
String leftAlign(const String string, char padding, int length)
{
    if (length < string.length)
        length = string.length;

    String result;
    result.length = length;
    result.word = (char *)malloc((length + 1) * sizeof(char));
    if (!result.word)
    {
        perror("Failed to allocate memory in leftAlign");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < string.length; i++)
        result.word[i] = string.word[i];
    for (int i = string.length; i < length; i++)
        result.word[i] = padding;
    result.word[length] = '\0';

    return result;
}

/**
 * @brief Centers a string within a specified total length, padding with a given character.
 * If the string is longer than the desired length, it is not truncated.
 * Creates a new String object with the aligned content.
 * @param string The String object to align.
 * @param padding The character to use for padding.
 * @param length The total desired length of the aligned string.
 * @return A new String object that is centered.
 */
String centerAlign(String string, char padding, int length)
{
    if (length < string.length)
        length = string.length;

    String result;
    result.length = length;
    result.word = (char *)malloc((length + 1) * sizeof(char));
    if (!result.word)
    {
        perror("Failed to allocate memory in centerAlign");
        exit(EXIT_FAILURE);
    }

    int left = (length - string.length) / 2;
    int right = length - string.length - left;

    for (int i = 0; i < left; i++)
        result.word[i] = padding;
    for (int i = 0; i < string.length; i++)
        result.word[left + i] = string.word[i];
    for (int i = 0; i < right; i++)
        result.word[string.length + left + i] = padding; // Corrected index for right padding
    result.word[length] = '\0';

    return result;
}

/**
 * @brief Right-aligns a string within a specified total length, padding with a given character.
 * If the string is longer than the desired length, it is not truncated.
 * Creates a new String object with the aligned content.
 * @param string The String object to align.
 * @param padding The character to use for padding.
 * @param length The total desired length of the aligned string.
 * @return A new String object that is right-aligned.
 */
String rightAlign(String string, char padding, int length)
{
    if (length < string.length)
        length = string.length;

    String result;
    result.length = length;
    result.word = (char *)malloc((length + 1) * sizeof(char));
    if (!result.word)
    {
        perror("Failed to allocate memory in rightAlign");
        exit(EXIT_FAILURE);
    }

    int filler = length - string.length;
    for (int i = 0; i < filler; i++)
        result.word[i] = padding;
    for (int i = 0; i < string.length; i++) // Corrected loop for copying original string
        result.word[filler + i] = string.word[i];
    result.word[length] = '\0';

    return result;
}

/**
 * @brief Trims leading and trailing occurrences of a specified character from a string.
 * Creates a new String object with the trimmed content.
 * @param string The String object to trim.
 * @param padding The character to trim from the ends of the string.
 * @return A new String object with the specified characters removed from the ends.
 */
String trim(String string, char padding)
{
    int start = 0;
    int end = string.length - 1;
    while (start <= end && string.word[start] == padding)
        start++;
    while (start <= end && string.word[end] == padding)
        end--;

    int len = end - start + 1;
    if (len < 0)
        len = 0; // Handle case where all characters are trimmed

    String trimmed;
    trimmed.length = len;
    trimmed.word = (char *)malloc((len + 1) * sizeof(char));
    if (!trimmed.word)
    {
        perror("Failed to allocate memory in trim");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < len; i++)
        trimmed.word[i] = string.word[start + i];
    trimmed.word[len] = '\0';

    return trimmed;
}

/**
 * @brief Splits a string into an array of String objects based on a delimiter.
 * Allocates memory for the array and each substring.
 * @param string The String object to split.
 * @param delimiter The character to use as a separator.
 * @param size A pointer to an integer that will store the number of substrings created.
 * @return A dynamically allocated array of String objects. The caller is responsible for freeing
 * each String.word and the array itself.
 */
String *split(String string, char delimiter, int *size)
{
    String *array = NULL;
    *size = 0;
    int start = 0;
    for (int i = 0; i <= string.length; i++)
    {
        if (string.word[i] == delimiter || string.word[i] == '\0')
        {
            if (start < i) // Only create a substring if there's content between delimiters
            {
                int len = i - start;
                String substring;
                substring.length = len;
                substring.word = (char *)malloc((len + 1) * sizeof(char));
                if (substring.word == NULL)
                {
                    perror("Failed to allocate memory in split");
                    exit(EXIT_FAILURE);
                }

                for (int j = 0; j < len; j++)
                    substring.word[j] = string.word[start + j];
                substring.word[len] = '\0';

                array = (String *)realloc(array, (*size + 1) * sizeof(String));
                if (array == NULL)
                {
                    perror("Failed to reallocate memory in split");
                    exit(EXIT_FAILURE);
                }
                array[*size] = substring;
                (*size)++;
            }
            start = i + 1;
        }
    }
    return array;
}

/**
 * @brief Joins an array of String objects into a single string using a delimiter.
 * Allocates memory for the new joined string.
 * @param array An array of String objects to join.
 * @param delimiter The character to insert between the joined strings.
 * @param size The number of String objects in the array.
 * @return A new String object containing the joined content.
 */
String join(String *array, char delimiter, int size)
{
    if (size == 0)
        return init(""); // Return an empty string if nothing to join

    int len = 0;
    for (int i = 0; i < size; i++)
        len += array[i].length;

    String joined;
    joined.length = len + (size > 0 ? size - 1 : 0); // Add space for delimiters: (size - 1) delimiters if size > 0, else 0
    joined.word = (char *)malloc((joined.length + 1) * sizeof(char));
    if (joined.word == NULL)
    {
        perror("Failed to allocate memory in join");
        exit(EXIT_FAILURE);
    }

    char *tempNew = joined.word;
    for (int i = 0; i < size; i++)
    {
        char *tempOld = array[i].word;
        while (*tempOld)
            *tempNew++ = *tempOld++;
        if (i < size - 1) // Add delimiter after each string except the last
            *tempNew++ = delimiter;
    }
    *tempNew = '\0';

    return joined;
}

/**
 * @brief Helper function: Converts a character to its numeric equivalent based on base.
 * @param c The character to convert.
 * @return The numeric value (0-35) or -1 if invalid.
 */
int __numericEquivalent__(const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 10;
    return -1; // Invalid character for number conversion
}

/**
 * @brief Parses a string representation of a number in a given base to an integer.
 * @param number The String object representing the number.
 * @param base The base of the number (2-36).
 * @return The integer equivalent, or -1 if the input is invalid or contains invalid digits for the base.
 */
int parse(const String number, const int base)
{
    if (base < 2 || base > 36)
        return -1;

    int num = 0;
    for (int i = 0; i < number.length; i++)
    {
        const int digit = __numericEquivalent__(number.word[i]);
        if (digit == -1)   // Character is not a valid digit for any base
            continue;      // Skip invalid characters, or could return -1 based on strictness
        if (digit >= base) // Digit value is too high for the given base
            return -1;
        num = num * base + digit;
    }
    return num;
}

/**
 * @brief Helper function: Converts an integer (0-35) to its character equivalent.
 * @param n The integer to convert.
 * @return The character equivalent ('0'-'9', 'A'-'Z') or '\0' if invalid.
 */
char __characterEquivalent__(const int n)
{
    if (n >= 0 && n <= 9)
        return n + '0';
    if (n >= 10 && n <= 35)
        return n - 10 + 'A';
    return '\0'; // Invalid number for character conversion
}

/**
 * @brief Converts an integer to its string representation in a given base.
 * Allocates memory for the new string.
 * @param num The integer to convert.
 * @param base The base for the conversion (2-36).
 * @return A new String object representing the number in the specified base.
 */
String convert(int num, int base)
{
    String number;
    if (num < 0 || base < 2 || base > 36)
    {
        perror("Invalid inputs in convert");
        exit(EXIT_FAILURE);
    }
    if (num == 0)
    {
        number.length = 1;
        number.word = (char *)malloc(2 * sizeof(char));
        if (number.word == NULL)
        {
            perror("Failed to allocate memory in convert");
            exit(EXIT_FAILURE);
        }
        number.word[0] = '0';
        number.word[1] = '\0';
        return number;
    }
    int len = 0;
    int temp = num;
    while (temp)
    {
        len++;
        temp /= base;
    }
    number.length = len;
    number.word = (char *)malloc((len + 1) * sizeof(char));
    if (number.word == NULL)
    {
        perror("Failed to allocate memory in convert");
        exit(EXIT_FAILURE);
    }
    number.word[len] = '\0';
    for (int i = len - 1; i >= 0; i--)
    {
        number.word[i] = __characterEquivalent__(num % base);
        num /= base;
    }
    return number;
}

#endif // STRING_LIB_H