//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "InterfaceReaction1.h"

registerMooseObject("InterfaceReaction1App", InterfaceReaction1);

template <>
InputParameters
validParams<InterfaceReaction1>()
{
  InputParameters params = validParams<InterfaceKernel>();
  params.addParam<MaterialPropertyName>("D", "D", "The diffusion coefficient.");
  params.addParam<MaterialPropertyName>(
      "D_neighbor", "D_neighbor", "The neighboring diffusion coefficient.");
  params.addRequiredParam<Real>("kf", "Forward reaction rate coefficient.");
  params.addRequiredParam<Real>("kb", "Backward reaction rate coefficient.");
  params.addClassDescription("Implements a reaction to establish Flux=k_f*u-k_b*v "
                             "at interface.");
  return params;
}

InterfaceReaction1::InterfaceReaction1(const InputParameters & parameters)
  : InterfaceKernel(parameters),
    _kf(getParam<Real>("kf")),
    _kb(getParam<Real>("kb")),
    _D(getMaterialProperty<Real>("D")),
    _D_neighbor(getNeighborMaterialProperty<Real>("D_neighbor"))
{
}

Real
InterfaceReaction1::computeQpResidual(Moose::DGResidualType type)
{
  Real r = 0;

  switch (type)
  {

      // Move all the terms to the LHS to get residual, for master domain
      // Residual = kf*u - kb*v = kf*u - kb*v
      // Weak form for master domain is: (test, kf*u - kb*v)

    case Moose::Element:
      r = _test[_i][_qp] * ( _kf * _u[_qp] - _kb * _neighbor_value[_qp]);
      break;

    // Similarly, weak form for slave domain is: -(test, kf*u - kb*v),
    // flip the sign because the direction is opposite.
    case Moose::Neighbor:
      r = -_test_neighbor[_i][_qp] * ( _kf * _u[_qp] - _kb * _neighbor_value[_qp]);
      break;
  }

  return r;
}

Real
InterfaceReaction1::computeQpJacobian(Moose::DGJacobianType type)
{
  Real jac = 0;

  switch (type)
  {
    case Moose::ElementElement:
      jac = _test[_i][_qp] * _kf * _phi[_j][_qp];
      break;

    case Moose::NeighborNeighbor:
      jac = -_test_neighbor[_i][_qp] * -_kb * _phi_neighbor[_j][_qp];
      break;

    case Moose::NeighborElement:
      jac = -_test_neighbor[_i][_qp] * _kf * _phi[_j][_qp];
      break;

    case Moose::ElementNeighbor:
      jac = _test[_i][_qp] * - _kb * _phi_neighbor[_j][_qp];
      break;
  }

  return jac;
}
