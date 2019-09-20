// Author: Mohamad Bazzi 2019 Summer University of Windsor

#include <ctime>
#include <fstream>
#include <iomanip>
#include "veins/base/utils/FindModule.h"
#include "VANETApiClient.h"
#include "HTTPRequest.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

#define DEV_NAME "" // ADD YOUR NAME HERE

Define_Module(VANETApiClient);

void VANETApiClient::initialize(){

    // getting current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    char *nowStr = ctime(&now);

    // starting JSON object
    json payload = {
            {"start_time", nowStr},
            {"dev_name", DEV_NAME}, // TODO: make 'dev_name' variable dynamic
            {"privacy_scheme", ""},
            {"eavesdroppers", {}}
    };

    int n = getParentModule()->par("nEavesdropper");

    // add n eavesdroppers to JSON payload object
    json evs;
    for(int i = 0; i < n; i++){

        // create JSON object of eavesdropper
        evs = {
            {"index", i},
            {"messages", {}}
        };

        // add this eavesdropper to the payload
        payload["eavesdroppers"].push_back(evs);
    }

    // open this file and truncate the previous data in it so we can begin with and create a new simulation JSON object
    std::ofstream o("simulation.json", std::ofstream::trunc);
    // write JSON object to the file
    o << std::setw(4) << payload << std::endl;
}

void VANETApiClient::dumpMsgForEavesdropper(int index, WAVEBeacon* bsm, float simTime){

    // read the JSON file
    std::ifstream i("simulation.json");
    // dump the JSON from the file into this "payload" JSON object
    json payload; i >> payload;

    if(!gotPrivType){
        // get PrivCar module and the privType param
        string privType = FindModule<BasePrivLayer*>::findGlobalModule()->getParentModule()->par("privType");
        // add the privType (aka privacy scheme) to the payloads "privacy_scheme"
        payload["privacy_scheme"] = privType;
        // set this bool to true because this conditional needs to be called only once
        gotPrivType = true;
    }

    // create new JSON message object for this received WAVEBeacon* bsm
    json message = {
                {"sender_address", bsm->getSenderAddress()},
                {"sender_psynm", bsm->getSenderPsynm()},
                {"sender_vel_x", bsm->getSenderVel().x},
                {"sender_vel_y", bsm->getSenderVel().y},
                {"sender_pos_x", bsm->getSenderPos().x},
                {"sender_pos_y", bsm->getSenderPos().y},
                {"sender_angle", bsm->getSenderAngle()},
                {"simTime_eavesdropped", simTime},
                {"is_encrypted", bsm->getIsEncrypted()}
        };

    // look for this eavesdropper in the payload and add the message to it
    for(int i = 0; i < payload["eavesdroppers"].size(); i++){
        if(payload["eavesdroppers"][i]["index"] == index){ // found this eavesdropper by its index
            // add the message to this eavesdropper
            payload["eavesdroppers"][i]["messages"].push_back(message);
        }
    }

    // update(aka write to file) the payload
    std::ofstream o("simulation.json");
    o << std::setw(4) << payload << std::endl;
}

void VANETApiClient::addEavesdropperToJSON(int index){

    // read the JSON file
    std::ifstream i("simulation.json");
    // dump the JSON from the file into this "payload" JSON object
    json payload; i >> payload;

    // create JSON object of eavesdropper
    json evs = {
        {"index", index},
        {"messages", {}}
    };

    // add this eavesdropper to the payload
    payload["eavesdroppers"].push_back(evs);

    // update(aka write to file) the payload
    std::ofstream o("simulation.json");
    o << std::setw(4) << payload << std::endl;
}

void VANETApiClient::dumpJSONToServer(){

    // read the JSON file
    std::ifstream i("simulation.json");
    // dump the JSON from the file into this "payload" JSON object
    json payload; i >> payload;

    // set up request object for this endpoint
    http::Request request("http://uwinvanetapi.pythonanywhere.com/simulations/");

    // make the POST request and store response
    http::Response response = request.send("POST", payload.dump(), {
        "Content-Type: application/json"
    });
}

void VANETApiClient::finish(){

    dumpJSONToServer();
}
