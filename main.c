#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//기본 초기화 목록입니다.
//기본 빈 값을 넣어주고,
//유사 멤버함수들을 전부 넣어줍니다.
#define NullString \
{ \
NULL, 0, \
_string_set, \
_string_setn, \
_string_gets, \
_string_get_length, \
_string_copy_from, \
_string_clone, \
_string_move, \
_string_append, \
_string_append_raw, \
_string_swap, \
_string_clear, \
_string_is_empty, \
_string_compare, \
_string_compare_raw, \
_string_is_same, \
_string_is_same_raw, \
_string_format, \
_string_append_format, \
_string_at, \
_string_ptr_at, \
_string_findc, \
_string_finds, \
_string_finds_raw, \
}


//C스타일 문자열 유사클래스입니다.
struct String
{
    char* data;
    size_t length;

    //문자열을 할당합니다.
    void (*set)(struct String*, const char*);
    void (*setn)(struct String*, const char*, size_t);

    //문자열을 가져옵니다.
    const char* (*gets)(const struct String*);

    //문자열의 길이를 가져옵니다.
    size_t (*get_length)(const struct String*);

    //깊은복사를 행합니다.
    void (*copy_from)(struct String*, const struct String*);

    //깊게 복제된 객체를 반환합니다.
    struct String (*clone)(const struct String*);

    //이동합니다. 자신은 clear가 됩니다.
    struct String (*move)(struct String*);

    //이어붙입니다.
    void (*append)(struct String*, const struct String*); //String 버전입니다.
    void (*append_raw)(struct String*, const char*); //생 문자열 버전입니다.

    //교환합니다.
    void (*swap)(struct String*, struct String*);

    //지워버립니다.
    void (*clear)(struct String*);

    //비어있는지 여부를 확인합니다.
    int (*is_empty)(const struct String*);

    //비교해서 같으면 0... 뭐 그런거요
    int (*compare)(const struct String*, const struct String*); //String 버전입니다.
    int (*compare_raw)(const struct String*, const char*); //생 문자열 버전입니다.

    //같으면 1. 다르면 0.
    int (*is_same)(const struct String*, const struct String*); //String 버전입니다.
    int (*is_same_raw)(const struct String*, const char*); //생 문자열 버전입니다.

    //포맷팅합니다.
    void (*format)(struct String*, const char*, ...); //포맷팅해서 그대로 대입합니다.
    void (*append_format)(struct String*, const char*, ...); //포맷팅한 문자열을 이어붙입니다.

    //인덱싱합니다. 범위를 벗어나면 EOF를 줘요.
    int (*at)(const struct String*, size_t); //그냥 그 문자를 반환합니다.
    int* (*ptr_at)(struct String*, size_t); //그 문자의 주소를 반환합니다.

    //문자나 문자열을 탐색해서 인덱스를 줍니다.
    //못찾으면 EOF를 반환합니다.
    int (*findc)(const struct String*, char c); //문자를 찾습니다.
    int (*finds)(const struct String*, const struct String*); //String 문자열을 찾습니다.
    int (*finds_raw)(const struct String*, const char*); //생 문자열을 받아서 찾습니다.
};
typedef struct String String;
typedef String string;


/*빌더 함수 선언*/
String make_string(const char*);
String make_nullstring();
/*String 유사메서드*/
void _string_set(String*, const char*);
void _string_setn(String*, const char*, size_t);
const char* _string_gets(const String*);
size_t _string_get_length(const String*);
void _string_copy_from(String* self, const String* other);
String _string_clone(const String* self);
String _string_move(String* self);
void _string_append(String* self, const String* other);
void _string_append_raw(String* self, const char* other);
void _string_swap(String* self, String* other);
void _string_clear(String* self);
int _string_is_empty(const String* self);
int _string_compare(const String* self, const String* other);
int _string_compare_raw(const String* self, const char* other);
int _string_is_same(const String* self, const String* other);
int _string_is_same_raw(const String* self, const char* other);
void _string_format(String* self, const char* format,...);
void _string_append_format(String* self, const char* format, ...);
int _string_at(const String* self, size_t index);
int _string_ptr_at(String* self, size_t index);
int _string_findc(const String* self, char c);
int _string_finds(const String* self, const String* key);
int _string_finds_raw(const String* self, const char* key);



