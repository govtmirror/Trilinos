/*
// @HEADER
// ***********************************************************************
// 
//    OptiPack: Collection of simple Thyra-based Optimization ANAs
//                 Copyright (2009) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Roscoe A. Bartlett (rabartl@sandia.gov) 
// 
// ***********************************************************************
// @HEADER
*/

#ifndef OPTIPACK_DIAGONAL_SCALAR_PROD_DEF_HPP
#define OPTIPACK_DIAGONAL_SCALAR_PROD_DEF_HPP


#include "OptiPack_DiagonalScalarProd_decl.hpp"
#include "Thyra_DetachedSpmdVectorView.hpp"
#include "Thyra_AssertOp.hpp"
#include "Teuchos_CommHelpers.hpp"


namespace OptiPack {


// Consturctors/Initializers/Accessors


template<class Scalar>
DiagonalScalarProd<Scalar>::DiagonalScalarProd()
{}


template<class Scalar>
void DiagonalScalarProd<Scalar>::initialize(
  const RCP<const Thyra::VectorBase<Scalar> > &s_diag )
{
  s_diag_ = s_diag.assert_not_null();
}


// Overridden from ScalarProdBase


template<class Scalar>
bool DiagonalScalarProd<Scalar>::isEuclideanImpl() const
{
  return false;
}


template<class Scalar>
void DiagonalScalarProd<Scalar>::scalarProdsImpl(
  const Thyra::MultiVectorBase<Scalar>& X, const Thyra::MultiVectorBase<Scalar>& Y,
  const ArrayView<Scalar> &scalarProds_out ) const
{

  using Teuchos::as;
  typedef Teuchos::ScalarTraits<Scalar> ST;
  typedef Thyra::Ordinal Ordinal;
  using Thyra::ConstDetachedSpmdVectorView;

  const Ordinal m = X.domain()->dim();

#ifdef TEUCHOS_DEBUG
  THYRA_ASSERT_VEC_SPACES( "DiagonalScalarProd<Scalar>::scalarProds(X,Y,sclarProds)",
    *s_diag_->space(), *Y.range() );
  THYRA_ASSERT_VEC_SPACES( "DiagonalScalarProd<Scalar>::scalarProds(X,Y,sclarProds)",
    *X.range(), *Y.range() );
  THYRA_ASSERT_VEC_SPACES( "DiagonalScalarProd<Scalar>::scalarProds(X,Y,sclarProds)",
    *X.domain(), *Y.domain() );
  TEUCHOS_ASSERT_EQUALITY( as<Ordinal>(scalarProds_out.size()), m );
#endif

  const ConstDetachedSpmdVectorView<Scalar> s_diag(s_diag_);

  const RCP<const Teuchos::Comm<Ordinal> > comm = s_diag.spmdSpace()->getComm();
  
  for (Ordinal j = 0; j < m; ++j) {

    const ConstDetachedSpmdVectorView<Scalar> x(X.col(j));
    const ConstDetachedSpmdVectorView<Scalar> y(Y.col(j));
    
    Scalar scalarProd_j = ST::zero();

    for (Ordinal i = 0; i < x.subDim(); ++i) {
      scalarProd_j += ST::conjugate(x[i]) * s_diag[i] * y[i];
    }

    if (!is_null(comm)) {
      Scalar g_scalarProd_j = 0.0;
      Teuchos::reduceAll<Ordinal,Scalar>(
        *comm, Teuchos::REDUCE_SUM,
        scalarProd_j,
        Teuchos::outArg(g_scalarProd_j)
        );
      scalarProds_out[j] = g_scalarProd_j;
    }
    else {
      scalarProds_out[j] = scalarProd_j;
    }

  }

}


template<class Scalar>
void DiagonalScalarProd<Scalar>::euclideanApplyImpl(
  const Thyra::EuclideanLinearOpBase<Scalar> &M,
  const Thyra::EOpTransp M_trans,
  const Thyra::MultiVectorBase<Scalar> &X,
  const Ptr<Thyra::MultiVectorBase<Scalar> > &Y,
  const Scalar alpha,
  const Scalar beta
  ) const
{
  TEST_FOR_EXCEPT_MSG(true, "Not implemented yet!")
}


} // end namespace OptiPack


#endif  // OPTIPACK_DIAGONAL_SCALAR_PROD_DEF_HPP
