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

#include "PottsBasedCellPopulation.hpp"
#include "CellwiseData.hpp"
#include "VertexMeshWriter.hpp"
#include "Warnings.hpp"
#include "Debug.hpp"

////template<unsigned DIM>
PottsBasedCellPopulation::PottsBasedCellPopulation(PottsMesh& rMesh,
                                          std::vector<CellPtr>& rCells,
                                          bool deleteMesh,
                                          bool validate,
                                          const std::vector<unsigned> locationIndices)
    : AbstractCellPopulation<2>(rCells, locationIndices),
      mrMesh(rMesh),
      mDeleteMesh(deleteMesh)
{
    // Check each element has only one cell attached
    if (validate)
    {
        Validate();
    }
}


////template<unsigned DIM>
//PottsBasedCellPopulation::PottsBasedCellPopulation(PottsMesh& rMesh)
//             : mrMesh(rMesh)
//{
//    mDeleteMesh = true;
//}


//template<unsigned DIM>
PottsBasedCellPopulation::~PottsBasedCellPopulation()
{
    if (mDeleteMesh)
    {
        // Not used until archiving is implemented
        #define COVERAGE_IGNORE
        delete &mrMesh;
        #undef COVERAGE_IGNORE
    }
}


//template<unsigned DIM>
PottsMesh& PottsBasedCellPopulation::rGetMesh()
{
    return mrMesh;
}


//template<unsigned DIM>
const PottsMesh& PottsBasedCellPopulation::rGetMesh() const
{
    return mrMesh;
}


//template<unsigned DIM>
PottsElement* PottsBasedCellPopulation::GetElement(unsigned elementIndex)
{
    return mrMesh.GetElement(elementIndex);
}


//template<unsigned DIM>
unsigned PottsBasedCellPopulation::GetNumNodes()
{
    return mrMesh.GetNumNodes();
}


//template<unsigned DIM>
c_vector<double, 2> PottsBasedCellPopulation::GetLocationOfCellCentre(CellPtr pCell)
{
    return mrMesh.GetCentroidOfElement(this->mCellLocationMap[pCell.get()]);
}


//template<unsigned DIM>
Node<2>* PottsBasedCellPopulation::GetNode(unsigned index)
{
    return mrMesh.GetNode(index);

}


//template<unsigned DIM>
unsigned PottsBasedCellPopulation::AddNode(Node<2>* pNewNode)
{
    //return mrMesh.AddNode(pNewNode);
    return 0u;
}


//template<unsigned DIM>
void PottsBasedCellPopulation::SetNode(unsigned nodeIndex, ChastePoint<2>& rNewLocation)
{
    //mrMesh.SetNode(nodeIndex, rNewLocation);
}


//template<unsigned DIM>
PottsElement* PottsBasedCellPopulation::GetElementCorrespondingToCell(CellPtr pCell)
{
    return mrMesh.GetElement(this->mCellLocationMap[pCell.get()]);
}


//template<unsigned DIM>
unsigned PottsBasedCellPopulation::GetNumElements()
{
    return mrMesh.GetNumElements();
}


//template<unsigned DIM>
CellPtr PottsBasedCellPopulation::AddCell(CellPtr pNewCell, const c_vector<double,2>& rCellDivisionVector, CellPtr pParentCell)
{
    //Method Not Written Yet
    #define COVERAGE_IGNORE
    assert(0);
    #undef COVERAGE_IGNORE

    // Get the element associated with this cell
    //PottsElement* p_element = GetElementCorrespondingToCell(pParentCell);

    // Divide the element
//    unsigned new_element_index=0u;
//    if (norm_2(rCellDivisionVector) < DBL_EPSILON)
//    {
//        // If the cell division vector is the default zero vector, divide the element along the short axis
//        //new_element_index = mrMesh.DivideElementAlongShortAxis(p_element, true);
//    }
//    else
//    {
//        // If the cell division vector has any non-zero component, divide the element along this axis
//        //new_element_index = mrMesh.DivideElementAlongGivenAxis(p_element, rCellDivisionVector, true);
//    }
//
//    // Associate the new cell with the element
//    this->mCells.push_back(pNewCell);

    // Update location cell map
    CellPtr p_created_cell = this->mCells.back();
//    this->mLocationCellMap[new_element_index] = p_created_cell;
//    this->mCellLocationMap[p_created_cell.get()] = new_element_index;
    return p_created_cell;
}


