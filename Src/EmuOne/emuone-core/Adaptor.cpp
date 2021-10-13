//
//  emuone-core/Adaptor.cpp
//
//  The core::Adaptor class implementation
//
//////////
#include "emuone-core/API.hpp"
using namespace core;

//////////
//  Construction/destruction
Adaptor::Adaptor(Component * adaptedComponent)
    :   Component(adaptedComponent->name() + "/" + "TODO" + " adaptor"),
        _adaptedComponent(adaptedComponent)
{
    Q_ASSERT(_adaptedComponent != nullptr);
}

Adaptor::~Adaptor()
{
}

//  End of emuone-core/Adaptor.cpp
