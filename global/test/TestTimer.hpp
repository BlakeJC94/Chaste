/*

Copyright (c) 2005-2014, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef _TESTTIMER_HPP_
#define _TESTTIMER_HPP_

#include <cxxtest/TestSuite.h>
#include "Timer.hpp"
#include "PetscSetupAndFinalize.hpp"

class TestTimer : public CxxTest::TestSuite
{
public:

    // Can't really test the timer, this is just for coverage and to illustrate usage
    void TestIt()
    {
    	Timer::Print("What time is it?");

        Timer::Reset();

        // Run simulation which shows **** cures cancer
        Timer::PrintAndReset("Cure cancer");

        // Run simulation which shows **** cures heart disease
        Timer::Print("Cure heart disease");

        double elapsed_time = Timer::GetElapsedTime();
        TS_ASSERT_LESS_THAN_EQUALS(0, elapsed_time);

        double current_time = Timer::GetWallTime();
        TS_ASSERT_LESS_THAN(1.4e9, current_time);
    }
};

#endif //_TESTTIMER_HPP_
