#include "Chandrayaan2IIRSCamera.h"

#include <QString>

#include "CameraDistortionMap.h"
#include "CameraFocalPlaneMap.h"
#include "IException.h"
#include "iTime.h"
#include "IString.h"
#include "LineScanCameraDetectorMap.h"
#include "LineScanCameraGroundMap.h"
#include "LineScanCameraSkyMap.h"
#include "NaifStatus.h"
#define DUMP_INFO 1

using namespace std;
namespace Isis {
  /**
   * Constructs a Chandrayaan 1 M3 Camera object using the image labels.
   *
   * @ingroup SpiceInstrumentsAndCameras
   * @ingroup MarsReconnaissanceOrbiter
   *
   * @author 2013-08-18 Stuart Sides and Tracie Sucharski
   *
   * @internal
   *   @history 2013-08-18 Stuart Sides - Original version.
   */
  //considering same model as chandryaan2 m3 taking LineScanCamra
  Chandrayaan2IIRSCamera::Chandrayaan2IIRSCamera(Cube &cube) : LineScanCamera(cube) {
    m_instrumentNameLong = "imaging infrared spectrometer";
    m_instrumentNameShort = "IIRS";

    m_spacecraftNameLong = "CHANDRAYAAN-2";
    m_spacecraftNameShort = "CH2";

    NaifStatus::CheckErrors();
    // Set up the camera info from ik/iak kernels
    SetFocalLength();
    //demo pixel pitch not the real value (need to add to iak)
    SetPixelPitch(0.03);

    // Get the start time from labelsP
    // Get the start time from labels
    Pvl &lab = *cube.label();
    PvlGroup &inst = lab.findGroup("Instrument", Pvl::Traverse);
    QString stime = inst["StartTime"];
    iTime startTime;
    startTime.setUtc((QString)inst["StartTime"]);
    double etStart = iTime(startTime).Et();


    // // Get other info from labels
    double csum = inst["SpatialSumming"];
    double lineRate = (double) inst["LineExposureDuration"] / 1000.0;
    //lineRate *= csum;

    // // Setup detector map
    LineScanCameraDetectorMap *detectorMap =
      new LineScanCameraDetectorMap(this, etStart, lineRate);
    detectorMap->SetDetectorSampleSumming(csum);

    // // Setup focal plane map
    CameraFocalPlaneMap *focalMap = new CameraFocalPlaneMap(this, naifIkCode());

    // //  Retrieve boresight location from instrument kernel (IK) (addendum?)
    QString ikernKey = "INS" + toString((int)naifIkCode()) + "_BORESIGHT_SAMPLE";
    double sampleBoreSight = getDouble(ikernKey);

    ikernKey = "INS" + toString((int)naifIkCode()) + "_BORESIGHT_LINE";
    double lineBoreSight = getDouble(ikernKey);

    focalMap->SetDetectorOrigin(sampleBoreSight, lineBoreSight);
    focalMap->SetDetectorOffset(0.0, 0.0);

    // QString ppKey("INS" + toString(naifIkCode()) + "_PP");
    // QString odKey("INS" + toString(naifIkCode()) + "_OD_K");
    // QString decenterKey("INS" + toString(naifIkCode()) + "_DECENTER");

    // Setup the ground and sky map
    new CameraDistortionMap(this);
    new LineScanCameraGroundMap(this);
    new LineScanCameraSkyMap(this);

    LoadCache();
    NaifStatus::CheckErrors();

    #if defined(DUMP_INFO)
    Table cache = instrumentRotation()->Cache("Loaded");
    cout << "Total Records: " << cache.Records() << "\n";

    for (int i = 0 ; i < cache.Records() ; i++) {
      TableRecord rec = cache[i];
      string separator("");
      for (int f = 0 ; f < rec.Fields() ; f++) {
        cout << separator << (double) rec[f];
        separator = ", ";
      }
      cout << "\n";
    }
    #endif


    
  }
}


/**
 * This is the function that is called in order to instantiate an Chandrayaan1M3Camera object.
 *
 * @param lab Cube labels
 *
 * @return Isis::Camera* CHANDRAYAAB2IIRS
 * 
 *
 */
extern "C" Isis::Camera *Chandrayaan2IIRSCameraPlugin(Isis::Cube &cube) {
  return new Isis::Chandrayaan2IIRSCamera(cube);
}
