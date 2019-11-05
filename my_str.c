#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//1.内存没有重叠的情况
char * my_strcpy(char *dest, const char *src)
{
    assert(dest && src);
    char *temp = dest;
    while((*temp++ = *src++) != '\0');
    return temp;
}

//2.内存有重叠的情况
char * my_strcpy1(char *dest, const char *src)
{
    assert(dest && src);
    char *temp = dest;
    int size = strlen(src)+1;
    if(dest >= src && dest <= src+size)
    {
        dest = dest + size - 1;
        src = src + size - 1;
        while(size--)
        {
            *dest-- = *src--;
        }
    }
    else
    {
        while(size--)
            *dest++ = *src++;
    }
    return temp;
}

//3.1 strlen的常规方法实现
int my_strlen(const char* str)
{
    assert(str);
    int count = 0;
    while(*str != '\0')
    {
        ++count;
        ++str;
    }
    return count;
}

//3.2 strlen的递归方法实现
int my_strlen_r(const char* str)
{
    assert(str);
    if(*str == '\0')
        return 0;
    else
        return 1+my_strlen_r(str+1);
}

//4. strcat的函数实现
char* my_strcat(char *dest, const char *src)
{
    assert(dest && src);
    char *temp = dest;
    while(*dest)
        ++dest;
    while((*dest++ = *src++) != '\0');
    return temp;
}

//5. strstr的函数实现(找子字符串在母字符串中第一次出现的位置)
char* my_strstr(const char *str, const char *substr)
{
    assert(str && substr);
    const char *s1 = str;
    const char *s2 = substr;
    const char *cur = str;
    while(*cur)
    {
        s1 = cur;
        s2 = substr;
        while(*s1 && *s2 && (*s1 == *s2))
        {
            ++s1;
            ++s2;
        }
        if(*s2 == '\0')
            return (char*)cur;
        if(*s1 == '\0')
            return NULL;
        ++cur;
    }
    return NULL;
}

// 6.strchr函数的实现
char* my_strchr(const char *str, char c)
{
    assert(str);
    while(*str)
    {
        if(*str == c)
            return (char*)str;
        ++str;  
    }
    return NULL;
}

// 7.strcmp函数的实现
int my_strcmp(const char *str1, const char *str2)
{
    assert(str1 && str2);
    while(*str1 == *str2)
    {
        if(*str1 == '\0')
            return 0;
        ++str1;
        ++str2;
    }
    return *str1-*str2;
}

// 8.memcpy函数的实现
void* my_memcpy(void *dest, const void *src, size_t size)
{
    assert(dest && src);
    char *pdest = (char*)dest;
    char *psrc = (char*)src;
    void *ret = dest;
    if(pdest > psrc && pdest < psrc + size)
    {
        pdest = pdest + size - 1;
        psrc = psrc + size - 1;
        while(size--)
        {
            *pdest-- = *psrc--;
        }
    }
    else
    {
        while(size--)
            *pdest++ = *psrc++;
    }
    return ret;
}
int main()
{
    char test[10] = {0};
    my_strcpy1(test, "abcde");
    printf("%s \n", test);
    printf("%d \n", my_strlen_r(test));
    char test1[20] = "ac";
    my_strcat(test1,"hahaha");
    printf("%s", test1);
    system("pause");
    return 0;
}