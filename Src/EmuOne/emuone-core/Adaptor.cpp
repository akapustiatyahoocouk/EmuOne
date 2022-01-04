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
Adaptor::Adaptor(const QString & name, Component * adaptedComponent)
    :   _name(name),
        _adaptedComponent(adaptedComponent)
{
    Q_ASSERT(_adaptedComponent != nullptr);
}

Adaptor::~Adaptor()
{
}

//  End of emuone-core/Adaptor.cpp
