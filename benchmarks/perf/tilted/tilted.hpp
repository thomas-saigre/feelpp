/* -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t -*- vim:fenc=utf-8:ft=tcl:et:sw=4:ts=4:sts=4

  This file is part of the Feel library

  Author(s): Christophe Prud'homme <christophe.prudhomme@ujf-grenoble.fr>
       Date: 2009-01-04

  Copyright (C) 2009 Christophe Prud'homme
  Copyright (C) 2009-2011 Université Joseph Fourier (Grenoble I)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
/**
   \file tilted.cpp
   \author Christophe Prud'homme <christophe.prudhomme@ujf-grenoble.fr>
   \date 2009-01-04
 */
#if !defined( __FEELPP_BENCH_TILTED_HPP)
#define __FEELPP_BENCH_TILTED_HPP 1

#include <boost/any.hpp>
#include <boost/utility.hpp>

#include <feel/options.hpp>
#include <feel/feelcore/simget.hpp>

#include <feel/feelalg/backend.hpp>

#include <feel/feeldiscr/functionspace.hpp>

#include <feel/feelfilters/gmsh.hpp>
#include <feel/feelfilters/exporter.hpp>
#include <feel/feelpoly/lagrange.hpp>
#include <feel/feelpoly/crouzeixraviart.hpp>



#include <feel/feelmesh/elements.hpp>

#include <feel/feelvf/vf.hpp>