/*유사메서드 정의*/
void _string_set(String* self, const char* s)
{
    self->length = strlen(s);

    free(self->data);
    self->data = malloc(sizeof(char)*self->length +1);

    strncpy(self->data, s, self->length+1);
}

void _string_setn(String* self, const char* s, size_t len)
{
    self->length = len;

    free(self->data);
    self->data = malloc(sizeof(char)*self->length +1);

    strncpy(self->data, s, self->length +1);
}

const char* _string_gets(const String* self)
{
    return self->data;
}

size_t _string_get_length(const String* self)
{
    return self->length;
}

void _string_copy_from(String* self, const String* other)
{
    self->length = other->length;

    free(self->data);
    self->data = malloc(sizeof(char)*self->length +1);

    strncpy(self->data, other->data, self->length +1);
}

String _string_clone(const String* self)
{
    String temp = make_nullstring();
    temp.setn(&temp, self->data, self->length);
    return temp;
}

String _string_move(String* self)
{
    String temp = make_nullstring();
    temp.data = self->data;
    temp.length = self->length;

    self->data=NULL;
    self->length=0;

    return temp;
}

void _string_append(String* self, const String* other)
{
    self->length+=other->length;
    realloc(self->data, self->length+1);
    strncat(self->data, other->data, self->length+1);
}

void _string_append_raw(String* self, const char* other)
{
    self->length+=strlen(other);
    realloc(self->data, self->length);
    strncat(self->data, other, self->length+1);
}

void _string_swap(String* self, String* other)
{
    char* t = self->data;
    size_t t2 = self->length;

    self->data = other->data;
    self->length = other->length;

    other->data = t;
    other->length = t2;
}

void _string_clear(String* self)
{
    self->length=0;
    free(self->data);
    self->data=NULL;
}

int _string_is_empty(const String* self)
{
    return self->data==NULL;
}

int _string_compare(const String* self, const String* other)
{
    return strcmp(self->data, other->data);
}
int _string_compare_raw(const String* self, const char* other)
{
    return strcmp(self->data, other);
}

int _string_is_same(const String* self, const String* other)
{
    return strcmp(self->data, other->data)==0;
}
int _string_is_same_raw(const String* self, const char* other)
{
    return strcmp(self->data, other)==0;
}


enum
{
    STRING_BUFFER_SIZE = 200 //포맷팅 함수에서 사용할 버퍼 크기. 늘려도 됨
};

void _string_format(String* self, const char* format,...)
{
    char buffer[STRING_BUFFER_SIZE];

    va_list args;
    va_start(args, format);

    vsprintf(buffer, format, args);

    self->set(&self, buffer);

    va_end(args);
}

void _string_append_format(String* self, const char* format, ...)
{
    char buffer[STRING_BUFFER_SIZE];

    va_list args;
    va_start(args, format);

    vsprintf(buffer, format, args);

    self->append_raw(&self, buffer);

    va_end(args);
}

int _string_at(const String* self, size_t index)
{
    if(index<0 || index>=self->length)
        return EOF;
    else
        return self->data[index];
}

int _string_ptr_at(String* self, size_t index)
{
    if(index<0 || index>=self->length)
        return EOF;
    else
        return &(self->data[index]);
}

int _string_findc(const String* self, char c)
{
    for(int i=0; i<self->length; ++i)
    {
        if(self->data[i]==c)
            return i;
    }

    return EOF;
}

int _string_finds(const String* self, const String* key)
{
    const char* finded = strstr(self->data, key->data);
    if(finded==NULL)
        return EOF;
    else
        return (int)finded - (int)(self->data);
}

int _string_finds_raw(const String* self, const char* key)
{
    const char* finded = strstr(self->data, key);
    if(finded==NULL)
        return EOF;
    else
        return (int)finded - (int)(self->data);
}



//String 유사클래스 유사객체를 생성해서 반환합니다.
String make_string(const char* s)
{
    String temp = NullString;
    temp.set(&temp, s);
    return temp;
}

//빈 유사객체를 생성해서 반환합니다.
String make_nullstring()
{
    String temp = NullString;
    return temp;
}


const int main(void)
{
    string s = make_string("hello");

    printf(s.data);

    return 0;
}