//template<unsigned DIM>
unsigned PottsBasedCellPopulation::RemoveDeadCells()
{
    unsigned num_removed = 0;

    for (std::list<CellPtr>::iterator it = this->mCells.begin();
         it != this->mCells.end();
         ++it)
    {
        if ((*it)->IsDead())
        {
            // Remove the element from the mesh
            num_removed++;
            //mrMesh.DeleteElementPriorToReMesh(this->mCellLocationMap[(*it).get()]);
            it = this->mCells.erase(it);
            --it;
        }
    }
    return num_removed;
}


//template<unsigned DIM>
void PottsBasedCellPopulation::UpdateNodeLocations(const std::vector< c_vector<double, 2> >& rNodeForces, double dt)
{
    // Randomly move elements
    // Loop over elements and exchange
    for (PottsMesh::PottsElementIterator elem_iter = mrMesh.GetElementIteratorBegin();
                elem_iter != mrMesh.GetElementIteratorEnd();
                 ++elem_iter)
    {
        // add the next node to the Element
        unsigned last_node = elem_iter->GetNodeGlobalIndex(elem_iter->GetNumNodes()-1);
        unsigned next_node;
        if (last_node +1  < mrMesh.GetNumNodes())
        {
            next_node = last_node +1;
        }
        else
        {
            next_node = 0;
        }

        std::set<unsigned> containing_elem_indices = GetNode(next_node)->rGetContainingElementIndices();

        // Iterate over these elements to remove node
        for (std::set<unsigned>::iterator iter = containing_elem_indices.begin();
             iter != containing_elem_indices.end();
             ++iter)
        {
            GetElement(*iter)->DeleteNode(GetElement(*iter)->GetNodeLocalIndex(next_node));
        }

        // Now add node to original element
        elem_iter->AddNode(mrMesh.GetNode(next_node));
    }

}


//template<unsigned DIM>
bool PottsBasedCellPopulation::IsCellAssociatedWithADeletedLocation(CellPtr pCell)
{
    return GetElementCorrespondingToCell(pCell)->IsDeleted();;
}


//template<unsigned DIM>
void PottsBasedCellPopulation::Update(bool hasHadBirthsOrDeaths)
{
//    VertexElementMap element_map(mrMesh.GetNumAllElements());
//
//    mrMesh.ReMesh(element_map);
//
//    if (!element_map.IsIdentityMap())
//    {
//    	// Fix up the mappings between CellPtrs and VertexElements
//        std::map<Cell*, unsigned> old_map = this->mCellLocationMap;
//
//        this->mCellLocationMap.clear();
//        this->mLocationCellMap.clear();
//
//        for (std::list<CellPtr>::iterator cell_iter = this->mCells.begin();
//             cell_iter != this->mCells.end();
//             ++cell_iter)
//        {
//            // This shouldn't ever happen, as the cell vector only contains living cells
//            unsigned old_elem_index = old_map[(*cell_iter).get()];
//
//            if (element_map.IsDeleted(old_elem_index))
//            {
//            	/*\todo this is a kludge to remove the cell once a T2Swap occurs this is not included in the dead cells counter.
//            	 * This should be included in the RemoveDeadCells method so the death is counted
//            	 */
//            	WARNING("Cell removed due to T2Swap this is not counted in the dead cells counter");
//            	cell_iter = this->mCells.erase(cell_iter);
//            	--cell_iter;
//            }
//            else
//            {
//				unsigned new_elem_index = element_map.GetNewIndex(old_elem_index);
//
//				this->mLocationCellMap[new_elem_index] = *cell_iter;
//				this->mCellLocationMap[(*cell_iter).get()] = new_elem_index;
//            }
//        }
//
//        // Check that each VertexElement has only one CellPtr associated with it in the updated cell population
//        Validate();
//    }
//
//    element_map.ResetToIdentity();
}


