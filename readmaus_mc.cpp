#include "readmaus_mc.h"

//#include "JsonCppStreamer/IRStream.hh"


ReadMAUS_MC::ReadMAUS_MC()
{
    // Empty ...
}


void ReadMAUS_MC::Read(QString fileToOpen){

    MAUS::Data data;
  //  irstream infile(fileToOpen.toStdString().c_str(), "Spill");

    std::cout << "About to iterate over MC truth events..\n";

//    while(infile >> readEvent != NULL){
//        infile >> branchName("data") >> data;
//        spill = data.GetSpill();

//        if (spill == NULL || spill->GetDaqEventType() != "physics_event " )
//            continue ;

//        readSpill();
//    }
}


void ReadMAUS_MC::readSpill(){
    for(size_t i = 0; i < spill->GetMCEvents()->size(); ++i){

        event = spill->GetAnMCEvent(i);

        for(size_t hit = 0; hit < event.GetVirtualHits()->size(); ++hit){

            virtualhit = event.GetAVirtualHit(hit);
            std::cout << "Spill = " << spill->GetSpillNumber();
            std::cout << ", Virtual hit " << hit;
            std::cout << ",   z = " << virtualhit.GetPosition().z();
        }

    }
}
