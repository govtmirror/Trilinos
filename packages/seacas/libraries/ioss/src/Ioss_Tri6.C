// Copyright(C) 1999-2010
// Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
// certain rights in this software.
//         
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
// 
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Sandia Corporation nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// ========================================================================
// Define a variable type for storage of this elements connectivity
#include <Ioss_Tri6.h>
#include <Ioss_ElementVariableType.h>   // for ElementVariableType
#include <assert.h>                     // for assert
#include <stddef.h>                     // for NULL
#include "Ioss_CodeTypes.h"             // for IntVector
#include "Ioss_ElementTopology.h"       // for ElementTopology


namespace Ioss {
  class St_Tri6 : public ElementVariableType
  {
  public:
    static void factory() {static St_Tri6 registerThis;}
  protected:
    St_Tri6()
      : ElementVariableType("tri6", 6) {}
  };
}
//------------------------------------------------------------------------
namespace {
  struct Constants {
    static const int nnode = 6;
    static const int nedge = 3;
    static const int nedgenode = 3;
    static const int nface = 0;
    static const int nfacenode = 0;
    static const int nfaceedge = 0;
    static int edge_node_order[nedge][nedgenode];
  };

  // Edge numbers are zero-based [0..number_edges)
  int Constants::edge_node_order[nedge][nedgenode] = // [edge][edge_node]
    { {0,1,3}, {1,2,4}, {2,0,5} };
}

void Ioss::Tri6::factory()
{
  static Ioss::Tri6 registerThis;
  Ioss::St_Tri6::factory();
}

Ioss::Tri6::Tri6()
  : Ioss::ElementTopology("tri6", "Triangle_6")
{
  Ioss::ElementTopology::alias("tri6", "triangle6");
  Ioss::ElementTopology::alias("tri6", "Solid_Tri_6_2D");
  Ioss::ElementTopology::alias("tri6", "Face_Tri_6_3D");
  Ioss::ElementTopology::alias("tri6", "TRIANGLE_6_2D");
  Ioss::ElementTopology::alias("tri6", "triface6");
}

Ioss::Tri6::~Tri6() {}

int Ioss::Tri6::parametric_dimension()           const {return  2;}
int Ioss::Tri6::spatial_dimension()           const {return  2;}
int Ioss::Tri6::order()               const {return  2;}

int Ioss::Tri6::number_corner_nodes() const {return 3;}
int Ioss::Tri6::number_nodes()        const {return Constants::nnode;}
int Ioss::Tri6::number_edges()        const {return Constants::nedge;}
int Ioss::Tri6::number_faces()        const {return Constants::nface;}

int Ioss::Tri6::number_nodes_edge(int /* edge */) const {return  Constants::nedgenode;}
int Ioss::Tri6::number_nodes_face(int /* face */) const {return  Constants::nfacenode;}
int Ioss::Tri6::number_edges_face(int /* face */) const {return  Constants::nfaceedge;}

Ioss::IntVector Ioss::Tri6::edge_connectivity(int edge_number) const
{
  assert(edge_number > 0 && edge_number <= number_edges());
  Ioss::IntVector connectivity(Constants::nedgenode);

  for (int i=0; i < Constants::nedgenode; i++)
    connectivity[i] = Constants::edge_node_order[edge_number-1][i];

  return connectivity;
}

Ioss::IntVector Ioss::Tri6::face_connectivity(int /* face_number */) const
{
  Ioss::IntVector connectivity;
  return connectivity;
}

Ioss::IntVector Ioss::Tri6::element_connectivity() const
{
  Ioss::IntVector connectivity(number_nodes());

  for (int i=0; i < number_nodes(); i++)
    connectivity[i] = i;

  return connectivity;
}

Ioss::ElementTopology* Ioss::Tri6::face_type(int /* face_number */) const
{
  return (Ioss::ElementTopology*)NULL;
}

Ioss::ElementTopology* Ioss::Tri6::edge_type(int edge_number) const
{
  assert(edge_number >= 0 && edge_number <= number_edges());
  return Ioss::ElementTopology::factory("edge3");
}
