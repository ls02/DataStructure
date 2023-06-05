#ifndef __MY_STRING_HPP__
#define __MY_STRING_HPP__
#include <iostream>
#include <cstring>
#include <assert.h>
#include <utility>

namespace ls {
    const static size_t npos = -1;
    class string {
        public:
            typedef char* iterator;
            typedef const char* const_iterator;

            iterator begin()
            {
                return _buff;
            }

            iterator end()
            {
                return _buff + _size;
            }

            const_iterator begin() const
            {
                return _buff;
            }

            const_iterator end() const
            {
                return _buff + _size;
            }

        public:
            // 默认构造
            string()
                :_buff(new char[1])
                ,_size(0)
                ,_capacity(0)
            {}

            // 析构函数
            ~string()
            {
                delete[] _buff;
                _size = _capacity = 0;
            }

            // 默认构造
            string(const char* buff)
            {
                _capacity = 4;
                int len = strlen(buff);
                if (len > 4)
                {
                    _capacity *= 2;
                }

                _size = len;
                _buff = new char[len + 1];
                _buff[len] = 0;
                
                strcpy(_buff, buff);
            }

            // 拷贝构造，利用默认构造完成深拷贝
            string(const string &str)
                :_buff(nullptr)
                ,_size(0)
                ,_capacity(0)
            {
                string tmp(str.c_str());
                swap(tmp);
            }

            string& operator=(string str)
            {
                swap(str);

                return *this;
            }

            char& operator[](size_t pos) 
            {
                assert(pos < _size);

                return _buff[pos];
            }

            const char& operator[](size_t pos) const 
            {
                assert(pos < _size);

                return _buff[pos];
            }

            char& at(size_t pos)
            {
                assert(pos < _size);

                return _buff[pos];
            }

            string&append(string &str)
            {
                insert(_size, str);

                return *this;
            }

            string& append(const char *buff)
            {
                insert(_size, buff);

                return *this;
            }

            bool empty()
            {
                return _size == 0;
            }

            void clear()
            {
                _size = 0;
                _buff[_size] = 0;
            }

            void pop_back()
            {
                erase(_size);
            }
            
            string substr(size_t pos = 0, size_t len = npos) const 
            {
                assert(pos < _size);
                string str;

                if (len > _size)
                {
                    str = _buff + pos;
                }
                else 
                {
                    while (len)
                    {
                        str += _buff[pos + len];
                        len--;
                    }
                }

                return str;
            }

            size_t find(const string &str, size_t pos = 0) const
            {
                assert(pos < _size);
                char *tmp = strstr(_buff + pos, str.c_str());
                if (tmp == nullptr)
                {
                    return npos;
                }

                return tmp - _buff;
            }

            size_t find(const char *str, size_t pos = 0) const
            {
                assert(pos < _size);
                char *tmp = strstr(_buff + pos, str);
                if (tmp == nullptr)
                {
                    return npos;
                }

                return tmp - _buff;
            }

            const char& at(size_t pos) const 
            {
                assert(pos < _size);

                return _buff[pos];
            }

            void swap(string &tmp)
            {
                std::swap(_buff, tmp._buff);
                std::swap(_size, tmp._size);
                std::swap(_capacity, tmp._capacity);
            }

            string& insert(int pos, const char *buff)
            {
                assert(pos <= _size);

                int len = strlen(buff);

                if (len + _size >= _capacity)
                {
                    reserver(len + _size);
                }

                int end = _size;

                while (pos <= end)
                {
                    _buff[end + len] = _buff[end];
                    end--;
                }

                strncpy(_buff + pos, buff, len);
                _size += len;

                return *this;
            }

            string& insert(int pos, const char ch)
            {
                assert(pos <= _size);

                if (_size == _capacity)
                {
                    reserver(_capacity == 0 ? 4 :_capacity * 2);
                }

                int end = _size;
                while (pos <= end)
                {
                    _buff[end + 1] = _buff[end];
                    end--;
                }

                _buff[pos] = ch;
                _size++;

                return *this;
            }

            string& insert(int pos, string &str)
            {
                assert(pos <= _size);

                int len = str.size();
                if (_size + len >= _capacity)
                {
                    reserver(len + _size);
                }

                int end = _size;
                while (pos <= end)
                {
                    _buff[end + len] = _buff[end];
                    end--;
                }

                strncpy(_buff + pos, str.c_str(), len);
                _size += len;

                return *this;
            }

            string& erase(size_t pos = 0, size_t len = npos)
            {
                assert(pos < _size);

                if (len > _size)
                {
                    _size = pos;
                }
                else 
                {
                   size_t end = pos + len;
                   while (end < _size)
                   {
                       _buff[end - len] = _buff[end];
                       end++;
                   }
                   _size -= len;
                }

                _buff[_size] = 0;

                return *this;

            }

            void PushBack(const char *buff) {
                insert(_size, buff);
            }

            void PushBack(const char ch)
            {
                insert(_size, ch);
            }

            string& operator+=(const char ch)
            {
                PushBack(ch);

                return *this;
            }

            string& operator+=(const char *str)
            {
                PushBack(str);

                return *this;
            }

            string& operator+=(string &str)
            {
                insert(_size, str);

                return *this;
            }

            void reserver(size_t n)
            {
                if (n > _capacity)
                {
                    char *tmp = new char[n + 1];
                    strcpy(tmp, _buff);
                    delete[] _buff;
                    _buff = tmp;

                    _capacity = n;
                }
            }

            const char* c_str() const
            {
                return _buff;
            }

            size_t size() const 
            {
                return _size;
            }

            size_t capacity() const 
            {
                return _capacity;
            }

            friend std::ostream& operator<<(std::ostream &out, const string &s);
            friend std::istream& operator>>(std::istream &in, string &str);

        private:
            char *_buff;
            size_t _size;
            size_t _capacity;
        public:
            static size_t npos;
    };
            std::ostream& operator<<(std::ostream &out, const string &str)
            {
                for (size_t i = 0; i < str.size(); i++)
                {
                    out << str[i];
                }

                return out;
            }

            std::istream& operator>>(std::istream &in, string &str)
            {
                str.clear();
                char ch = 0;
                ch = in.get();
                
                const size_t N = 32;
                char buff[N] = { 0 };
                size_t i = 0;

                while (ch != ' ' && ch != '\n')
                {
                    buff[i++] = ch;
                    if (i == N - 1)
                    {
                        buff[i] = 0;
                        str += buff;
                        i = 0;
                    }

                    ch = in.get();
                }

                buff[i] = 0;
                str += buff;

                return in;
            }

    size_t string::npos = -1;
}

#endif
