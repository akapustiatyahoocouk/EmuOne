//
//  emuone-hadesos/HadesOs.hpp
//
//  The HADES OS component
//
//////////
#pragma once

namespace hadesos
{
    //////////
    //  A HADES OS
    class EMUONE_HADESOS_EXPORT HadesOs : public core::Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(HadesOs)

        //////////
        //  Types
    public:
        //  The "type" of "HADES OS" component
        class EMUONE_HADESOS_EXPORT Type : public core::ComponentType
        {
            DECLARE_SINGLETON(Type)

            //////////
            //  StockObject
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;

            //////////
            //  ComponentType
        public:
            virtual core::ComponentCategory *   category() const override;
            virtual bool        isCompatibleWith(const core::Architecture * architecture) const override;
            virtual HadesOs *   createComponent() override;
        };

        //  A "shared host folder" that appears as a "volume" in HADES
        class EMUONE_HADESOS_EXPORT SharedHostFolder
        {
            //////////
            //  Construction/destruction/assignment
        public:
            SharedHostFolder() : _volumeName(), _hostPath() {}  //  "invalid" one
            SharedHostFolder(const QString & volumeName, const QString & hostPath)
                :   _volumeName(volumeName), _hostPath(hostPath) {}

            //////////
            //  Operators
        public:
            bool                operator == (const SharedHostFolder & op2) const { return _volumeName == op2._volumeName && _hostPath == op2._hostPath; }
            bool                operator != (const SharedHostFolder & op2) const { return _volumeName != op2._volumeName || _hostPath != op2._hostPath; }

            //////////
            //  Operations
        public:
            //  Checks if this "shared host folder" is valid, i.e. its "volume name"
            //  and "host path" are both non-empty
            bool                isValid() const { return _volumeName.trimmed().length() > 0 && _hostPath.trimmed().length() > 0; }

            QString             volumeName() const { return _volumeName; }
            QString             hostPath() const { return _hostPath; }

            QString             toString() const { return _volumeName + " => " + _hostPath; }

            //////////
            //  Implementation
        private:
            QString             _volumeName;//  Vilume name for HADES
            QString             _hostPath;  //  full path in the host OS file system
        };
        using SharedHostFolderList = QList<SharedHostFolder>;

        //////////
        //  Construction/destruction
    public:
        explicit HadesOs(const QString & name);
        virtual ~HadesOs();

        //////////
        //  Component
    public:
        virtual Type *                  type() const override;
        virtual core::ComponentEditor * createEditor(QWidget * parent) override;
        virtual QString                 shortStatus() const override;
        virtual core::ComponentUi *     createUi() override;

        //////////
        //  Component (state control) - all thread-safe
    public:
        virtual State       state() const override;
        virtual void        connect() override;     //  throws VirtualApplianceException
        virtual void        initialise() override;  //  throws VirtualApplianceException;
        virtual void        start() override;       //  throws VirtualApplianceException
        virtual void        stop() noexcept override;
        virtual void        deinitialise() noexcept override;
        virtual void        disconnect() noexcept override;

        //////////
        //  Component (serialisation)
    public:
        virtual void        serialiseConfiguration(QDomElement & configurationElement) const override;
        virtual void        deserialiseConfiguration(QDomElement & configurationElement) override;

        //////////
        //  Operations (configuration)
    public:
        //  The unordered list of all configured "shared host folders"
        SharedHostFolderList    sharedHostFolders() const { return _sharedHostFolders; }

        //  Adda a "shared host folder" this component's configuration. If another
        //  "shared host folder" with the same "volumeName" is already configured, it is replaced.
        SharedHostFolder        addSharedHostFolder(const QString & volumeName, const QString & hostPath);

        void                    removeSharedHostFolder(const SharedHostFolder & sharedHostFolder);
        void                    removeSharedHostFolder(const QString & volumeName);

        //////////
        //  Implementation
    private:
        State                   _state = State::Constructed;
        mutable QRecursiveMutex _stateGuard = {};

        //ScpEditorList           _editors = {};  //  ...that have been created so far

        //  Configuration
        SharedHostFolderList    _sharedHostFolders = {};
    };
}

//  End of emuone-hadesos/HadesOs.hpp
