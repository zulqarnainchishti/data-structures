#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "adt_String.h" // Assuming adt_String.h contains the String struct and functions

int tests_run = 0;
int tests_passed = 0;

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

#define ASSERT_VERBOSE(expr, msg, expected, actual)     \
    do                                                  \
    {                                                   \
        tests_run++;                                    \
        printf(CYAN "TEST: %s\n" RESET, msg);           \
        printf("  Expected: \"%s\"\n", expected);       \
        printf("  Actual  : \"%s\"\n", actual);         \
        if (expr)                                       \
        {                                               \
            printf(GREEN "  Result  : PASS\n\n" RESET); \
            tests_passed++;                             \
        }                                               \
        else                                            \
        {                                               \
            printf(RED "  Result  : FAIL\n\n" RESET);   \
        }                                               \
    } while (0)

#define ASSERT_BOOL(expr, msg, expected_bool)                     \
    do                                                            \
    {                                                             \
        tests_run++;                                              \
        printf(CYAN "TEST: %s\n" RESET, msg);                     \
        char actual_str[6], expected_str[6];                      \
        strcpy(expected_str, (expected_bool) ? "true" : "false"); \
        strcpy(actual_str, (expr) ? "true" : "false");            \
        printf("  Expected: \"%s\"\n", expected_str);             \
        printf("  Actual  : \"%s\"\n", actual_str);               \
        if (expr == expected_bool)                                \
        {                                                         \
            printf(GREEN "  Result  : PASS\n\n" RESET);           \
            tests_passed++;                                       \
        }                                                         \
        else                                                      \
        {                                                         \
            printf(RED "  Result  : FAIL\n\n" RESET);             \
        }                                                         \
    } while (0)

void test_init_and_copy()
{
    String s1 = init("Hello, World!");
    String s2 = init("Hello, World!");
    String s3 = copy(s1);

    char s1_len_str[10];
    sprintf(s1_len_str, "%d", s1.length);
    ASSERT_VERBOSE(s1.length == 13, "init() sets correct length", "13", s1_len_str);
    ASSERT_VERBOSE(isEqual(s1, s2), "init() produces correct string", s2.word, s1.word);
    ASSERT_VERBOSE(isEqual(s1, s3), "copy() produces identical string", s1.word, s3.word);

    destroy(&s1);
    destroy(&s2);
    destroy(&s3);
}

void test_get_set()
{
    String s = init("abcdef");
    char get_char = get(s, 2);
    char str_get[2] = {get_char, '\0'};
    ASSERT_VERBOSE(get_char == 'c', "get(s,2) == 'c'", "c", str_get);

    String replacement = init("XYZ");
    String expected_set = init("abXYZf");
    String modified = set(s, replacement, 2, 5);
    ASSERT_VERBOSE(isEqual(modified, expected_set), "set() replaces substring", expected_set.word, modified.word);

    destroy(&s);
    destroy(&replacement);
    destroy(&expected_set);
    destroy(&modified);
}

void test_insert_and_concat()
{
    String s1 = init("Hello");
    String s2 = init("World");
    String expected_inserted = init("HelWorldlo");
    String inserted = insert(s1, s2, 3);
    ASSERT_VERBOSE(isEqual(inserted, expected_inserted), "insert('Hello','World',3)", expected_inserted.word, inserted.word);

    String expected_combined = init("HelloWorld");
    String combined = concat(s1, s2);
    ASSERT_VERBOSE(isEqual(combined, expected_combined), "concat('Hello','World')", expected_combined.word, combined.word);

    destroy(&s1);
    destroy(&s2);
    destroy(&expected_inserted);
    destroy(&inserted);
    destroy(&expected_combined);
    destroy(&combined);
}

void test_discard_and_slice()
{
    String s = init("abcdefg");
    String expected_cut = init("abfg");
    String cut = discard(s, 2, 3);
    ASSERT_VERBOSE(isEqual(cut, expected_cut), "discard('abcdefg',2,3)", expected_cut.word, cut.word);

    String expected_sliced = init("cde");
    String sliced = slice(s, 2, 5);
    ASSERT_VERBOSE(isEqual(sliced, expected_sliced), "slice('abcdefg',2,5)", expected_sliced.word, sliced.word);

    destroy(&s);
    destroy(&expected_cut);
    destroy(&cut);
    destroy(&expected_sliced);
    destroy(&sliced);
}

void test_replace()
{
    String s = init("the cat sat");
    String old_s = init("cat");
    String new_s = init("dog");
    String expected_result = init("the dog sat");
    String result = replace(s, old_s, new_s, 0);
    ASSERT_VERBOSE(isEqual(result, expected_result), "replace('the cat sat','cat','dog')", expected_result.word, result.word);

    destroy(&s);
    destroy(&old_s);
    destroy(&new_s);
    destroy(&expected_result);
    destroy(&result);
}

