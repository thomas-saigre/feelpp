//! -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t  -*- vim:fenc=utf-8:ft=cpp:et:sw=4:ts=4:sts=4
//!
//! This file is part of the Feel++ library
//!
//! This library is free software; you can redistribute it and/or
//! modify it under the terms of the GNU Lesser General Public
//! License as published by the Free Software Foundation; either
//! version 2.1 of the License, or (at your option) any later version.
//!
//! This library is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//! Lesser General Public License for more details.
//!
//! You should have received a copy of the GNU Lesser General Public
//! License along with this library; if not, write to the Free Software
//! Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//!
//! @file
//! @author Christophe Prud'homme <christophe.prudhomme@feelpp.org>
//! @date 25 Jul 2018
//! @copyright 2018 Feel++ Consortium
//!
#include <pybind11/pybind11.h>

#include <feel/feelmodels/modelcore/modelbase.hpp>
#include <feel/feelmodels/modelcore/modelalgebraic.hpp>
#include <feel/feelmodels/modelcore/modelnumerical.hpp>
#if defined( FEELPP_MODELS_HAS_MESHALE )
#include <feel/feelmodels/modelmesh/meshale.hpp>
#endif
#include <feel/feelmodels/modelcore/options.hpp>

namespace py = pybind11;
using namespace Feel;

template<typename ConvexT>
void defToolbox(py::module &m)
{
    using namespace Feel;
    using namespace Feel::FeelModels;
    using convex_t = ConvexT;
    using mesh_t = Mesh<convex_t>;
    using mesh_ptr_t = std::shared_ptr<mesh_t>;
#if defined( FEELPP_MODELS_HAS_MESHALE )
    using toolbox_t = MeshALE< convex_t >;
    using ale_map_element_t = typename toolbox_t::ale_map_element_type;
    using ale_map_element_ptr_t = typename toolbox_t::ale_map_element_ptrtype;

    std::string pyclass_name = std::string("MeshALE_") + std::to_string(convex_t::nDim) + std::string("DP") + std::to_string(convex_t::nOrder);
    py::class_<toolbox_t,std::shared_ptr<toolbox_t>,ModelBase>(m,pyclass_name.c_str())
        .def(py::init<mesh_ptr_t,std::string const&,worldcomm_ptr_t const&, ModelBaseRepository const&>(),
             py::arg("mesh"),
             py::arg("prefix") = "",
             py::arg("worldComm")=Environment::worldCommPtr(),
             py::arg("modelRep") = ModelBaseRepository(),
             "Initialize the meshALE mechanics toolbox"
             )
        .def("init",static_cast<void (toolbox_t::*)()>(&toolbox_t::init), "initialize the meshALE  toolbox")
        .def("init",static_cast<void (toolbox_t::*)(std::shared_ptr<toolbox_t> const&,std::vector<std::string> const&)>(&toolbox_t::init), "initialize from another meshALE  toolbox and a set of markers")
        // mesh
        .def( "addMarkerInBoundaryCondition", (void (toolbox_t::*)(std::string const&,std::string const&)) &toolbox_t::addMarkerInBoundaryCondition, py::arg("boundary"), py::arg("marker"), "add the boundary flags" )
        .def( "referenceMesh", &toolbox_t::referenceMesh, "get the reference mesh" )
        .def( "movingMesh", &toolbox_t::movingMesh, "get the moving mesh" )
        .def( "isOnReferenceMesh", &toolbox_t::isOnReferenceMesh, "return true if on reference mesh, false otherwise" )
        .def( "isOnMovingMesh", &toolbox_t::isOnMovingMesh, "return true if on moving mesh, false otherwise" )

        // elements
        .def( "functionSpace", &toolbox_t::functionSpace, "get the ALE map function space")
        .def( "identityALE", &toolbox_t::identityALE, "returns the identity ale map" )
        .def( "displacementOnMovingBoundary", &toolbox_t::displacementOnMovingBoundary, "returns the displacement on moving boundary" )
        .def( "displacement", &toolbox_t::displacement, "returns the displacement field" )
        .def( "velocity", &toolbox_t::velocity, "returns the velocity field" )

        .def( "revertReferenceMesh", &toolbox_t::revertReferenceMesh, py::arg("updateMeshMeasures") = true, "revert mesh in reference state" )
        .def( "revertMovingMesh", &toolbox_t::revertMovingMesh, py::arg("updateMeshMeasures") = true, "revert mesh in reference state" )
        .def( "updateTimeStep", &toolbox_t::updateTimeStep, "update time step" )
        .def( "exportResults", &toolbox_t::exportResults, py::arg("time")=0., "export results" );
#endif
}

PYBIND11_MODULE(_modelmesh, m )
{
    using namespace Feel;
    using namespace Feel::FeelModels;

    defToolbox<Simplex<2,1>>( m );
    defToolbox<Simplex<2,2>>( m );
    defToolbox<Simplex<3,1>>( m );
    defToolbox<Simplex<3,2>>( m );
    
}
