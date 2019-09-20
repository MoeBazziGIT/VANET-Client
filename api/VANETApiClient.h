// Author: Mohamad Bazzi 2019 Summer University of Windsor

#ifndef VANETApiClient_H
#define VANETApiClient_H

#include "veins/modules/Prext/base/BasePrivLayer.h"

class VANETApiClient : public cSimpleModule
{

  public:
    void dumpMsgForEavesdropper(int index, WAVEBeacon* bsm, float simTime);

  protected:
    // methods
    virtual void initialize();
    virtual void finish();
    void addEavesdropperToJSON(int index);
    void dumpJSONToServer();

    // params
    bool gotPrivType = false;

};

#endif