//template<unsigned DIM>
void PottsBasedCellPopulation::Validate()
{
	// Check each element has only one cell attached
    std::vector<unsigned> validated_element = std::vector<unsigned>(this->GetNumElements(), 0);

    for (AbstractCellPopulation<2>::Iterator cell_iter = this->Begin();
         cell_iter != this->End();
         ++cell_iter)
    {
        unsigned elem_index = GetLocationIndexUsingCell(*cell_iter);
        validated_element[elem_index]++;
    }

    for (unsigned i=0; i<validated_element.size(); i++)
    {
        if (validated_element[i] == 0)
        {
            std::stringstream ss;
            ss << "Element " << i << " does not appear to have a cell associated with it";
            EXCEPTION(ss.str());
        }

        if (validated_element[i] > 1)
        {
            std::stringstream ss;
            ss << "Element " << i << " appears to have " << validated_element[i] << " cells associated with it";
            EXCEPTION(ss.str());
        }
    }
}


//template<unsigned DIM>
std::set<unsigned> PottsBasedCellPopulation::GetNeighbouringNodeIndices(unsigned nodeIndex)
{
    // Get the neighbouring node indices as a vector
    std::vector<unsigned> neighbouring_node_indices_vector = GetNeighbouringNodeIndicesVector(nodeIndex);

    // Now populate a std::set with these node indices
    std::set<unsigned> all_neighbours;
    for (unsigned i=0; i<neighbouring_node_indices_vector.size(); i++)
    {
        all_neighbours.insert(neighbouring_node_indices_vector[i]);
    }

    return all_neighbours;
}