void test_comparison()
{
    String s1 = init("ABC");
    String s2 = init("abc");

    ASSERT_BOOL(isEqual(s1, s2), "isEqual() is case-sensitive", false); // s1 != s2
    ASSERT_BOOL(isSimilar(s1, s2), "isSimilar() ignores case", true);   // s1 == s2 (case-insensitive)

    int cmp = compare(s1, s2);
    char cmp_str[10];
    sprintf(cmp_str, "%d", cmp);
    ASSERT_VERBOSE(cmp < 0, "compare('ABC','abc') < 0", "negative", cmp_str); // 'A' comes before 'a' in ASCII

    destroy(&s1);
    destroy(&s2);
}

void test_properties()
{
    String s_upper = init("HELLO");
    String s_mixed_case = init("Hello"); // Used for tests expecting 'false'
    String s_lower = init("hello");
    String s_alphabet = init("AbCdEf");
    String s_alphabet_num = init("AbC123");
    String s_numeric = init("123456");
    String s_numeric_alpha = init("123a");
    String s_alphanum = init("abc123");
    String s_alphanum_special = init("abc-123");
    String s_whitespace = init(" \t\n");
    String s_non_whitespace = init("abc");

    ASSERT_BOOL(isUpper(s_upper), "isUpper('HELLO')", true);
    ASSERT_BOOL(isUpper(s_mixed_case), "isUpper('Hello')", false); // Corrected: No '!'

    ASSERT_BOOL(isLower(s_lower), "isLower('hello')", true);
    ASSERT_BOOL(isLower(s_mixed_case), "isLower('Hello')", false); // Corrected: No '!'

    ASSERT_BOOL(isAlphabet(s_alphabet), "isAlphabet('AbCdEf')", true);
    ASSERT_BOOL(isAlphabet(s_alphabet_num), "isAlphabet('AbC123')", false); // Corrected: No '!'

    ASSERT_BOOL(isNumeric(s_numeric), "isNumeric('123456')", true);
    ASSERT_BOOL(isNumeric(s_numeric_alpha), "isNumeric('123a')", false); // Corrected: No '!'

    ASSERT_BOOL(isAlphanum(s_alphanum), "isAlphanum('abc123')", true);
    ASSERT_BOOL(isAlphanum(s_alphanum_special), "isAlphanum('abc-123')", false); // Corrected: No '!'

    ASSERT_BOOL(isWhitespace(s_whitespace), "isWhitespace(' \\t\\n')", true);
    ASSERT_BOOL(isWhitespace(s_non_whitespace), "isWhitespace('abc')", false); // This one was already correct

    destroy(&s_upper);
    destroy(&s_mixed_case);
    destroy(&s_lower);
    destroy(&s_alphabet);
    destroy(&s_alphabet_num);
    destroy(&s_numeric);
    destroy(&s_numeric_alpha);
    destroy(&s_alphanum);
    destroy(&s_alphanum_special);
    destroy(&s_whitespace);
    destroy(&s_non_whitespace);
}

void test_searching()
{
    String s = init("the quick brown fox jumps");
    String target_the = init("the");
    String target_jumps = init("jumps");
    String target_quick = init("quick");
    String target_o = init("o");

    ASSERT_BOOL(startsWith(s, target_the), "startsWith('the quick...', 'the')", true);
    ASSERT_BOOL(endsWith(s, target_jumps), "endsWith('the quick...', 'jumps')", true);
    ASSERT_BOOL(contains(s, target_quick), "contains('the quick...', 'quick')", true);

    int idx = find(s, target_quick, 0);
    char idx_str[10];
    sprintf(idx_str, "%d", idx);
    ASSERT_VERBOSE(idx == 4, "find('the quick...', 'quick')", "4", idx_str);

    int c = count(s, target_o);
    char count_str[10];
    sprintf(count_str, "%d", c);
    ASSERT_VERBOSE(c == 2, "count('the quick...', 'o')", "2", count_str);

    destroy(&s);
    destroy(&target_the);
    destroy(&target_jumps);
    destroy(&target_quick);
    destroy(&target_o);
}

