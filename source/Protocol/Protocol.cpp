#include "Protocol.hpp"

Mark1::Mark1() : type(DataType::NONE), length(0), data(nullptr), own(false)
{
}

Mark1::Mark1(const Mark1 &argv)
{
    type = argv.type;
    length = argv.length;
    own = true;
    data = new char[length];
    memcpy(data, argv.data, length);
}

Mark1 &Mark1::operator=(const Mark1 &argv)
{
    if (this != &argv)
    {
        type = argv.type;
        length = argv.length;
        own = true;
        data = new char[length];
        memcpy(data, argv.data, length);
    }
    return *this;
}

Mark1::Mark1(Mark1 &&argv)
{
    type = argv.type;
    length = argv.length;
    data = argv.data;
    own = argv.own;
    argv.length = 0;
    argv.data = nullptr;
    argv.own = false;
}

Mark1 &Mark1::operator=(Mark1 &&argv)
{
    if (this != &argv)
    {
        type = argv.type;
        length = argv.length;
        data = argv.data;
        own = argv.own;
        argv.length = 0;
        argv.data = nullptr;
        argv.own = false;
    }
    return *this;
}

Mark1::~Mark1()
{
    if (own)
    {
        delete[] data;
    }
}

uint32_t Mark1::capacity() const { return length + 5; } // data.length + data.type + data.data
uint32_t Mark1::fullsize() const { return length + 9; } // capacity + data.length + data.type + data.data

// Структура пакета:
// 0 - 3: capacity
// 4: type
// 5-9: length
// 10-10+length: data
char *Mark1::serialize() const
{
    char *result = new char[fullsize()]; // теперь мы учитываем полную длину данных

    uint32_t net_capacity = htonl(capacity());
    uint32_t net_length = htonl(length);

    memcpy(result, &net_capacity, 4);
    memcpy(result + 4, &type, 1);       // запоминаем тип данных
    memcpy(result + 5, &net_length, 4); // запоминаем длину данных
    memcpy(result + 9, data, length);   // запонимаем сами данные, которые в последстивии буду передаваться в методы self_deserialize
    return result;
}

Mark1 Mark1::deserialize(char *buffer)
{
    Mark1 result;

    memcpy(&result.type, buffer, 1);

    uint32_t net_length;
    memcpy(&net_length, buffer + 1, 4);
    result.length = ntohl(net_length);

    result.data = new char[result.length];
    result.own = true;
    memcpy(result.data, buffer + 5, result.length);
    return result;
}