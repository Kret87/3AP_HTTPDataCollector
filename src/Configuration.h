#pragma once

#include <string>
#include <vector>

#include "Endpoint.h"

class Configuration
{
public:

    bool load(const std::string& fileName);

    const std::vector<Endpoint>& endpoints() const;

private:

    std::vector<Endpoint> endpoints_;
};
