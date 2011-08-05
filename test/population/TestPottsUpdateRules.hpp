/*

Copyright (C) University of Oxford, 2005-2011

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Chaste is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

Chaste is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details. The offer of Chaste under the terms of the
License is subject to the License being interpreted in accordance with
English Law and subject to any action against the University of Oxford
being under the jurisdiction of the English Courts.

You should have received a copy of the GNU Lesser General Public License
along with Chaste. If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef TESTPOTTSUPDATERULES_HPP_
#define TESTPOTTSUPDATERULES_HPP_

#include <cxxtest/TestSuite.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "AbstractPottsUpdateRule.hpp"
#include "VolumeConstraintUpdateRule.hpp"
#include "AdhesionUpdateRule.hpp"
#include "DifferentialAdhesionUpdateRule.hpp"
#include "CellsGenerator.hpp"
#include "FixedDurationGenerationBasedCellCycleModel.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "CellwiseData.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "WildTypeCellMutationState.hpp"
#include "CellLabel.hpp"
#include "PottsMeshGenerator.hpp"
#include "NodesOnlyMesh.hpp"
#include "NodeBasedCellPopulation.hpp"

class TestPottsUpdateRules : public AbstractCellBasedTestSuite
{
public:

    void TestVolumeConstraintUpdateRuleMethods() throw (Exception)
    {
		// Create a simple 2D PottsMesh with 2 elements
		PottsMeshGenerator<2> generator(4, 1, 2, 4, 2, 2);
		PottsMesh<2>* p_mesh = generator.GetMesh();

		// Create cells
		std::vector<CellPtr> cells;
		CellsGenerator<FixedDurationGenerationBasedCellCycleModel, 2> cells_generator;
		cells_generator.GenerateBasic(cells, p_mesh->GetNumElements());

		// Create cell population
		PottsBasedCellPopulation cell_population(*p_mesh, cells);

		// Create an update law system
		VolumeConstraintUpdateRule<2> volume_constraint;

		// Test get/set methods
		TS_ASSERT_DELTA(volume_constraint.GetDeformationEnergyParameter(), 0.5, 1e-12);
		TS_ASSERT_DELTA(volume_constraint.GetMatureCellTargetVolume(), 16.0, 1e-12);

		volume_constraint.SetDeformationEnergyParameter(0.5);
		volume_constraint.SetMatureCellTargetVolume(0.6);

		TS_ASSERT_DELTA(volume_constraint.GetDeformationEnergyParameter(), 0.5, 1e-12);
		TS_ASSERT_DELTA(volume_constraint.GetMatureCellTargetVolume(), 0.6, 1e-12);

		volume_constraint.SetDeformationEnergyParameter(1.0);
		volume_constraint.SetMatureCellTargetVolume(16.0);

		// Test EvaluateHamiltonianContribution()
		///\todo Check this is the correct numerical value (see #1665)
		double contribution = volume_constraint.EvaluateHamiltonianContribution(0, 1, cell_population);
		TS_ASSERT_DELTA(contribution, 2.0, 1e-6);
	}

    void TestArchiveVolumeConstraintUpdateRule() throw(Exception)
    {
        OutputFileHandler handler("archive", false);
        std::string archive_filename = handler.GetOutputDirectoryFullPath() + "VolumeConstraintUpdateRule.arch";

        {
            VolumeConstraintUpdateRule<2> update_rule;

            std::ofstream ofs(archive_filename.c_str());
            boost::archive::text_oarchive output_arch(ofs);

            // Set member variables
            update_rule.SetDeformationEnergyParameter(0.5);
            update_rule.SetMatureCellTargetVolume(0.6);

            // Serialize via pointer to most abstract class possible
            AbstractPottsUpdateRule<2>* const p_update_rule = &update_rule;
            output_arch << p_update_rule;
        }

        {
            AbstractPottsUpdateRule<2>* p_update_rule;

            // Create an input archive
            std::ifstream ifs(archive_filename.c_str(), std::ios::binary);
            boost::archive::text_iarchive input_arch(ifs);

            // Restore from the archive
            input_arch >> p_update_rule;

            // Test the member data
            TS_ASSERT_DELTA((static_cast<VolumeConstraintUpdateRule<2>*>(p_update_rule))->GetDeformationEnergyParameter(), 0.5, 1e-6);
            TS_ASSERT_DELTA((static_cast<VolumeConstraintUpdateRule<2>*>(p_update_rule))->GetMatureCellTargetVolume(), 0.6, 1e-6);

            // Tidy up
            delete p_update_rule;
        }
    }

    void TestAdhesionUpdateRuleMethods() throw (Exception)
    {
    	// Create a simple 2D PottsMesh with 2 elements
    	PottsMeshGenerator<2> generator(4, 1, 2, 4, 2, 2);
		PottsMesh<2>* p_mesh = generator.GetMesh();

		// Create cells
		std::vector<CellPtr> cells;
		CellsGenerator<FixedDurationGenerationBasedCellCycleModel, 2> cells_generator;
		cells_generator.GenerateBasic(cells, p_mesh->GetNumElements());

		// Create cell population
		PottsBasedCellPopulation cell_population(*p_mesh, cells);

		// Create an update law system
		AdhesionUpdateRule<2> adhesion_update;

		// Test get/set methods
	 	TS_ASSERT_DELTA(adhesion_update.GetCellCellAdhesionEnergyParameter(), 0.1, 1e-12);
		TS_ASSERT_DELTA(adhesion_update.GetCellBoundaryAdhesionEnergyParameter(), 0.2, 1e-12);

		adhesion_update.SetCellCellAdhesionEnergyParameter(0.5);
		adhesion_update.SetCellBoundaryAdhesionEnergyParameter(0.6);

     	TS_ASSERT_DELTA(adhesion_update.GetCellCellAdhesionEnergyParameter(), 0.5, 1e-12);
		TS_ASSERT_DELTA(adhesion_update.GetCellBoundaryAdhesionEnergyParameter(), 0.6, 1e-12);

		adhesion_update.SetCellCellAdhesionEnergyParameter(0.1);
		adhesion_update.SetCellBoundaryAdhesionEnergyParameter(0.2);

        // Test get adhesion methods
		CellPtr p_cell_0 =  cell_population.GetCellUsingLocationIndex(0u);
		CellPtr p_cell_1 =  cell_population.GetCellUsingLocationIndex(1u);
        TS_ASSERT_DELTA(adhesion_update.GetCellCellAdhesionEnergy(p_cell_0,p_cell_1), 0.1, 1e-12);
        TS_ASSERT_DELTA(adhesion_update.GetCellBoundaryAdhesionEnergy(p_cell_0), 0.2, 1e-12);


        // Test EvaluateHamiltonianContribution()
        ///\todo Check this is the correct numerical value (see #1665)
        double contribution = adhesion_update.EvaluateHamiltonianContribution(0, 1, cell_population);
        TS_ASSERT_DELTA(contribution, 0.0, 1e-6);
    }

    void TestArchiveAdhesionUpdateRule() throw(Exception)
    {
        OutputFileHandler handler("archive", false);
        std::string archive_filename = handler.GetOutputDirectoryFullPath() + "AdhesionUpdateRule.arch";

        {
            AdhesionUpdateRule<2> update_rule;

            std::ofstream ofs(archive_filename.c_str());
            boost::archive::text_oarchive output_arch(ofs);

            // Set member variables
            update_rule.SetCellCellAdhesionEnergyParameter(0.5);
            update_rule.SetCellBoundaryAdhesionEnergyParameter(0.6);

            // Serialize via pointer to most abstract class possible
            AbstractPottsUpdateRule<2>* const p_update_rule = &update_rule;
            output_arch << p_update_rule;
        }

        {
            AbstractPottsUpdateRule<2>* p_update_rule;

            // Create an input archive
            std::ifstream ifs(archive_filename.c_str(), std::ios::binary);
            boost::archive::text_iarchive input_arch(ifs);

            // Restore from the archive
            input_arch >> p_update_rule;

            // Test the member data
            TS_ASSERT_DELTA((static_cast<AdhesionUpdateRule<2>*>(p_update_rule))->GetCellCellAdhesionEnergyParameter(), 0.5, 1e-6);
            TS_ASSERT_DELTA((static_cast<AdhesionUpdateRule<2>*>(p_update_rule))->GetCellBoundaryAdhesionEnergyParameter(), 0.6, 1e-6);

            // Tidy up
            delete p_update_rule;
        }
    }

    void TestDifferentialAdhesionUpdateRuleMethods() throw (Exception)
    {
        // Create a simple 2D PottsMesh with 4 elements
        PottsMeshGenerator<2> generator(5, 2, 2, 4, 2, 2);
        PottsMesh<2>* p_mesh = generator.GetMesh();

        // Create cells
        std::vector<CellPtr> cells;
        CellsGenerator<FixedDurationGenerationBasedCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasic(cells, p_mesh->GetNumElements());

        // Label cells 0 and 1
        boost::shared_ptr<AbstractCellProperty> p_label(new CellLabel);
        cells[0]->AddCellProperty(p_label);
        cells[1]->AddCellProperty(p_label);

        // Create cell population
        PottsBasedCellPopulation cell_population(*p_mesh, cells);

        // Create an update law system
        DifferentialAdhesionUpdateRule<2> differential_adhesion_update;

        // Test get/set methods
        TS_ASSERT_DELTA(differential_adhesion_update.GetLabelledCellLabelledCellAdhesionEnergyParameter(), 0.1, 1e-12);
        TS_ASSERT_DELTA(differential_adhesion_update.GetLabelledCellCellAdhesionEnergyParameter(), 0.1, 1e-12);
        TS_ASSERT_DELTA(differential_adhesion_update.GetLabelledCellBoundaryAdhesionEnergyParameter(), 0.2, 1e-12);

        differential_adhesion_update.SetLabelledCellLabelledCellAdhesionEnergyParameter(0.3);
        differential_adhesion_update.SetLabelledCellCellAdhesionEnergyParameter(0.4);
        differential_adhesion_update.SetLabelledCellBoundaryAdhesionEnergyParameter(0.5);

        TS_ASSERT_DELTA(differential_adhesion_update.GetLabelledCellLabelledCellAdhesionEnergyParameter(), 0.3, 1e-12);
        TS_ASSERT_DELTA(differential_adhesion_update.GetLabelledCellCellAdhesionEnergyParameter(), 0.4, 1e-12);
        TS_ASSERT_DELTA(differential_adhesion_update.GetLabelledCellBoundaryAdhesionEnergyParameter(), 0.5, 1e-12);

        // Test get adhesion methods
        CellPtr p_labelled_cell_0 = cell_population.GetCellUsingLocationIndex(0u);
        CellPtr p_labelled_cell_1 = cell_population.GetCellUsingLocationIndex(1u);
        CellPtr p_cell_0 = cell_population.GetCellUsingLocationIndex(2u);
        CellPtr p_cell_1 = cell_population.GetCellUsingLocationIndex(3u);
        TS_ASSERT_DELTA(differential_adhesion_update.GetCellCellAdhesionEnergy(p_labelled_cell_0,p_labelled_cell_1), 0.3, 1e-12);
        TS_ASSERT_DELTA(differential_adhesion_update.GetCellCellAdhesionEnergy(p_labelled_cell_0,p_cell_0), 0.4, 1e-12);
        TS_ASSERT_DELTA(differential_adhesion_update.GetCellCellAdhesionEnergy(p_cell_0,p_cell_1), 0.1, 1e-12);// Default value from AdhesionUpdateRule
        TS_ASSERT_DELTA(differential_adhesion_update.GetCellBoundaryAdhesionEnergy(p_labelled_cell_0), 0.5, 1e-12);
        TS_ASSERT_DELTA(differential_adhesion_update.GetCellBoundaryAdhesionEnergy(p_cell_0), 0.2, 1e-12); // Default value from AdhesionUpdateRule

        // Reset the parameters
        differential_adhesion_update.SetLabelledCellLabelledCellAdhesionEnergyParameter(0.1);
        differential_adhesion_update.SetLabelledCellCellAdhesionEnergyParameter(0.1);
        differential_adhesion_update.SetCellBoundaryAdhesionEnergyParameter(0.2);


        // Test EvaluateHamiltonianContribution()
        ///\todo Check this is the correct numerical value (see #1665)
        double contribution = differential_adhesion_update.EvaluateHamiltonianContribution(0, 1, cell_population);
        TS_ASSERT_DELTA(contribution, 0.0, 1e-6);
    }

    void TestDifferentialArchiveAdhesionUpdateRule() throw(Exception)
    {
        OutputFileHandler handler("archive", false);
        std::string archive_filename = handler.GetOutputDirectoryFullPath() + "AdhesionUpdateRule.arch";

        {
            DifferentialAdhesionUpdateRule<2> update_rule;

            std::ofstream ofs(archive_filename.c_str());
            boost::archive::text_oarchive output_arch(ofs);

            // Set member variables
            update_rule.SetLabelledCellLabelledCellAdhesionEnergyParameter(0.3);
            update_rule.SetLabelledCellCellAdhesionEnergyParameter(0.4);
            update_rule.SetCellCellAdhesionEnergyParameter(0.5);
            update_rule.SetLabelledCellBoundaryAdhesionEnergyParameter(0.6);
            update_rule.SetCellBoundaryAdhesionEnergyParameter(0.7);

            // Serialize via pointer to most abstract class possible
            AbstractPottsUpdateRule<2>* const p_update_rule = &update_rule;
            output_arch << p_update_rule;
        }

        {
            AbstractPottsUpdateRule<2>* p_update_rule;

            // Create an input archive
            std::ifstream ifs(archive_filename.c_str(), std::ios::binary);
            boost::archive::text_iarchive input_arch(ifs);

            // Restore from the archive
            input_arch >> p_update_rule;

            // Test the member data
            TS_ASSERT_DELTA((static_cast<DifferentialAdhesionUpdateRule<2>*>(p_update_rule))->GetLabelledCellLabelledCellAdhesionEnergyParameter(), 0.3, 1e-6);
            TS_ASSERT_DELTA((static_cast<DifferentialAdhesionUpdateRule<2>*>(p_update_rule))->GetLabelledCellCellAdhesionEnergyParameter(), 0.4, 1e-6);
            TS_ASSERT_DELTA((static_cast<DifferentialAdhesionUpdateRule<2>*>(p_update_rule))->GetCellCellAdhesionEnergyParameter(), 0.5, 1e-6);
            TS_ASSERT_DELTA((static_cast<DifferentialAdhesionUpdateRule<2>*>(p_update_rule))->GetLabelledCellBoundaryAdhesionEnergyParameter(), 0.6, 1e-6);
            TS_ASSERT_DELTA((static_cast<DifferentialAdhesionUpdateRule<2>*>(p_update_rule))->GetCellBoundaryAdhesionEnergyParameter(), 0.7, 1e-6);

            // Tidy up
            delete p_update_rule;
        }
    }


	void TestUpdateRuleOutputUpdateRuleInfo()
	{
		std::string output_directory = "TestPottsUpdateRulesOutputParameters";
		OutputFileHandler output_file_handler(output_directory, false);

		// Test with VolumeConstraintUpdateRule
		VolumeConstraintUpdateRule<2> volume_constraint;
        volume_constraint.SetDeformationEnergyParameter(0.1);
        volume_constraint.SetMatureCellTargetVolume(20);

		TS_ASSERT_EQUALS(volume_constraint.GetIdentifier(), "VolumeConstraintUpdateRule-2");

		out_stream volume_constraint_parameter_file = output_file_handler.OpenOutputFile("volume_constraint_results.parameters");
		volume_constraint.OutputUpdateRuleInfo(volume_constraint_parameter_file);
		volume_constraint_parameter_file->close();

		std::string volume_constraint_results_dir = output_file_handler.GetOutputDirectoryFullPath();
		TS_ASSERT_EQUALS(system(("diff " + volume_constraint_results_dir + "volume_constraint_results.parameters notforrelease_cell_based/test/data/TestUpdateRules/volume_constraint_results.parameters").c_str()), 0);

		// Test with AdhesionUpdateRule
		AdhesionUpdateRule<2> adhesion_update;
        adhesion_update.SetCellCellAdhesionEnergyParameter(0.3);
        adhesion_update.SetCellBoundaryAdhesionEnergyParameter(0.4);

		TS_ASSERT_EQUALS(adhesion_update.GetIdentifier(), "AdhesionUpdateRule-2");

		out_stream adhesion_update_parameter_file = output_file_handler.OpenOutputFile("adhesion_update_results.parameters");
		adhesion_update.OutputUpdateRuleInfo(adhesion_update_parameter_file);
		adhesion_update_parameter_file->close();

		std::string adhesion_update_results_dir = output_file_handler.GetOutputDirectoryFullPath();
		TS_ASSERT_EQUALS(system(("diff " + adhesion_update_results_dir + "adhesion_update_results.parameters notforrelease_cell_based/test/data/TestUpdateRules/adhesion_update_results.parameters").c_str()), 0);

        // Test with VolumeConstraintUpdateRule
        DifferentialAdhesionUpdateRule<2> differential_adhesion_update;
        differential_adhesion_update.SetLabelledCellLabelledCellAdhesionEnergyParameter(0.3);
        differential_adhesion_update.SetLabelledCellCellAdhesionEnergyParameter(0.4);
        differential_adhesion_update.SetCellCellAdhesionEnergyParameter(0.5);
        differential_adhesion_update.SetLabelledCellBoundaryAdhesionEnergyParameter(0.6);
        differential_adhesion_update.SetCellBoundaryAdhesionEnergyParameter(0.7);


        TS_ASSERT_EQUALS(differential_adhesion_update.GetIdentifier(), "DifferentialAdhesionUpdateRule-2");

        out_stream differential_adhesion_update_parameter_file = output_file_handler.OpenOutputFile("differential_adhesion_update_results.parameters");
        differential_adhesion_update.OutputUpdateRuleInfo(differential_adhesion_update_parameter_file);
        differential_adhesion_update_parameter_file->close();

        std::string differential_adhesion_update_results_dir = output_file_handler.GetOutputDirectoryFullPath();
        TS_ASSERT_EQUALS(system(("diff " + differential_adhesion_update_results_dir + "differential_adhesion_update_results.parameters notforrelease_cell_based/test/data/TestUpdateRules/differential_adhesion_update_results.parameters").c_str()), 0);
	}
};

#endif /*TESTPOTTSUPDATERULES_HPP_*/
