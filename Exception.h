#ifndef EXCEPTION_H
#define EXCEPTION_H

namespace DTLib
{

#define THROW_EXCEPTION(Exception, message) (throw Exception(message, __FILE__, __LINE__))

class Exception
{
protected:
    char* m_message;
    char* m_location;

    void init(const char* message, const char* file, int line);
public:
    Exception(const char* message);
    Exception(const char* file, int line);
    Exception(const char* message, const char* file, int line);

    Exception(const Exception& e);
    Exception& operator=(const Exception& e);

    virtual const char* message() const;
    virtual const char* location() const;

    virtual ~Exception() = 0;
};

class ArithmeticException : public Exception
{
public:
    ArithmeticException() : Exception(nullptr) { }
    ArithmeticException(const char* message) : Exception(message) { }
    ArithmeticException(const char* file, int line) : Exception(file, line) { }
    ArithmeticException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    ArithmeticException(const ArithmeticException& e) : Exception(e) { }
    ArithmeticException& operator=(const ArithmeticException& e)
    {
        Exception::operator=(e);

        return *this;
    }

    ~ArithmeticException();
};

class NullPointerException : public Exception
{
public:
    NullPointerException() : Exception(nullptr) { }
    NullPointerException(const char* message) : Exception(message) { }
    NullPointerException(const char* file, int line) : Exception(file, line) { }
    NullPointerException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    NullPointerException(const NullPointerException& e) : Exception(e) { }
    NullPointerException& operator=(const NullPointerException& e)
    {
        Exception::operator=(e);

        return *this;
    }

    ~NullPointerException();
};

class IndexOutOfBoundsException : public Exception
{
public:
    IndexOutOfBoundsException() : Exception(nullptr) { }
    IndexOutOfBoundsException(const char* message) : Exception(message) { }
    IndexOutOfBoundsException(const char* file, int line) : Exception(file, line) { }
    IndexOutOfBoundsException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e) { }
    IndexOutOfBoundsException& operator=(const IndexOutOfBoundsException& e)
    {
        Exception::operator=(e);

        return *this;
    }

    ~IndexOutOfBoundsException();
};

class NoEnoughMemoryException : public Exception
{
public:
    NoEnoughMemoryException() : Exception(nullptr) { }
    NoEnoughMemoryException(const char* message) : Exception(message) { }
    NoEnoughMemoryException(const char* file, int line) : Exception(file, line) { }
    NoEnoughMemoryException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    NoEnoughMemoryException(const NoEnoughMemoryException& e) : Exception(e) { }
    NoEnoughMemoryException& operator=(const NoEnoughMemoryException& e)
    {
        Exception::operator=(e);

        return *this;
    }

    ~NoEnoughMemoryException();
};

class InvalidParameterException : public Exception
{
public:
    InvalidParameterException() : Exception(nullptr) { }
    InvalidParameterException(const char* message) : Exception(message) { }
    InvalidParameterException(const char* file, int line) : Exception(file, line) { }
    InvalidParameterException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    InvalidParameterException(const InvalidParameterException& e) : Exception(e) { }
    InvalidParameterException& operator=(const InvalidParameterException& e)
    {
        Exception::operator=(e);

        return *this;
    }

    ~InvalidParameterException();
};

class InvalidOperationException : public Exception
{
public:
    InvalidOperationException() : Exception(nullptr) { }
    InvalidOperationException(const char* message) : Exception(message) { }
    InvalidOperationException(const char* file, int line) : Exception(file, line) { }
    InvalidOperationException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    InvalidOperationException(const InvalidOperationException& e) : Exception(e) { }
    InvalidOperationException& operator=(const InvalidOperationException& e)
    {
        Exception::operator=(e);

        return *this;
    }

    ~InvalidOperationException();
};

}

#endif // EXCEPTION_H
