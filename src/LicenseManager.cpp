#include "LicenseManager.h"
#include <fstream>
#include <iostream>

bool LicenseManager::registerDevice(){
    std::ofstream file("license.dat");

    if(!file)
        return false;

    file << getDeviceSerial();

    return true;
}

bool LicenseManager::verifyLicense(){
    std::ifstream file("license.dat");

    if(!file)
        return false;

    std::string deviceID;
    file >> deviceID;
    
    if(deviceID != getDeviceSerial())
        return false;
        
    return true;
}

std::string LicenseManager::getDeviceSerial() const{
    std::string deviceID = "Test123456";
    return deviceID;
}