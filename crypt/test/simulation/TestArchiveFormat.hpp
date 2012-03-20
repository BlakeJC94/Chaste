/*

Copyright (c) 2005-2012, University of Oxford.
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

#ifndef TESTARCHIVEFORMAT_HPP_
#define TESTARCHIVEFORMAT_HPP_

#include <cxxtest/TestSuite.h>

// Must be included before any other cell_based or crypt headers
#include "CellBasedSimulationArchiver.hpp"

#include <iomanip>
#include "CryptSimulation2d.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "CylindricalHoneycombMeshGenerator.hpp"
#include "SloughingCellKiller.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "StochasticWntCellCycleModel.hpp"

/**
 * This class consists of a single crypt simulation archiving test.
 */
class TestArchiveFormat : public CxxTest::TestSuite
{
public:

    /**
     * This test is required because Test2DCryptRepresentativeSimulation loads
     * an archive stored in crypt/test/data. When the archiving of
     * OffLatticeSimulation and associate classes is updated, the stored archive
     * needs to be updated. This test checks that the archive can be loaded,
     * and will seg fault if not. It does nothing more, so it runs quickly
     * and can be in the continuous test pack.
     *
     * IF THIS TEST FAILS:
     * - You have probably changed an archiving method somewhere
     * - You need to remake crypt/test/data/<test below>/archive/
     * - To do this re-run TestGenerateSteadyStateCrypt.hpp
     * - Archives produced can then be copied to crypt/test/data/<test below>/archive/
     *
     * Note that when updating the archive, you can run TestGenerateSteadyStateCrypt.hpp
     * with build=GccOpt to speed up the test.
     *
     * Note: from Chaste release 3.1 onward we no longer support Boost 1.33.
     * The earliest version of Boost supported in 1.34
     *
     * NB: Produce archives with
       scons build=GccOpt_hostconfig,boost=1-34,use-cvode=0 test_suite=crypt/test/simulation/TestGenerateSteadyStateCrypt.hpp
       cp /tmp/$USER/testoutput/SteadyStateCrypt/archive/?*_150.* crypt/test/data/SteadyStateCrypt/archive/
     *
     */
    void TestLoadArchive() throw (Exception)
    {
        // Set start time
        SimulationTime::Instance()->SetStartTime(0.0);

        // Directory in which the stored results were archived
        std::string test_to_profile = "SteadyStateCrypt";

        // Simulation time at which the stored results were archived
        double t = 150;

        // Open a new directory
        OutputFileHandler file_handler(test_to_profile, true);

        // The archive must be copied from crypt/test/data/<test_to_profile>
        // to the testoutput directory to continue running the simulation
        std::string test_output_directory = OutputFileHandler::GetChasteTestOutputDirectory();
        std::string test_data_directory = "crypt/test/data/" + test_to_profile +"/";
        std::string command = "cp -Rf --remove-destination " + test_data_directory +" "+ test_output_directory +"/";

        // Test that the above command was implemented successfully
        int return_value = system(command.c_str());
        TS_ASSERT_EQUALS(return_value, 0);

        // Load and run crypt simulation
        CryptSimulation2d* p_simulator = CellBasedSimulationArchiver<2, CryptSimulation2d>::Load(test_to_profile,t);
        p_simulator->SetEndTime(t + 1);

        ///\todo we should also test the rest of the simulation setup

        // Tidy up
        delete p_simulator;
        SimulationTime::Destroy();
        RandomNumberGenerator::Destroy();
    }
};

#endif /*TESTARCHIVEFORMAT_HPP_*/
