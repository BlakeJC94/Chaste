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
#include "PottsElement.hpp"
#include "RandomNumberGenerator.hpp"
#include <cassert>
#include "Exception.hpp"
#include "Debug.hpp"


template<unsigned DIM>
PottsElement<DIM>::PottsElement(unsigned index, const std::vector<Node<DIM>*>& rNodes)
    : MutableElement<DIM,DIM>(index, rNodes)
{
    this->RegisterWithNodes();
}

template<unsigned DIM>
PottsElement<DIM>::~PottsElement()
{
}

template<unsigned DIM>
void PottsElement<DIM>::AddNode(Node<DIM>* pNode,  const unsigned& rIndex)
{
    // Add element to this node
    pNode->AddElement(this->mIndex);

    // Add pNode to mNodes
    this->mNodes.push_back(pNode);
}

template<unsigned DIM>
double PottsElement<DIM>::GetAspectRatio()
{
	assert(DIM==2);

	// See http://stackoverflow.com/questions/7059841/estimating-aspect-ratio-of-a-convex-hull for how to do it.

	// Calculate entries of covariance matrix (var_x,cov_xy;cov_xy,var_y)
	double mean_x=0;
	double mean_y=0;

	for (unsigned i=0; i<this->GetNumNodes(); i++)
	{
		mean_x += this->mNodes[i]->rGetLocation()[0];
		mean_y += this->mNodes[i]->rGetLocation()[1];
	}
	mean_x /= this->GetNumNodes();
	mean_y /= this->GetNumNodes();

	double variance_x = 0;
	double variance_y = 0;
	double covariance_xy = 0;

	for (unsigned i=0; i<this->GetNumNodes(); i++)
	{
		variance_x += pow((this->mNodes[i]->rGetLocation()[0]-mean_x),2);
		variance_y += pow((this->mNodes[i]->rGetLocation()[1]-mean_y),2);
		covariance_xy += (this->mNodes[i]->rGetLocation()[0]-mean_x)*(this->mNodes[i]->rGetLocation()[1]-mean_y);
	}
	variance_x /= this->GetNumNodes();
	variance_y /= this->GetNumNodes();
	covariance_xy /= this->GetNumNodes();

	// Calculate max/min eigenvalues
	double trace = variance_x+variance_y;
	double det = variance_x*variance_y - covariance_xy*covariance_xy;

	double eig_max = 0.5*(trace+sqrt(trace*trace - 4*det));
	double eig_min = 0.5*(trace-sqrt(trace*trace - 4*det));


	// As matrix is Symmetric Positive Definate
	assert(eig_min >=0);
	assert(eig_max >=0);


    // This trips because all nodes in an element are in a line so you get an infinite aspect ratio.
    if(eig_min==0)
	{
    	EXCEPTION("All nodes in an element are in a line so you get an infinite aspect ratio terms and this interferes with calculating the Hamiltonian.");
	}

	return eig_max/eig_min;


}


/////////////////////////////////////////////////////////////////////////////////////
// Explicit instantiation
/////////////////////////////////////////////////////////////////////////////////////

template class PottsElement<1>;
template class PottsElement<2>;
template class PottsElement<3>;
