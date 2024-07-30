//-*-C++-*-

#ifndef TETRAHEDRON_EIGENVALUE_DEGENERACY_H
#define TETRAHEDRON_EIGENVALUE_DEGENERACY_H

enum    eigenvalue_degeneracy {NO_DEGENERACY, TWOFOLD_DEGENERACY, THREEFOLD_DEGENERACY, THREEFOLD_DEGENERACY_A, THREEFOLD_DEGENERACY_B, FOURFOLD_DEGENERACY};
typedef eigenvalue_degeneracy eigenvalue_degeneracy_t;

#endif

#include "simplex.h"
#include "facet.h"

#include "tetrahedron.h"
#include "tetrahedron_1D.h"
#include "tetrahedron_2D.h"
#include "tetrahedron_3D.h"

#include "tetrahedron_mesh_initializer.h"
#include "tetrahedron_mesh_initializer_2D.h"
#include "tetrahedron_mesh_initializer_3D.h"

#include "tetrahedron_mesh.h"

// #include "tetrahedron_mesh_2D.h"
// #include "tetrahedron_mesh_3D.h"

#include "tetrahedron_neighbour_domain.h"
