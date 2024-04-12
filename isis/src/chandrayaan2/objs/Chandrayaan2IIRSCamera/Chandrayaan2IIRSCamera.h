#ifndef Chandrayaan2IIRSCamera_h
#define Chandrayaan2IIRSCamera_h

/** This is free and unencumbered software released into the public domain.

The authors of ISIS do not claim copyright on the contents of this file.
For more details about the LICENSE terms and the AUTHORS, you will
find files of those names at the top level of this repository. **/

/* SPDX-License-Identifier: CC0-1.0 */

#include "LineScanCamera.h"

namespace Isis {
  /**
   * @brief Chandrayaan1 M3 Camera Model
   *
   * This is the camera model for the Chandrayaan1 M3 (Moon Mineralogy Mapper) Camera.
   *
   * @ingroup SpiceInstrumentsAndCameras
   * @ingroup Chandrayaan1
   *
   * @author 2013-08-18 Stuart Sides
   *
   * @internal
   *   @history 2015-08-11 Ian Humphrey and Makayla Shepherd - Added new data members and methods
   *                           to get spacecraft and instrument names. Extended unit test to test
   *                           these methods.
   *   @history 2015-10-16 Ian Humphrey - Removed declarations of spacecraft and instrument
   *                           members and methods and removed implementation of these methods
   *                           since Camera now handles this. References #2335.
   *   @history 2016-08-01 Kristin Berry - Added to unitTest to test camera model performance on
   *                           level 2 cubes and to test RA/DEC values. References #2400.
   */
  class Chandrayaan2IIRSCamera : public LineScanCamera {
    public:
      Chandrayaan2IIRSCamera(Cube &cube);

      //! Destroys the Chandrayaan2IIRSCamera object.
      ~Chandrayaan2IIRSCamera() {};

      /**
       * CK frame ID -  - Instrument Code from spacit run on CK
       *
       * @return @b int The appropriate instrument code for the "Camera-matrix"
       *         Kernel Frame ID
       */
      virtual int CkFrameId() const { return (-152001); }

      /**
       *  CK Reference ID - J2000
       *
       * @return @b int The appropriate instrument code for the "Camera-matrix"
       *         Kernel Reference ID
       */
      virtual int CkReferenceId() const { return (1); }

      /**
       * SPK Reference ID - J2000
       *
       * @return @b int The appropriate instrument code for the Spacecraft
       *         Kernel Reference ID
       */
      virtual int SpkReferenceId() const { return (1); }
  };
};
#endif
