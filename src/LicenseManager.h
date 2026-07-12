#pragma once
#include <string>

class LicenseManager
{
public:
    bool registerDevice();
    bool verifyLicense();

private:
    std::string getDeviceSerial() const;
};