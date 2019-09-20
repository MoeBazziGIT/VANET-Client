# VANET-Client
A client app to upload simulation data to our VANET server for further data analysis
## Table of Contents
**[Dependencies to install](#Dependencies-to-install)** <br>
**[Client side source code](#Client-side-source-code)** <br>

## Dependencies to install
**List of dependencies to install**

## Client side source code
**1.Add the api directory under ```veins/src/veins/``` so that it looks like:** <br/>
<img src="./images/apiDirectoryTree.png" width=250 height=375/> <br/>
**2.Add this import line in the Eavesdropper.h file** <br/>
``` #include "veins/api/VANETApiClient.h" ``` <br/>
**3.Add this decloration of the object under the protected class parameters in the Eavesdropper.h file** <br/>
``` VANETApiClient *api; ``` <br/>
<img src="./images/eavesdropperhSourceCode.png" width=250 height=450/> <br/>
