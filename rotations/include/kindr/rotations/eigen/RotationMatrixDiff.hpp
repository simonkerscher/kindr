/*
 * Copyright (c) 2013, Christian Gehring, Hannes Sommer, Paul Furgale, Remo Diethelm
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autonomous Systems Lab, ETH Zurich nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Christian Gehring, Hannes Sommer, Paul Furgale,
 * Remo Diethelm BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#ifndef KINDR_ROTATIONS_EIGEN_ROTATIONMATRIX_DIFF_HPP_
#define KINDR_ROTATIONS_EIGEN_ROTATIONMATRIX_DIFF_HPP_

#include <Eigen/Core>

#include "kindr/common/common.hpp"
#include "kindr/common/assert_macros.hpp"
#include "kindr/common/assert_macros_eigen.hpp"
#include "kindr/rotations/RotationDiffBase.hpp"
#include "kindr/rotations/RotationEigen.hpp"
#include "kindr/linear_algebra/LinearAlgebra.hpp"

namespace kindr {
namespace rotations {
namespace eigen_impl {


/*! \class RotationMatrixDiff
 * \brief Time derivative of a rotation matrix.
 *
 * This class implements the time derivative of a rotation matrix using a Eigen::Matrix<Scalar, 3, 3> as data storage.
 *
 * \tparam PrimType_  Primitive data type of the coordinates.
 * \ingroup rotations
 */
template<typename PrimType_, enum RotationUsage Usage_>
class RotationMatrixDiff : public RotationMatrixDiffBase<RotationMatrixDiff<PrimType_, Usage_>,Usage_>, private Eigen::Matrix<PrimType_, 3, 3> {
 private:
  /*! \brief The base type.
   */
  typedef Eigen::Matrix<PrimType_, 3, 3> Base;

 public:
  /*! \brief The implementation type.
   *  The implementation type is always an Eigen object.
   */
  typedef Base Implementation;
  /*! \brief The primitive type.
   *  Float/Double
   */
  typedef PrimType_ Scalar;

  RotationMatrixDiff()
    : Base() {
  }

  /*! \brief Constructor using Eigen::Matrix<Scalar, 3, 3>.
   *  \param other   Eigen::Matrix<Scalar, 3, 3>
   */
  explicit RotationMatrixDiff(const Base& other) // explicit on purpose
    : Base(other) {
  }

  /*! \brief Constructor using nine scalars.
   *  \param r11     entry in row 1, col 1
   *  \param r12     entry in row 1, col 2
   *  \param r13     entry in row 1, col 3
   *  \param r21     entry in row 2, col 1
   *  \param r22     entry in row 2, col 2
   *  \param r23     entry in row 2, col 3
   *  \param r31     entry in row 3, col 1
   *  \param r32     entry in row 3, col 2
   *  \param r33     entry in row 3, col 3
   */
  RotationMatrixDiff(Scalar r11, Scalar r12, Scalar r13,
                     Scalar r21, Scalar r22, Scalar r23,
                     Scalar r31, Scalar r32, Scalar r33) {
    *this << r11,r12,r13,r21,r22,r23,r31,r32,r33;
  }

  /*! \brief Constructor using a time derivative with a different parameterization
   *
   * \param rotation  rotation
   * \param other     other time derivative
   */
  template<typename RotationDerived_, typename OtherDerived_>
  inline explicit RotationMatrixDiff(const RotationBase<RotationDerived_, Usage_>& rotation, const RotationDiffBase<OtherDerived_, Usage_>& other)
    : Base(internal::RotationDiffConversionTraits<RotationMatrixDiff, OtherDerived_, RotationDerived_>::convert(rotation.derived(), other.derived())){
  }

  /*! \brief Cast to another representation of the time derivative of a rotation
   *  \param other   other rotation
   *  \returns reference
   */
  template<typename OtherDerived_, typename RotationDerived_>
  OtherDerived_ cast(const RotationBase<RotationDerived_, Usage_>& rotation) const {
    return internal::RotationDiffConversionTraits<OtherDerived_, RotationMatrixDiff, RotationDerived_>::convert(rotation.derived(), *this);
  }

  /*! \brief Cast to the implementation type.
   *  \returns the implementation (recommended only for advanced users)
   */
  inline Implementation& toImplementation() {
    return static_cast<Implementation&>(*this);
  }

  /*! \brief Cast to the implementation type.
   *  \returns the implementation (recommended only for advanced users)
   */
  inline const Implementation& toImplementation() const {
    return static_cast<const Implementation&>(*this);
  }

  /*! \brief Reading access to the time derivative of the rotation matrix.
   *  \returns rotation matrix (matrix) with reading access
   */
  inline const Implementation& matrix() const {
    return this->toImplementation();
  }

  /*! \brief Writing access to the time derivative of the rotation matrix.
   *  \returns rotation matrix (matrix) with writing access
   */
  inline Implementation& matrix() {
    return this->toImplementation();
  }

  /*! \brief Sets all time derivatives to zero.
   *  \returns reference
   */
  RotationMatrixDiff& setZero() {
    this->toImplementation().setZero();
    return *this;
  }

  /*! \brief Used for printing the object with std::cout.
   *  \returns std::stream object
   */
  friend std::ostream& operator << (std::ostream& out, const RotationMatrixDiff& diff) {
    out << diff.toImplementation();
    return out;
  }
};


//! \brief Time derivative of a rotation quaternion with primitive type double
typedef RotationMatrixDiff<double, RotationUsage::PASSIVE> RotationMatrixDiffPD;
//! \brief Time derivative of a rotation quaternion with primitive type float
typedef RotationMatrixDiff<float, RotationUsage::PASSIVE> RotationMatrixDiffPF;
//! \brief Time derivative of a rotation quaternion with primitive type double
typedef RotationMatrixDiff<double, RotationUsage::ACTIVE> RotationMatrixDiffAD;
//! \brief Time derivative of a rotation quaternion with primitive type float
typedef RotationMatrixDiff<float, RotationUsage::ACTIVE> RotationMatrixDiffAF;

} // namespace eigen_impl

namespace internal {




} // namespace internal
} // namespace rotations
} // namespace kindr




#endif /* KINDR_ROTATIONS_EIGEN_ROTATIONMATRIX_DIFF_HPP_ */
