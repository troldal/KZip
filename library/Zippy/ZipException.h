//
// Created by Troldal on 2019-03-10.
//

#ifndef MINIZ_ZIPEXCEPTION_H
#define MINIZ_ZIPEXCEPTION_H

#include <stdexcept>

namespace Zippy {

    /**
     * @brief The ZipException class is a custom exception class derived from the std::runtime_error class.
     * @details In case of an error in the Zippy library, an ZipException object will be thrown, with a message
     * describing the details of the error.
     */
    class ZipException : public std::runtime_error {
    public:

        /**
         * @brief Constructor.
         * @param err A string with a description of the error.
         */
        inline explicit ZipException(const std::string& err)
                : runtime_error(err) {
        }

        /**
         * @brief Destructor.
         */
        inline ~ZipException() override = default;
    };
} // namespace Zippy


#endif //MINIZ_ZIPEXCEPTION_H
