#include <pybind11/pybind11.h>
/* #include <pybind11/operators.h> */
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "CGALSlice.h"
#include "CGALSurface.h"
#include "CGALMeshCreator.h"
//#include "Neuron.h"
#include "surface_mesher.h"


namespace py = pybind11;


class PyAbstractMap : public AbstractMap{
public:
       using AbstractMap::AbstractMap; /* Inherit constructors */

};

typedef std::function<double(double,double,double)> Surface_implicit_function;


typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point_3;

// We want to construct Polyline in Python then let polyline translate into cgal Point in c++
// 

Point_3 Wrapper_point_3(double x,double y,double z)
{
       return Point_3(x,y,z);
}



PYBIND11_MODULE(SVMTK, m) {

    py::class_<Point_3>(m, "Point_3")
       .def(py::init<double,double,double>())
       .def("x", &Point_3::x);
       
    py::class_<AbstractMap,PyAbstractMap> abstractmap(m,"AbstractMap");

    py::class_<SubdomainMap,AbstractMap>(m, "SubdomainMap")
        .def(py::init<>())
        .def("print",  &SubdomainMap::print)
        .def("add", &SubdomainMap::add);

    py::class_<CGALSlice,std::shared_ptr<CGALSlice>>(m, "Slice")
        .def(py::init<>())
        .def(py::init<CGALSlice&>())
        .def("create_mesh", &CGALSlice::create_mesh) 
        .def("simplify", &CGALSlice::simplify) 
        .def("keep_component", &CGALSlice::keep_component) 
        .def("save", &CGALSlice::save)
        .def("mark_holes", &CGALSlice::find_holes)
        .def("add_constraints",(void (CGALSlice::*)(CGALSlice&)) &CGALSlice::add_constraints);



    py::class_<CGALSurface,std::shared_ptr<CGALSurface>>(m, "Surface")
        .def(py::init<std::string &>())                                             
        .def(py::init<>())

        .def("implicit_surface",  &CGALSurface::implicit_surface<Surface_implicit_function> )  
        .def("triangulate_hole",&CGALSurface::triangulate_hole)
        .def("clip",&CGALSurface::clip )
        .def("intersection", &CGALSurface::surface_intersection)
        .def("union", &CGALSurface::surface_union)
        .def("difference", &CGALSurface::surface_difference)
        .def("slice", &CGALSurface::mesh_slice)
        .def("span", &CGALSurface::span)

        .def("fill_holes", &CGALSurface::fill_holes)
        .def("triangulate_faces", &CGALSurface::triangulate_faces)
        .def("stitch_borders", &CGALSurface::stitch_borders)
        .def("isotropic_remeshing", &CGALSurface::isotropic_remeshing)
        .def("adjust_boundary", &CGALSurface::adjust_boundary)

        .def("smooth_laplacian", &CGALSurface::smooth_laplacian)
        .def("smooth_taubin", &CGALSurface::smooth_taubin)

        // Either use these two for operator overloading, or return the vertices
        .def("points_inside",(CGALSurface::vertex_vector (CGALSurface::*)(CGALSurface&)) &CGALSurface::points_inside)
        .def("points_outside", &CGALSurface::points_outside)
        .def("make_cube", &CGALSurface::make_cube)
	.def("make_cone", &CGALSurface::make_cone)
	.def("make_cylinder", &CGALSurface::make_cylinder)
        .def("make_sphere", &CGALSurface::make_sphere)
        .def("num_self_intersections", &CGALSurface::num_self_intersections) 
        .def("collapse_edges", &CGALSurface::collapse_edges)
        .def("save", &CGALSurface::save)
        .def("split_edges", &CGALSurface::split_edges)


        .def("fair", (void (CGALSurface::*)(CGALSurface::vertex_vector)) &CGALSurface::fair)
        .def("fair", (void (CGALSurface::*)()) &CGALSurface::fair)
        //.def("load", &CGALSurface::load)//
        .def("seperate_close_junctures", &CGALSurface::seperate_close_junctures)
        .def("reconstruct", &CGALSurface::reconstruct)

        .def("num_faces", &CGALSurface::num_faces)
        .def("num_edges", &CGALSurface::num_edges)
        .def("num_vertices", &CGALSurface::num_vertices);

    py::class_<CGALMeshCreator,std::shared_ptr<CGALMeshCreator>>(m, "Domain")
        .def(py::init<CGALSurface &>())
        .def(py::init<std::vector<CGALSurface>>())
        .def(py::init<std::vector<CGALSurface>, AbstractMap&>())
        .def("set_parameters", &CGALMeshCreator::set_parameters) // std::map<std::string, double>
        .def("set_parameter", &CGALMeshCreator::set_parameter)

        .def("create_mesh", (void (CGALMeshCreator::*)()) &CGALMeshCreator::create_mesh) 
        .def("create_mesh", (void (CGALMeshCreator::*)(double)) &CGALMeshCreator::create_mesh)
        .def("refine_mesh", (void (CGALMeshCreator::*)()) &CGALMeshCreator::refine_mesh)
        .def("refine_mesh", (void (CGALMeshCreator::*)(double)) &CGALMeshCreator::refine_mesh)

        .def("get_boundary", &CGALMeshCreator::get_boundary)

        .def("default_creating_mesh", &CGALMeshCreator::default_creating_mesh) // FIXME : REMOVE

        .def("lloyd", &CGALMeshCreator::lloyd)
        .def("odt", &CGALMeshCreator::odt)
        .def("exude", &CGALMeshCreator::exude)
        .def("perturb", &CGALMeshCreator::perturb)
        .def("add_sharp_border_edges", (void (CGALMeshCreator::*)(CGALSurface&)) &CGALMeshCreator::add_sharp_border_edges)
        .def("reset_borders", &CGALMeshCreator::reset_borders)


        .def("number_of_cells", &CGALMeshCreator::number_of_cells)
        .def("set_borders", &CGALMeshCreator::set_borders)
        .def("set_features", (void(CGALMeshCreator::*)()) &CGALMeshCreator::set_features) 
        .def("add_feature", &CGALMeshCreator::add_feature) 
        .def("save", &CGALMeshCreator::save); 






       m.def("surface_overlapp", &surface_overlapp<CGALSurface> );


}
