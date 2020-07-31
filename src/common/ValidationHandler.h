//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//


#ifndef COMMON_VALIDATIONHANDLER_H_
#define COMMON_VALIDATIONHANDLER_H_

#include <omnetpp.h>
using namespace omnetpp;

class ValidationHandler
{
public:

    static void ThrowError(const char* message)
    {
        EV << message << "\n";
        throw cRuntimeError(message);
    }

    /**
     * Validates the input is within the minimum and maximum bounds.
     * Throws an exception if the input is out of bounds.
     */
    template<typename T>
    static void validateInputBounds(T input, T min, T max, const char* message)
    {
        if (input < min || input > max)
        {
            ThrowError(message);
        }
    }

};

#endif /* COMMON_VALIDATIONHANDLER_H_ */
