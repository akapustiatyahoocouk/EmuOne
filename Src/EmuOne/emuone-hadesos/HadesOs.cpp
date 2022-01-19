//
//  emuone-hadesos/HadesOs.cpp
//
//  The hadesos::HadesOs class implementation
//
//////////
#include "emuone-hadesos/API.hpp"
using namespace hadesos;

//////////
//  Construction/destruction
HadesOs::HadesOs(const QString & name)
    :   core::Component(name)
{
}

HadesOs::~HadesOs()
{
}

//////////
//  Component
HadesOs::Type * HadesOs::type() const
{
    return HadesOs::Type::instance();
}

core::ComponentEditor * HadesOs::createEditor(QWidget * parent)
{
    return new HadesOsEditor(this, parent);
}

QString HadesOs::shortStatus() const
{
    return "operational";
}

core::ComponentUi * HadesOs::createUi()
{
    return nullptr;
}

//////////
//  Component (state control) - all thread-safe
HadesOs::State HadesOs::state() const
{
    QMutexLocker lock(&_stateGuard);
    return _state;
}

void HadesOs::connect()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Constructed)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void HadesOs::initialise()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void HadesOs::start()
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Running;
}

void HadesOs::stop() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Running)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Initialised;
}

void HadesOs::deinitialise() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Initialised)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Connected;
}

void HadesOs::disconnect() noexcept
{
    QMutexLocker lock(&_stateGuard);

    if (_state != State::Connected)
    {   //  OOPS! Can't make this state transiton!
        return;
    }

    //  Done
    _state = State::Constructed;
}

//////////
//  Component (serialisation)
void HadesOs::serialiseConfiguration(QDomElement & configurationElement) const
{
    QString sharedHostFoldersString;
    for (SharedHostFolder sharedHostFolder : _sharedHostFolders)
    {
        if (!sharedHostFoldersString.isEmpty())
        {
            sharedHostFoldersString += "\n";
        }
        sharedHostFoldersString += sharedHostFolder.volumeName();
        sharedHostFoldersString += "=>";
        sharedHostFoldersString += sharedHostFolder.hostPath();
    }
    configurationElement.setAttribute("SharedHostFolders", sharedHostFoldersString);
}

void HadesOs::deserialiseConfiguration(QDomElement & configurationElement)
{
    QStringList sharedHostFolderChunks = configurationElement.attribute("SharedHostFolders").split("\n");
    for (const QString & sharedHostFolderChunk : sharedHostFolderChunks)
    {
        qsizetype separatorIndex = sharedHostFolderChunk.indexOf("=>");
        if (separatorIndex != -1)
        {
            QString volumeName = sharedHostFolderChunk.left(separatorIndex);
            QString hostPath = sharedHostFolderChunk.mid(separatorIndex + 2);
            addSharedHostFolder(volumeName, hostPath);
        }
    }
}

//////////
//  Operations (configuration)
HadesOs::SharedHostFolder HadesOs::addSharedHostFolder(const QString & volumeName, const QString & hostPath)
{
    Q_ASSERT(Validation::isValidVolumeName(volumeName));    //  TODO "hostPath" too

    //  Already exists ?
    for (SharedHostFolder sharedHostFolder : _sharedHostFolders)
    {
        if (sharedHostFolder.volumeName() == volumeName)
        {   //  Remove the old mapping
            _sharedHostFolders.removeAll(sharedHostFolder);
            break;
        }
    }
    //  Add a new mapping
    SharedHostFolder sharedHostFolder(volumeName, hostPath);
    _sharedHostFolders.append(sharedHostFolder);
    return sharedHostFolder;
}

void HadesOs::removeSharedHostFolder(const SharedHostFolder & sharedHostFolder)
{
    _sharedHostFolders.removeAll(sharedHostFolder);
}

void HadesOs::removeSharedHostFolder(const QString & volumeName)
{
    for (qsizetype i = _sharedHostFolders.size() - 1; i > 0; i--)
    {
        if (_sharedHostFolders[i].volumeName() == volumeName)
        {
            _sharedHostFolders.removeAt(i);
        }
    }
}

//  End of emuone-hadesos/HadesOs.cpp