//template<unsigned DIM>
std::vector<unsigned> PottsBasedCellPopulation::GetNeighbouringNodeIndicesVector(unsigned nodeIndex)
{
    std::vector<unsigned> all_neighbours;

//    switch (DIM)
//    {
//        case 1:
//        {
//            double width = this->mrMesh.GetWidth(0);
//            unsigned nodes_across = (unsigned)width + 1; // Getting the number of nodes along x axis of mesh
//
//            /*
//             * This stores the available neighbours using the following numbering:
//             *
//             * 0------x------1
//             *
//             */
//
//            // Create a vector of possible neighbouring node indices
//            std::vector<unsigned> neighbour_indices(2, nodeIndex);
//            neighbour_indices[0] -= 1;
//            neighbour_indices[1] += 1;
//
//            // Work out whether this node lies on any edge of the mesh
//            bool on_west_edge = (nodeIndex == 0);
//            bool on_east_edge = (nodeIndex == nodes_across-1);
//
//            // Create a vector of booleans for which neighbours are available
//            std::vector<bool> available_neighbours = std::vector<bool>(8, true);
//            available_neighbours[0] = !on_west_edge;
//            available_neighbours[1] = !on_east_edge;
//
//            // Using neighbour_indices and available_neighbours, store the indices of all available neighbours to the set all_neighbours
//            for (unsigned i=0; i<2; i++)
//            {
//                if (available_neighbours[i])
//                {
//                    all_neighbours.push_back(neighbour_indices[i]);
//                }
//            }
//            break;
//        }
//        case 2:
//        {
            double width = this->mrMesh.GetWidth(0);
            unsigned nodes_across = (unsigned)width + 1; // Getting the number of nodes along x axis of mesh

            double height = this->mrMesh.GetWidth(1);
            unsigned nodes_up = (unsigned)height + 1;

            /*
             * This stores the available neighbours using the following numbering:
             *
             *  1-----0-----7
             *  |     |     |
             *  |     |     |
             *  2-----x-----6
             *  |     |     |
             *  |     |     |
             *  3-----4-----5
             */

            // Create a vector of possible neighbouring node indices
            std::vector<unsigned> neighbour_indices(8, nodeIndex);
            neighbour_indices[0] += nodes_across;
            neighbour_indices[1] += nodes_across - 1;
            neighbour_indices[2] -= 1;
            neighbour_indices[3] -= nodes_across + 1;
            neighbour_indices[4] -= nodes_across;
            neighbour_indices[5] -= nodes_across - 1;
            neighbour_indices[6] += 1;
            neighbour_indices[7] += nodes_across + 1;

            // Work out whether this node lies on any edge of the mesh
            bool on_south_edge = (nodeIndex < nodes_across);
            bool on_north_edge = (nodeIndex > nodes_up*(nodes_across - 1)-1);
            bool on_west_edge = (nodeIndex%nodes_across == 0);
            bool on_east_edge = (nodeIndex%nodes_across == nodes_across - 1);

            // Create a vector of booleans for which neighbours are available
            // Use the order N, NW, W, SW, S, SE, E, NE
            std::vector<bool> available_neighbours = std::vector<bool>(8, true);
            available_neighbours[0] = !on_north_edge;
            available_neighbours[1] = !(on_north_edge || on_west_edge);// || mUseVonNeumannNeighbourhoods);
            available_neighbours[2] = !on_west_edge;
            available_neighbours[3] = !(on_south_edge || on_west_edge);// || mUseVonNeumannNeighbourhoods);
            available_neighbours[4] = !on_south_edge;
            available_neighbours[5] = !(on_south_edge || on_east_edge);// || mUseVonNeumannNeighbourhoods);
            available_neighbours[6] = !on_east_edge;
            available_neighbours[7] = !(on_north_edge || on_east_edge);// || mUseVonNeumannNeighbourhoods);

            // Using neighbour_indices and available_neighbours, store the indices of all available neighbours to the set all_neighbours
            for (unsigned i=0; i<8; i++)
            {
                if (available_neighbours[i])
                {
                    all_neighbours.push_back(neighbour_indices[i]);
                }
            }
//            break;
//        }
//        case 3:
//        {
//            double width = this->mrMesh.GetWidth(0);
//            unsigned nodes_across = (unsigned)width + 1; // Getting the number of nodes along x axis of mesh
//
//            double height = this->mrMesh.GetWidth(1);
//            unsigned nodes_up = (unsigned)height + 1; // Getting the number of nodes along y axis of mesh
//
//            double depth = this->mrMesh.GetWidth(2);
//            unsigned nodes_deep = (unsigned)depth + 1; // Getting the number of nodes along z axis of mesh
//
//            unsigned nodes_layer = nodes_across*nodes_up; // Number of nodes in each layer
//
//            /*
//             * This stores the available neighbours using the following numbering:
//             *
//             *     6------7------8           14-----15-----16            23-----24-----25
//             *     |      |      |           |      |      |             |      |      |
//             *     |      |      |           |      |      |             |      |      |
//             *     3------4------5           12-----x------13            20-----21-----22
//             *     |      |      |           |      |      |             |      |      |
//             *     |      |      |           |      |      |             |      |      |
//             *     0------1------2           9------10-----11            17-----18-----19
//             *
//             */
//
//            // Create a vector of possible neighbouring node indices
//            std::vector<unsigned> neighbour_indices(26, nodeIndex);
//            for (unsigned i=0; i<9; i++)
//            {
//                neighbour_indices[i] -= nodes_layer;
//                neighbour_indices[25-i] += nodes_layer;
//            }
//            for (unsigned i=0; i<3; i++)
//            {
//                neighbour_indices[i] -= nodes_across;
//                neighbour_indices[9+i] -= nodes_across;
//                neighbour_indices[17+i] -= nodes_across;
//                neighbour_indices[6+i] += nodes_across;
//                neighbour_indices[14+i] += nodes_across;
//                neighbour_indices[23+i] += nodes_across;
//            }
//            for (unsigned i=0; i<4; i++)
//            {
//                neighbour_indices[3*i] -= 1;
//                neighbour_indices[25-3*i] += 1;
//                neighbour_indices[2+3*i] += 1;
//                neighbour_indices[23-3*i] -= 1;
//            }
//            neighbour_indices[12] -= 1;
//            neighbour_indices[13] += 1;
//
//            // Work out whether this node lies on any face of the mesh
//            bool on_bottom_face = (nodeIndex < nodes_layer);
//            bool on_top_face = (nodeIndex > (nodes_deep-1)*nodes_layer-1);
//            bool on_south_face = (nodeIndex%nodes_layer < nodes_across);
//            bool on_north_face = (nodeIndex%nodes_layer > nodes_across*(nodes_up-1)-1);
//            bool on_west_face = (nodeIndex%nodes_across == 0);
//            bool on_east_face = (nodeIndex%nodes_across == nodes_across - 1);
//
//            // Create a vector of booleans for which neighbours are available
//            std::vector<bool> available_neighbours = std::vector<bool>(26, true);
//            available_neighbours[0] = !(on_bottom_face || on_west_face || on_south_face || mUseVonNeumannNeighbourhoods);
//            available_neighbours[1] = !(on_bottom_face || on_south_face || mUseVonNeumannNeighbourhoods);
//            available_neighbours[2] = !(on_bottom_face || on_east_face || on_south_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[3] = !(on_bottom_face || on_west_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[4] = !on_bottom_face;
//            available_neighbours[5] = !(on_bottom_face || on_east_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[6] = !(on_bottom_face || on_west_face || on_north_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[7] = !(on_bottom_face || on_north_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[8] = !(on_bottom_face || on_east_face || on_north_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[9] = !(on_west_face || on_south_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[10] = !on_south_face;
//            available_neighbours[11] = !(on_east_face || on_south_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[12] = !on_west_face;
//            available_neighbours[13] = !on_east_face;
//            available_neighbours[14] = !(on_west_face || on_north_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[15] = !on_north_face;
//            available_neighbours[16] = !(on_east_face || on_north_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[17] = !(on_top_face || on_west_face || on_south_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[18] = !(on_top_face || on_south_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[19] = !(on_top_face || on_east_face || on_south_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[20] = !(on_top_face || on_west_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[21] = !on_top_face;
//            available_neighbours[22] = !(on_top_face || on_east_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[23] = !(on_top_face || on_west_face || on_north_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[24] = !(on_top_face || on_north_face|| mUseVonNeumannNeighbourhoods);
//            available_neighbours[25] = !(on_top_face || on_east_face || on_north_face|| mUseVonNeumannNeighbourhoods);
//
//            // Using neighbour_indices and available_neighbours, store the indices of all available neighbours to the set all_neighbours
//            for (unsigned i=0; i<26; i++)
//            {
//                if (available_neighbours[i])
//                {
//                    all_neighbours.push_back(neighbour_indices[i]);
//                }
//            }
//            break;
//        }
//        default:
//            NEVER_REACHED;
//    }
    return all_neighbours;
}