namespace Feel
{
/**
 * \class Tilted class
 * \brief solves the tilted equations
 *
 */
template<int Dim,
         typename BasisU,
         template<uint16_type,uint16_type,uint16_type> class Entity>
class Tilted
    :
public Simget
{
    typedef Simget super;
public:

    typedef double value_type;

    typedef Backend<value_type> backend_type;
    typedef boost::shared_ptr<backend_type> backend_ptrtype;

    /*mesh*/
    typedef Entity<Dim,1,Dim> convex_type;
    typedef Mesh<convex_type> mesh_type;
    typedef boost::shared_ptr<mesh_type> mesh_ptrtype;

    /*basis*/
    //# marker1 #
    typedef BasisU basis_u_type;
    typedef bases<basis_u_type> basis_type;
    //# endmarker1 #

    /*space*/
    typedef FunctionSpace<mesh_type, basis_type> space_type;
    typedef boost::shared_ptr<space_type> space_ptrtype;
    typedef typename space_type::element_type element_type;

    /* export */
    typedef Exporter<mesh_type> export_type;

    Tilted( std::string const& basis_name,
               po::variables_map const& vm, AboutData const& ad )
        :
        super( vm, ad ),
        M_backend(),
        M_basis_name( basis_name ),
        exporter()
    {
        mu = this->vm()["mu"].template as<value_type>();
        penalbc = this->vm()["bccoeff"].template as<value_type>();
    }


    std::string name() const
    {
        return M_basis_name;
    }

    /**
     * run the convergence test
     */
    void run();
    void run( const double* X, unsigned long P, double* Y, unsigned long N )
    {
        run();
    }

private:

    /**
     * export results to ensight format (enabled by  --export cmd line options)
     */
    void exportResults( element_type& u, element_type& v, element_type& k );

private:

    backend_ptrtype M_backend;
    std::string M_basis_name;
    double mu;
    double penalbc;

    boost::shared_ptr<export_type> exporter;
}; // Tilted


template<int Dim, typename BasisU, template<uint16_type,uint16_type,uint16_type> class Entity>
void
Tilted<Dim, BasisU, Entity>::run()
{
    using namespace Feel::vf;

    if ( this->vm().count( "nochdir" ) == false )
    {
        this->changeRepository( boost::format( "perf/%1%/%2%/%3%/h_%4%/" )
                                % this->about().appName()
                                % convex_type::name()
                                % M_basis_name
                                % meshSize() );
    }

    //! init backend
    M_backend = backend_type::build( this->vm() );
    exporter =  boost::shared_ptr<export_type>( Exporter<mesh_type>::New( this->vm(),
                                                                          (boost::format( "%1%-%2%" ) % this->about().appName() % this->level() ).str() ) );

    boost::timer t;
#if defined(KOVASZNAY)
    double xmin = -0.5, xmax=1.5;
    double ymin =  0, ymax=2;
#else
    double xmin = -1, xmax=1;
    double ymin = -1, ymax=1;
#endif
    double shear = this->vm()["shear"].template as<value_type>();
    bool recombine = this->vm()["recombine"].template as<bool>();
    /*
     * First we create the mesh, in the case of quads we wish to have
     * non-regular meshes to ensure that we don't have some super-convergence
     * associated to the mesh. Hence we use recombine=true to recombine
     * triangles generated by a Delaunay algorithm into quads
     */
    auto mesh = loadGMSHMesh( _mesh=new mesh_type, _filename=(boost::format("tilted%1%.msh") % this->level()).str() );
    M_stats.put( "t.init.mesh",t.elapsed() );
    t.restart();
    /*
     * The function space and some associate elements are then defined
     */
    //# marker4 #

    space_ptrtype Xh = space_type::New( mesh );

    auto u = Xh->element();
    auto v = Xh->element();

    Log() << "Data Summary:\n";
    Log() << "   hsize = " << M_meshSize << "\n";
    Log() << "  export = " << this->vm().count( "export" ) << "\n";
    Log() << "      mu = " << mu << "\n";
    Log() << " bccoeff = " << penalbc << "\n";
    Log() << "[mesh]   number of elements: " << Xh->mesh()->numElements() << "\n";
    Log() << "[dof]         number of dof: " << Xh->nDof() << "\n";
    Log() << "[dof]    number of dof/proc: " << Xh->nLocalDof() << "\n";

    M_stats.put( "h",M_meshSize );
    M_stats.put( "n.space.nelts",Xh->mesh()->numElements() );
    M_stats.put( "n.space.ndof",Xh->nLocalDof() );
    M_stats.put( "t.init.space",t.elapsed() );
    Log() << "  -- time space and functions construction "<<t.elapsed()<<" seconds \n";
    t.restart() ;

    double penalbc = this->vm()["bccoeff"].template as<value_type>();
    double mu = this->vm()["mu"].template as<value_type>();

    double kappa = this->vm()["kappa"].template as<value_type>();
    double pi = boost::math::constants::pi<double>();

#if defined(FEELPP_SOLUTION_1)
    double alpha=(3./pi)*math::atan(math::sqrt(1.+2./kappa));
    double beta=math::cos(alpha*pi/3.)/math::cos(2*alpha*pi/3.);

    auto r = sqrt((Px()-0.5)*(Px()-0.5)+(Py()-0.5)*(Py()-0.5));
    auto theta1=acos( (Px()-0.5)/r );
    auto theta =
        chi((Py()-0.5)>=0.)*theta1 +
        chi((Py()-0.5)<0)*(2*pi-theta1);

    auto ralpha=pow(r,alpha);
    auto u_exact =
        chi(0<=theta && theta<2.*pi/3.)*ralpha*cos(alpha*(theta-pi/3.)) +
        chi(2.*pi/3.<=theta && theta<=2.*pi)*ralpha*beta*cos(alpha*(4.*pi/3.-theta));
    auto f = cst(0.);
    auto k =
        chi(0<=theta && theta<2.*pi/3.)*cst(kappa) + // k1
        chi(2.*pi/3.<=theta && theta<=2.*pi)* cst(1.0); // k2
#endif
#if defined(FEELPP_SOLUTION_2)
    double alpha=(3./pi)*math::atan(math::sqrt(1.+2.*kappa));
    double beta=1./(2.*math::cos(alpha*pi/3.));

    auto r = sqrt((Px()-0.5)*(Px()-0.5)+(Py()-0.5)*(Py()-0.5));
    auto theta1=acos( (Px()-0.5)/r );
    auto theta =
        chi((Py()-0.5)>=0.)*theta1 +
        chi((Py()-0.5)<0)*(2*pi-theta1);

    auto ralpha=pow(r,alpha);
    auto u_exact =
        chi(0<=theta && theta<2.*pi/3.)*ralpha*sin(alpha*(theta-pi/3.)) +
        chi(2.*pi/3.<=theta && theta<=2.*pi)*ralpha*beta*sin(alpha*(4.*pi/3.-theta));
    auto f = cst(0.);
    auto k =
        chi(0<=theta && theta<2.*pi/3.)*cst(kappa) + // k1
        chi(2.*pi/3.<=theta && theta<=2.*pi)* cst(1.0); // k2

#endif

#if defined(FEELPP_SOLUTION_3)
    double alpha=(6./pi)*math::atan(1./math::sqrt(1.+2.*kappa));
    double beta=cos(alpha*pi*/3.)*(math::sin(alpha*pi/6.));

    auto r = sqrt((Px()-0.5)*(Px()-0.5)+(Py()-0.5)*(Py()-0.5));
    auto theta1=acos( (Px()-0.5)/r );
    auto theta =
        chi((Py()-0.5)>=0.)*theta1 +
        chi((Py()-0.5)<0)*(2*pi-theta1);

    auto ralpha=pow(r,alpha);
    auto u_exact =
        chi(0<=theta && theta<2.*pi/3.)*ralpha*cos(alpha*(theta-pi/3.)) +
        chi(2.*pi/3.<=theta && theta<=2.*pi)*ralpha*beta*sin(alpha*(5.*pi/6.-theta));
    auto f = cst(0.);
    auto k =
        chi(0<=theta && theta<2.*pi/3.)*cst(kappa) + // k1
        chi(2.*pi/3.<=theta && theta<=2.*pi)* cst(1.0); // k2

#endif


    boost::timer subt;
    // right hand side
    auto F = M_backend->newVector( Xh );
    form1( Xh, F, _init=true );
    M_stats.put( "t.init.vector",t.elapsed() );
    Log() << "  -- time for vector init done in "<<t.elapsed()<<" seconds \n";
    t.restart() ;
    form1( Xh, F ) = integrate( elements( mesh ), trans( f )*id( v ) );
    M_stats.put( "t.assembly.vector.source",subt.elapsed() );
    subt.restart();

    if ( this->vm()[ "bctype" ].template as<int>() == 1  )
    {
        form1( Xh, F ) += integrate( _range=boundaryfaces( mesh ), _expr=-u_exact*k*dn( u ) );
        M_stats.put( "t.assembly.vector.dirichlet1",subt.elapsed() );
        subt.restart();
        form1( Xh, F ) += integrate( _range=boundaryfaces( mesh ), _expr=k*penalbc*inner( u_exact,id( v ) )/hFace() );
        //form1( Xh, F ) += integrate( _range=boundaryfaces(mesh), _expr=penalbc*max(betacoeff,mu/hFace())*(trans(id(v))*N())*N());
        M_stats.put( "t.assembly.vector.dirichlet2",subt.elapsed() );
        Log() << "   o time for rhs weak dirichlet terms: " << subt.elapsed() << "\n";
        subt.restart();
    }

    M_stats.put( "t.assembly.vector.total",t.elapsed() );
    Log() << "  -- time vector global assembly done in "<<t.elapsed()<<" seconds \n";
    t.restart() ;

    /*
     * Construction of the left hand side
     */
    size_type pattern = Pattern::COUPLED;
    size_type patternsym = Pattern::COUPLED;

    if ( this->vm()[ "faster" ].template as<int>() == 1 )
    {
        pattern = Pattern::COUPLED;
        patternsym = Pattern::COUPLED|Pattern::SYMMETRIC;
    }

    if ( this->vm()[ "faster" ].template as<int>() == 2 )
    {
        pattern = Pattern::DEFAULT;
        patternsym = Pattern::DEFAULT;
    }

    if ( this->vm()[ "faster" ].template as<int>() == 3 )
    {
        pattern = Pattern::DEFAULT;
        patternsym = Pattern::DEFAULT|Pattern::SYMMETRIC;
    }

    //# marker7 #
    t.restart();
    auto D = M_backend->newMatrix( Xh, Xh );
    form2( Xh, Xh, D, _init=true );
    M_stats.put( "t.init.matrix",t.elapsed() );
    Log() << "  -- time for matrix init done in "<<t.elapsed()<<" seconds \n";
    t.restart() ;

    subt.restart();
    t.restart();

    form2( Xh, Xh, D, _pattern=patternsym ) =integrate( _range=elements( mesh ),_expr=k*( inner( gradt( u ),grad( v ) ) ) );
    M_stats.put( "t.assembly.matrix.diffusion",subt.elapsed() );
    Log() << "   o time for diffusion terms: " << subt.elapsed() << "\n";
    subt.restart();

    if ( this->vm()[ "bctype" ].template as<int>() == 1  )
    {
        form2( Xh, Xh, D, _pattern=patternsym )+=integrate( _range=boundaryfaces( mesh ),_expr=-k*trans( dnt( u ) )*id( v )-k*trans( dn( u ) )*idt( v ) );
        M_stats.put( "t.assembly.matrix.dirichlet1",subt.elapsed() );
        subt.restart();
        form2( Xh, Xh, D, _pattern=patternsym )+=integrate( _range=boundaryfaces( mesh ),_expr=+k*penalbc*inner( idt( u ),id( v ) )/hFace() );
        M_stats.put( "t.assembly.matrix.dirichlet2",subt.elapsed() );
        subt.restart();
        Log() << "   o time for weak dirichlet terms: " << subt.elapsed() << "\n";
        subt.restart();
    }

    //# endmarker7 #
    D->close();
    F->close();

    if ( this->vm()[ "bctype" ].template as<int>() == 0  )
    {
        form2( Xh, Xh, D ) += on( _range=boundaryfaces( mesh ), _element=u, _rhs=F, _expr=u_exact );
        M_stats.put( "t.assembly.matrix.dirichlet",subt.elapsed() );
        Log() << "   o time for strong dirichlet terms: " << subt.elapsed() << "\n";
        subt.restart();
    }

    M_stats.put( "t.assembly.matrix.total",t.elapsed() );
    Log() << " -- time matrix global assembly done in "<<t.elapsed()<<" seconds \n";
    t.restart() ;



    t.restart();

    if ( !this->vm().count( "no-solve" ) )
    {
        auto r = M_backend->solve( _matrix=D, _solution=u, _rhs=F );
        M_stats.put( "d.solver.bool.converged",r.template get<0>() );
        M_stats.put( "d.solver.int.nit",r.template get<1>() );
        M_stats.put( "d.solver.double.residual",r.template get<2>() );
    }

    M_stats.put( "t.solver.total",t.elapsed() );
    Log() << " -- time for solver : "<<t.elapsed()<<" seconds \n";
    t.restart();


    double meas = integrate( _range=elements( mesh ), _expr=constant( 1.0 ) ).evaluate()( 0, 0 );
    Log() << "[tilted] measure(Omega)=" << meas << " (should be equal to 4)\n";
    std::cout << "[tilted] measure(Omega)=" << meas << " (should be equal to 4)\n";

    double mean_u = integrate( elements( mesh ), idv( u ) ).evaluate()( 0, 0 )/meas;
    Log() << "[tilted] mean(u)=" << mean_u << "\n";
    std::cout << "[tilted] mean(u)=" << mean_u << "\n";

    // get the zero mean pressure
    u.add( - mean_u );
    mean_u = integrate( elements( mesh ), idv( u ) ).evaluate()( 0, 0 )/meas;
    Log() << "[tilted] mean(u-mean(u))=" << mean_u << "\n";
    std::cout << "[tilted] mean(u-mean(u))=" << mean_u << "\n";
    double mean_uexact = integrate( elements( mesh ), u_exact ).evaluate()( 0, 0 )/meas;
    std::cout << "[tilted] mean(uexact)=" << mean_uexact << "\n";
    M_stats.put( "t.integrate.mean",t.elapsed() );
    size_type nnz = 0 ;
    auto nNz = D->graph()->nNz() ;

    for ( auto iter = nNz.begin(); iter!=nNz.end(); ++iter )
        nnz += ( *iter ) ;

    Log() << "[tilted] matrix NNZ "<< nnz << "\n";
    M_stats.put( "n.matrix.nnz",nnz );

    t.restart();
    double u_errorL2 = integrate( _range=elements( mesh ), _expr=trans( idv( u )-u_exact )*( idv( u )-u_exact ) ).evaluate()( 0, 0 );
    double u_exactL2 = integrate( _range=elements( mesh ), _expr=trans( u_exact )*( u_exact ) ).evaluate()( 0, 0 );
    M_stats.put( "t.integrate.l2norm",t.elapsed() );
    t.restart();
    std::cout << "||u_error||_2 = " << math::sqrt( u_errorL2/u_exactL2 ) << "\n";;
    Log() << "||u_error||_2 = " << math::sqrt( u_errorL2/u_exactL2 ) << "\n";;
    M_stats.put( "e.l2.u",math::sqrt( u_errorL2/u_exactL2 ) );

#if 0
    double u_errorsemiH1 = integrate( _range=elements( mesh ),
                                      _expr=trace( ( gradv( u )-grad_exact )*trans( gradv( u )-grad_exact ) ) ).evaluate()( 0, 0 );
    double u_exactsemiH1 = integrate( _range=elements( mesh ),
                                      _expr=trace( ( grad_exact )*trans( grad_exact ) ) ).evaluate()( 0, 0 );
    double u_error_H1 = math::sqrt( ( u_errorL2+u_errorsemiH1 )/( u_exactL2+u_exactsemiH1 ) );
    M_stats.put( "t.integrate.h1norm",t.elapsed() );
    t.restart();
    std::cout << "||u_error||_1= " << u_error_H1 << "\n";
    M_stats.put( "e.h1.u",u_error_H1 );
#endif
    v = vf::project( Xh, elements( Xh->mesh() ), u_exact );
    auto ke = vf::project( Xh, elements( Xh->mesh() ), k );
    M_stats.put( "t.export.projection",t.elapsed() );
    t.restart();
    this->exportResults( u, v, ke );
    M_stats.put( "t.export.total",t.elapsed() );
    t.restart();

} // Tilted::run


template<int Dim, typename BasisU, template<uint16_type,uint16_type,uint16_type> class Entity>
void
Tilted<Dim, BasisU, Entity>::exportResults( element_type& u, element_type& v, element_type& k )
{
    if ( exporter->doExport() )
    {
        exporter->step( 0 )->setMesh( u.functionSpace()->mesh() );
        exporter->step( 0 )->add( (boost::format("u-%1%")%this->level()).str(), u );
        exporter->step( 0 )->add( (boost::format("u_exact-%1%")%this->level()).str(), v );
        exporter->step( 0 )->add( (boost::format("k-%1%")%this->level()).str(), k );
        exporter->save();
    }
} // Tilted::export
} // Feel

#endif // __FEELPP_BENCH_TILTED_HPP