void test_case_conversion_and_repeat()
{
    String s = init("abc");
    String expected_upper = init("ABC");
    String upper = toUpper(s);
    ASSERT_VERBOSE(isEqual(upper, expected_upper), "toUpper('abc')", expected_upper.word, upper.word);

    String expected_lower = init("abc");
    String lower = toLower(upper);
    ASSERT_VERBOSE(isEqual(lower, expected_lower), "toLower('ABC')", expected_lower.word, lower.word);

    String expected_repeated = init("abcabcabc");
    String repeated = repeat(s, 3);
    ASSERT_VERBOSE(isEqual(repeated, expected_repeated), "repeat('abc', 3)", expected_repeated.word, repeated.word);

    destroy(&s);
    destroy(&expected_upper);
    destroy(&upper);
    destroy(&expected_lower);
    destroy(&lower);
    destroy(&expected_repeated);
    destroy(&repeated);
}

void test_alignment_and_trim()
{
    String base = init("hello");
    String expected_left = init("hello_____");
    String left = leftAlign(base, '_', 10);
    ASSERT_VERBOSE(isEqual(left, expected_left), "leftAlign('hello','_',10)", expected_left.word, left.word);

    String expected_center = init("...hello...");
    String center = centerAlign(base, '.', 11);
    ASSERT_VERBOSE(isEqual(center, expected_center), "centerAlign('hello','.',11)", expected_center.word, center.word);

    String expected_right = init("****hello");
    String right = rightAlign(base, '*', 9);
    ASSERT_VERBOSE(isEqual(right, expected_right), "rightAlign('hello','*',9)", expected_right.word, right.word);

    String padded = init("$$$hello$$$");
    String expected_trimmed = init("hello");
    String trimmed = trim(padded, '$');
    ASSERT_VERBOSE(isEqual(trimmed, expected_trimmed), "trim('$$$hello$$$','$')", expected_trimmed.word, trimmed.word);

    destroy(&base);
    destroy(&expected_left);
    destroy(&padded);
    destroy(&left);
    destroy(&expected_center);
    destroy(&center);
    destroy(&expected_right);
    destroy(&right);
    destroy(&expected_trimmed);
    destroy(&trimmed);
}

void test_split_and_join()
{
    String s = init("apple,banana,cherry");
    int size;
    String *parts = split(s, ',', &size);

    char size_str[10];
    sprintf(size_str, "%d", size);
    ASSERT_VERBOSE(size == 3, "split() returns correct size", "3", size_str);

    String expected_part0 = init("apple");
    ASSERT_VERBOSE(isEqual(parts[0], expected_part0), "split()[0] == 'apple'", expected_part0.word, parts[0].word);
    String expected_part1 = init("banana");
    ASSERT_VERBOSE(isEqual(parts[1], expected_part1), "split()[1] == 'banana'", expected_part1.word, parts[1].word);
    String expected_part2 = init("cherry");
    ASSERT_VERBOSE(isEqual(parts[2], expected_part2), "split()[2] == 'cherry'", expected_part2.word, parts[2].word);

    String expected_joined = init("apple-banana-cherry");
    String joined = join(parts, '-', size);
    ASSERT_VERBOSE(isEqual(joined, expected_joined), "join() works", expected_joined.word, joined.word);

    for (int i = 0; i < size; i++)
    {
        destroy(&parts[i]);
    }
    free(parts); // Free the array of String objects itself

    destroy(&s);
    destroy(&expected_part0);
    destroy(&expected_part1);
    destroy(&expected_part2);
    destroy(&expected_joined);
    destroy(&joined);
}

void test_base_conversion()
{
    String s = init("255");
    int val = parse(s, 10);
    char val_str[10];
    sprintf(val_str, "%d", val);
    ASSERT_VERBOSE(val == 255, "parse('255') in base 10", "255", val_str);

    String expected_b2 = init("11111111");
    String b2 = convert(255, 2);
    ASSERT_VERBOSE(isEqual(b2, expected_b2), "convert(255, 2)", expected_b2.word, b2.word);

    String expected_b16 = init("FF");
    String b16 = convert(255, 16);
    ASSERT_VERBOSE(isEqual(b16, expected_b16), "convert(255, 16)", expected_b16.word, b16.word);

    destroy(&s);
    destroy(&expected_b2);
    destroy(&b2);
    destroy(&expected_b16);
    destroy(&b16);
}

int main()
{
    printf("Running custom string library tests...\n\n");
    test_init_and_copy();
    test_get_set();
    test_insert_and_concat();
    test_discard_and_slice();
    test_replace();
    test_comparison();
    test_properties();
    test_searching();
    test_case_conversion_and_repeat();
    test_alignment_and_trim();
    test_split_and_join();
    test_base_conversion();
    printf("\nSummary: %d/%d tests passed.\n", tests_passed, tests_run);
    return (tests_passed == tests_run) ? EXIT_SUCCESS : EXIT_FAILURE;
}