//template<unsigned DIM>
void PottsBasedCellPopulation::WriteResultsToFiles()
{
    // Only works for 2D at present
    //assert(DIM ==2);

    AbstractCellPopulation<2>::WriteResultsToFiles();

    SimulationTime* p_time = SimulationTime::Instance();

    // Write element data to file
    *mpVizElementsFile << p_time->GetTime() << "\t";

    // Loop over cells and find associated elements so in the same order as the cells in output files
    for (std::list<CellPtr>::iterator cell_iter = this->mCells.begin();
         cell_iter != this->mCells.end();
         ++cell_iter)
    {
    	unsigned elem_index = this->GetLocationIndexUsingCell(*cell_iter);

    	// Hack that covers the case where the element is associated with a cell that has just been killed (#1129)
		bool elem_corresponds_to_dead_cell = false;

		if (this->mLocationCellMap[elem_index])
		{
			elem_corresponds_to_dead_cell = this->mLocationCellMap[elem_index]->IsDead();
		}

		// Write node data to file
		if ( !(GetElement(elem_index)->IsDeleted()) && !elem_corresponds_to_dead_cell)
		{
			PottsElement* p_element = mrMesh.GetElement(elem_index);

			unsigned num_nodes_in_element = p_element->GetNumNodes();

			// First write the number of Nodes belonging to this PottsElement
			*mpVizElementsFile << num_nodes_in_element << " ";

			// Then write the global index of each Node in this element
			for (unsigned i=0; i<num_nodes_in_element; i++)
			{
				*mpVizElementsFile << p_element->GetNodeGlobalIndex(i) << " ";
			}
		}
    }
    *mpVizElementsFile << "\n";
}


