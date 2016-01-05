#ifndef STICK_STICKERROR_HPP
#define STICK_STICKERROR_HPP

#include <Stick/StickString.hpp>
#include <Stick/StickUtility.hpp>

namespace stick
{
    class ErrorCategory;

    namespace detail
    {
        template<class T>
        struct isErrorEnum
        {
            static const bool value = false;
        };
    }

    /**
     * @brief An Error describes an error condition: http://en.wikipedia.org/wiki/Error_code
     *
     * This implementation holds an integer code that describes the error condition and a pointer to a
     * ErrorCategory which is the group of errors that the Error is part of.
     */
    class Error
    {
    public:

        /**
         * @brief Default Constructor.
         *
         * This will initialize the error category to NoErrorCategory and the code to 0,
         * which is the default no error indicator.
         */
        Error();

        /**
         * @brief Constructs an Error from a code and a category.
         *
         * You rarely use this constructor directly. Usually you would just provide the error code enum value,
         * and let the magic behind the scenes deduce the ErrorCategory automatically.
         *
         * @see detail::isErrorEnum
         */
        Error(Int32 _code, const ErrorCategory & _category);

        /**
         * @brief Constructs an Error from an ErrorEnum value.
         *
         * The error category is automatically deducted at compile time.
         */
        template<class ErrorEnum>
        Error(ErrorEnum _code, const String & _message = "", const char * _file = "", UInt32 _line = 0, typename EnableIf<detail::isErrorEnum<ErrorEnum>::value>::Type * = 0);

        /**
         * @brief Assigns an ErrorEnum value to this.
         *
         * The error category is automatically deducted at compile time.
         */
        template<class ErrorEnum>
        typename EnableIf<detail::isErrorEnum<ErrorEnum>::value, Error>::Type & operator = (ErrorEnum _code);

        /**
         * @brief Implicit conversion to bool.
         */
        explicit operator bool() const;

        /**
         * @brief Implicit conversion to bool.
         */
        bool operator!() const;

        /**
         * @brief Returns true if the codes and categories of both Errors are equal.
         */
        bool operator == (const Error & _other) const;

        /**
         * @brief Returns true if either the codes and/or categories of the Errors are not equal.
         */
        bool operator != (const Error & _other) const;

        /**
         * @brief Returns a String describing the error condition of this code.
         */
        String description() const;

        /**
         * @brief Returns a contextual message in which the error occured.
         */
        const String & message() const;

        /**
         * @brief Returns the file in which the error occured.
         */
        const String & file() const;

        /**
         * @brief Returns the line number that emitted the error.
         */
        UInt32 line() const;

        /**
         * @brief Returns the integer representation of the error condition.
         */
        Int32 code() const;

        /**
         * @brief Returns the resolved, platform independent error code.
         * @see ErrorCategory::resolveError
         */
        Error resolvedCode() const;

        /**
         * @brief Returns the ErrorCategory that this Error belongs to.
         */
        const ErrorCategory & category() const;


    protected:

        const ErrorCategory * m_category;
        Int32 m_code;
        String m_message;
        String m_file;
        UInt32 m_line;
    };

    template<class T>
    inline const ErrorCategory & errorCategory(T);
}

#include <Stick/StickErrorCategory.hpp>

namespace stick
{
    template<class ErrorEnum>
    Error::Error(ErrorEnum _code, const String & _message, const char * _file, UInt32 _line, typename EnableIf<detail::isErrorEnum<ErrorEnum>::value>::Type *) :
    m_category(&errorCategory(_code)),
    m_code(_code),
    m_message(_message),
    m_file(_file),
    m_line(_line)
    {

    }

    template<class ErrorEnum>
    typename EnableIf<detail::isErrorEnum<ErrorEnum>::value, Error>::Type & Error::operator = (ErrorEnum _code)
    {
        m_category = &errorCategory(_code);
        m_code = _code;
        return *this;
    }
}

#endif //STICK_STICKERROR_HPP