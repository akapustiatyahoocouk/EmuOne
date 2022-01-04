//
//  emuone-hadesos-cereon/Plugin.cpp
//
//  The emuone-hadesos-cereon plugin
//
//////////
#include "emuone-hadesos-cereon/API.hpp"

namespace hadesos
{
    namespace cereon
    {
        //////////
        //  The "emuone-hadesos-cereon" plugin
        class Plugin : public util::Plugin
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Plugin)

            //////////
            //  Construction/destruction
        public:
            Plugin() {}
            virtual ~Plugin() {}

            //////////
            //  Plugin
        public:
            virtual void initialise()
            {
                core::AdaptorType::registerAdaptorType(hadesos::cereon::Adaptor::Type::instance());
            }
        };
    }
}

using namespace hadesos::cereon;
BEGIN_PLUGIN_TABLE
    PLUGIN_TABLE_ITEM(Plugin)
END_PLUGIN_TABLE

//  End of emuone-hadesos-cereon/Plugin.cpp