//template<unsigned DIM>
void PottsBasedCellPopulation::CreateOutputFiles(const std::string& rDirectory, bool cleanOutputDirectory)
{
    AbstractCellPopulation<2>::CreateOutputFiles(rDirectory, cleanOutputDirectory);

    OutputFileHandler output_file_handler(rDirectory, cleanOutputDirectory);
    mpVizElementsFile = output_file_handler.OpenOutputFile("results.vizelements");
}


//template<unsigned DIM>
void PottsBasedCellPopulation::CloseOutputFiles()
{
    AbstractCellPopulation<2>::CloseOutputFiles();
    mpVizElementsFile->close();
}

//template<unsigned DIM>
void PottsBasedCellPopulation::GenerateCellResultsAndWriteToFiles()
{
    // Set up cell type counter
    unsigned num_cell_types = this->mCellProliferativeTypeCount.size();
    std::vector<unsigned> cell_type_counter(num_cell_types);
    for (unsigned i=0; i<num_cell_types; i++)
    {
        cell_type_counter[i] = 0;
    }

    // Set up cell cycle phase counter
    unsigned num_cell_cycle_phases = this->mCellCyclePhaseCount.size();
    std::vector<unsigned> cell_cycle_phase_counter(num_cell_cycle_phases);
    for (unsigned i=0; i<num_cell_cycle_phases; i++)
    {
        cell_cycle_phase_counter[i] = 0;
    }

    for (AbstractCellPopulation<2>::Iterator cell_iter = this->Begin();
         cell_iter != this->End();
         ++cell_iter)
    {
        this->GenerateCellResults(this->GetLocationIndexUsingCell(*cell_iter), cell_type_counter, cell_cycle_phase_counter);
    }

    this->WriteCellResultsToFiles(cell_type_counter, cell_cycle_phase_counter);
}

//template<unsigned DIM>
void PottsBasedCellPopulation::OutputCellPopulationParameters(out_stream& rParamsFile)
{
	// Call direct parent class method
	AbstractCellPopulation<2>::OutputCellPopulationParameters(rParamsFile);
}

//template<unsigned DIM>
double PottsBasedCellPopulation::GetWidth(const unsigned& rDimension)
{
    // Call GetWidth() on the mesh
    double width = mrMesh.GetWidth(rDimension);

    return width;
}

//template<unsigned DIM>
double PottsBasedCellPopulation::GetDampingConstant(unsigned nodeIndex)
{
    // \TODO Method Not Needed for this population type need to refactor out
    #define COVERAGE_IGNORE
    assert(0);
    #undef COVERAGE_IGNORE
    return 0.0;
}

/////////////////////////////////////////////////////////////////////////////
// Explicit instantiation
/////////////////////////////////////////////////////////////////////////////

//template class PottsBasedCellPopulation<1>;
//template class PottsBasedCellPopulation<2>;
//template class PottsBasedCellPopulation<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(PottsBasedCellPopulation)
