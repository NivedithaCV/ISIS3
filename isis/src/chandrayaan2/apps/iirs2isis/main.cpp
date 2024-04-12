/** This is free and unencumbered software released into the public domain.

The authors of ISIS do not claim copyright on the contents of this file.
For more details about the LICENSE terms and the AUTHORS, you will
find files of those names at the top level of this repository. **/

/* SPDX-License-Identifier: CC0-1.0 */

#include "Isis.h"
#include "UserInterface.h"
#include "Cube.h"
#include "FileName.h"
#include "IException.h"
#include "OriginalXmlLabel.h"
#include "ProcessImport.h"

#include "Pvl.h"
#include "PvlKeyword.h"
#include "Table.h"
#include "UserInterface.h"
#include "XmlToPvlTranslationManager.h"
using namespace std;
using namespace Isis;


namespace Isis {
  void iirscoreinfo(FileName &inputLabel, ProcessImport &importer);
  void iirscoreinfo(XmlToPvlTranslationManager iirs_labelxlater,ProcessImport &importer);
  void translateIIRSLabels(FileName &inputLabel, Pvl *outputLabel);
  void iirstranslateLabels(FileName &inputLabel, Pvl *outputLabel, FileName transFile);
}


void IsisMain() {
  
  UserInterface &ui = Application::GetUserInterface();
  FileName xmlFileName = ui.GetFileName("FROM");
  
  try {
      ProcessImport p;
      //function of translateCoreInfo(Filename &inputlabel,Processimport &importer)
      //xmlFileName =xmlFileName
      //p = p
      iirscoreinfo(xmlFileName, p);
      //*****
      if (xmlFileName.removeExtension().addExtension("qub").fileExists()) {
        p.SetInputFile(xmlFileName.removeExtension().addExtension("qub").expanded());
      }
      else {
        QString msg = "Cannot find image file for [" + xmlFileName.name() + "]. Confirm the "
          ".qub file for this XML exists and is located in the same directory.";
        throw IException(IException::User, msg, _FILEINFO_);
      }


      Cube *outputCube = p.SetOutputCube("TO", ui);  //this will be the Processimport object to which the core info will be set
      Pvl *outputLabel = outputCube->label();
      
      // Additional code within the try block
      translateIIRSLabels(xmlFileName, outputLabel);

      FileName outputCubeFileName(ui.GetCubeName("TO"));

      OriginalXmlLabel xmlLabel;
      xmlLabel.readFromXmlFile(xmlFileName);

      p.StartProcess();


      PvlKeyword *instrumentName = &outputLabel->findGroup("Instrument", Pvl::Traverse)["InstrumentId"];
      QString instrumentNameString = instrumentName[0];

      PvlGroup kerns("Kernels");
      if (instrumentNameString == "IIRS") {
        // This ID will need to be updated. It is temporarily used for testing but is NOT the actual
        // NAC ID.
        kerns += PvlKeyword("NaifFrameCode", toString(-152240));
      }
      else {
        QString msg = "Input file [" + xmlFileName.expanded() + "] has an invalid " +
                  "InstrumentId.";
        throw IException(IException::Unknown, msg, _FILEINFO_);
      
      }

      outputCube->putGroup(kerns);

      p.EndProcess();

      
      
  }
  catch (IException &e) {

      QString msg = "Given file [" + xmlFileName.expanded() + "] does not appear to be a valid ";
      throw IException(e, IException::User, msg, _FILEINFO_);
    }
    return;
  }

namespace Isis {
  //function calling xmltopvltranslationmanager object
  void iirscoreinfo(FileName &inputLabel, ProcessImport &importer){
    FileName transfile = "$ISISROOT/appdata/translations/chan2iirsCore.trn";
    if (transfile.fileExists()) {
      QString msg ="Found it";
    }
    else {
      QString msg = "Cannot find transfile [" + transfile.name();
      throw IException(IException::User, msg, _FILEINFO_);
    }
    XmlToPvlTranslationManager iirs_labelxlater(inputLabel, transfile.expanded());
    iirscoreinfo(iirs_labelxlater,importer);

  }

  void iirscoreinfo(XmlToPvlTranslationManager iirs_labelxlater,ProcessImport &importer){

    QString str;
    str = iirs_labelxlater.Translate("CoreSamples");
    int ns = toInt(str);
    str = iirs_labelxlater.Translate("CoreLines");
    int nl = toInt(str);
    str = iirs_labelxlater.Translate("CoreBands");
    int nb = toInt(str);
    importer.SetDimensions(ns, nl, nb);
    importer.SetFileHeaderBytes(0);

    str = iirs_labelxlater.Translate("CoreType");
    importer.SetPixelType(PixelTypeEnumeration(str));

    str = iirs_labelxlater.Translate("CoreByteOrder");
    importer.SetByteOrder(ByteOrderEnumeration(str));

    importer.SetFileHeaderBytes(0);

    str = iirs_labelxlater.Translate("CoreBase");
    importer.SetBase(toDouble(str));
    str = iirs_labelxlater.Translate("CoreMultiplier");

    importer.SetNull(Isis::NULL4, Isis::NULL4);
    importer.SetLRS(Isis::LOW_REPR_SAT4, Isis::LOW_REPR_SAT4);
    importer.SetLIS(Isis::LOW_INSTR_SAT4, Isis::LOW_INSTR_SAT4);
    importer.SetHRS(Isis::HIGH_REPR_SAT4, Isis::HIGH_REPR_SAT4);
    importer.SetHIS(Isis::HIGH_INSTR_SAT4, Isis::HIGH_INSTR_SAT4);
    }

  void translateIIRSLabels(FileName &inputLabel, Pvl *outputLabel){
    iirstranslateLabels(inputLabel, outputLabel, FileName("$ISISROOT/../isis/src/chandrayaan2/apps/chandrayaan2IIRSinstrument.trn"));

  }

  void iirstranslateLabels(FileName &inputLabel, Pvl *outputLabel, FileName transFile)
  {
    XmlToPvlTranslationManager iirs_labelxlater(inputLabel, transFile.expanded());

    // Translate the output label
    iirs_labelxlater.Auto(*(outputLabel));
  }
}