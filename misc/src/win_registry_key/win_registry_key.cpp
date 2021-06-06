#include "win_registry_key.h"

template <class T>
class Buffer
{
public:
    Buffer(int size)
        : m_size(size)
    {
        m_ptr = new T[size];
    }
    ~Buffer()
    {
        delete []m_ptr;
    }

    T* begin()
    {
        return m_ptr;
    }
    
    const T* begin() const
    {
        return m_ptr;
    }

    T& operator[] (int index)
    {
        return m_ptr[index];
    }

    const T& operator[] (int index) const
    {
        return m_ptr[index];
    }

    operator T* ()
    {
        return m_ptr;
    }

private:
    T* m_ptr;
    int m_size;
};

namespace
{
    class AutoHandle
    {
    public:
        AutoHandle(HMODULE h):
            _h(h)
        {
        }

        ~AutoHandle()
        {
            FreeLibrary(_h);
        }

        HMODULE handle()
        {
            return _h;
        }

    private:
        HMODULE _h;
    };
}


WinRegistryKey::WinRegistryKey(const std::string& key, bool readOnly, REGSAM extraSam):
    m_hKey(0),
    m_readOnly(readOnly),
    m_extraSam(extraSam)
{
    std::string::size_type pos = key.find('\\');
    if (pos != std::string::npos)
    {
        std::string rootKey = key.substr(0, pos);
        m_hRootKey = handleFor(rootKey);
        m_subKey   = key.substr(pos + 1);
    }
}


WinRegistryKey::WinRegistryKey(HKEY hRootKey, const std::string& subKey, bool readOnly, REGSAM extraSam):
    m_hRootKey(hRootKey),
    m_subKey(subKey),
    m_hKey(0),
    m_readOnly(readOnly),
    m_extraSam(extraSam)
{
}


WinRegistryKey::~WinRegistryKey()
{
    close();
}


bool WinRegistryKey::setString( const std::string& name, const std::string& value )
{
    if (!open()) 
    {
        return false;
    }

    if (RegSetValueExA(m_hKey, name.c_str(), 0, REG_SZ, (CONST BYTE*) value.c_str(), (DWORD) value.size() + 1) != ERROR_SUCCESS)
    {
        return false;
    }

    return true;
}


std::string WinRegistryKey::getString(const std::string& name)
{
    if (!open()) 
    {
        return std::string();
    }

    DWORD type;
    DWORD size;

    if (RegQueryValueExA(m_hKey, name.c_str(), nullptr, &type, nullptr, &size) != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ && type != REG_LINK))
    {
        return std::string();
    }

    if (size > 0)
    {
        Buffer<char> buffer(size + 1);
        size += 1;
        RegQueryValueExA(m_hKey, name.c_str(), nullptr, nullptr, (BYTE*) buffer.begin(), &size);
        buffer[size] = 0;
        std::string result(buffer.begin());
        return result;
    }

    return std::string();
}


bool WinRegistryKey::setStringExpand( const std::string& name, const std::string& value )
{
    if (!open()) 
    {
        return false;
    }

    if (RegSetValueExA(m_hKey, name.c_str(), 0, REG_EXPAND_SZ, (CONST BYTE*) value.c_str(), (DWORD) value.size() + 1) != ERROR_SUCCESS)
    {
        return false;
    }

    return true;
}


bool WinRegistryKey::getStringExpand(const std::string& name, std::string& value)
{
    if (!open()) 
    {
        return false;
    }

    DWORD type;
    DWORD size;

    if (RegQueryValueExA(m_hKey, name.c_str(), nullptr, &type, nullptr, &size) != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ && type != REG_LINK))
    {
        return false;
    }

    if (size > 0)
    {
        Buffer<char> buffer(size + 1);
        RegQueryValueExA(m_hKey, name.c_str(), nullptr, nullptr, (BYTE*) buffer.begin(), &size);
        buffer[size] = 0;
        char temp;
        DWORD expSize = ExpandEnvironmentStringsA(buffer, &temp, 1);
        Buffer<char> expBuffer(expSize);
        ExpandEnvironmentStringsA(buffer.begin(), expBuffer.begin(), expSize);
        std::string result(expBuffer.begin());

        value = result;
    }

    return true;
}



bool WinRegistryKey::setBinary( const std::string& name, const std::vector<char>& value )
{
    if (!open()) 
    {
        return false;
    }

    if (RegSetValueExA(m_hKey,  name.c_str(), 0, REG_BINARY, (CONST BYTE*) &value[0], (DWORD) value.size()) != ERROR_SUCCESS)
    {
        return false;
    }

    return true;
}


std::vector<char> WinRegistryKey::getBinary( const std::string& name )
{
    if (!open()) 
    {
        return std::vector<char>();
    }

    DWORD type;
    DWORD size;
    std::vector<char> result;

    if (RegQueryValueExA(m_hKey, name.c_str(), nullptr, &type, nullptr, &size) != ERROR_SUCCESS || type != REG_BINARY)
    {
        return std::vector<char>();
    }

    if (size > 0)
    {
        result.resize(size);
        RegQueryValueExA(m_hKey, name.c_str(), nullptr, nullptr, (BYTE*) &result[0], &size);
    }

    return result;
}


bool WinRegistryKey::setInt( const std::string& name, int value )
{
    if (!open()) 
    {
        return false;
    }

    DWORD data = value;

    if (RegSetValueExA(m_hKey, name.c_str(), 0, REG_DWORD, (CONST BYTE*) &data, sizeof(data)) != ERROR_SUCCESS)
    {
        return false;
    }

    return true;
}


bool WinRegistryKey::getInt( const std::string& name, int& value )
{
    if (!open()) 
    {
        return false;
    }

    DWORD type;
    DWORD data;
    DWORD size = sizeof(data);

    if (RegQueryValueExA(m_hKey, name.c_str(), nullptr, &type, (BYTE*) &data, &size) != ERROR_SUCCESS || (type != REG_DWORD && type != REG_DWORD_BIG_ENDIAN))
    {
        return false;
    }

    value = data;

    return true;
}


bool WinRegistryKey::setInt64( const std::string& name, Int64 value )
{
    if (!open()) 
    {
        return false;
    }

    if (RegSetValueExA(m_hKey, name.c_str(), 0, REG_QWORD, (CONST BYTE*) &value, sizeof(value)) != ERROR_SUCCESS)
    {
        return false;
    }

    return true;
}

bool WinRegistryKey::getInt64( const std::string& name, WinRegistryKey::Int64& value )
{
    if (!open()) 
    {
        return false;
    }

    DWORD type;
    WinRegistryKey::Int64 data;
    DWORD size = sizeof(data);

    if (RegQueryValueExA(m_hKey, name.c_str(), nullptr, &type, (BYTE*) &data, &size) != ERROR_SUCCESS || type != REG_QWORD)
    {
        return false;
    }

    value = data;

    return true;
}


bool WinRegistryKey::deleteValue( const std::string& name )
{
    if (!open()) return false;

    if (RegDeleteValueA(m_hKey, name.c_str()) != ERROR_SUCCESS)
    {
        return false;
    }

    return true;
}


bool WinRegistryKey::deleteKey()
{
    Keys keys;
    subKeys(keys);
    close();
    
    for (Keys::iterator it = keys.begin(); it != keys.end(); ++it)
    {
        std::string subKey(m_subKey);
        subKey += "\\";
        subKey += *it;
        WinRegistryKey subRegKey(m_hRootKey, subKey);
        subRegKey.deleteKey();
    }

    // NOTE: RegDeleteKeyEx is only available on Windows XP 64-bit SP3, Windows Vista or later.
    // We cannot call it directly as this would prevent the code running on Windows XP 32-bit.
    // Therefore, if we need to call RegDeleteKeyEx (m_extraSam != 0) we need to check for
    // its existence in ADVAPI32.DLL and call it indirectly.
    typedef LONG (WINAPI *RegDeleteKeyExAFunc)(HKEY hKey, const char* lpSubKey, REGSAM samDesired, DWORD Reserved);
    
    if (m_extraSam != 0)
    {
        AutoHandle advAPI32(LoadLibraryA("ADVAPI32.DLL"));
        if (advAPI32.handle())
        {
            RegDeleteKeyExAFunc pRegDeleteKeyExA = reinterpret_cast<RegDeleteKeyExAFunc>(GetProcAddress(advAPI32.handle() , "RegDeleteKeyExA"));
        
            if (pRegDeleteKeyExA)
            {
                if ((*pRegDeleteKeyExA)(m_hRootKey, m_subKey.c_str(), m_extraSam, 0) != ERROR_SUCCESS)
                {
                    return false;
                }

                return true;
            }
        }
    }

    if (RegDeleteKeyA(m_hRootKey, m_subKey.c_str()) != ERROR_SUCCESS)
    {
        return false;
    }

    return true;
}


bool WinRegistryKey::exists()
{
    HKEY hKey;    

    if (RegOpenKeyExA(m_hRootKey, m_subKey.c_str(), 0, KEY_READ | m_extraSam, &hKey) == ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return true;
    }

    return false;
}


WinRegistryKey::Type WinRegistryKey::type(const std::string& name)
{
    open();
    DWORD type = REG_NONE;
    DWORD size;

    if (RegQueryValueExA(m_hKey, name.c_str(), nullptr, &type, nullptr, &size) != ERROR_SUCCESS)
    {
        return REGT_ERROR;
    }

    Type aType = (Type)type;
    return aType;
}


bool WinRegistryKey::exists(const std::string& name)
{
    bool exists = false;
    HKEY hKey;

    if (RegOpenKeyExA(m_hRootKey, m_subKey.c_str(), 0, KEY_READ | m_extraSam, &hKey) == ERROR_SUCCESS)
    {
        exists = RegQueryValueExA(hKey, name.c_str(), nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS;
        RegCloseKey(hKey);
    }

    return exists;
}


bool WinRegistryKey::open()
{
    if (!m_hKey)
    {
        if (m_readOnly)
        {
            if (RegOpenKeyExA(m_hRootKey, m_subKey.c_str(), 0, KEY_READ | m_extraSam, &m_hKey) != ERROR_SUCCESS)
            {
                return false;
            }
        }
        else
        {
            if (RegCreateKeyExA(m_hRootKey, m_subKey.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE | m_extraSam, nullptr, &m_hKey, nullptr) != ERROR_SUCCESS)
            {
                return false;
            }
        }
    }

    return true;
}


void WinRegistryKey::close()
{
    if (m_hKey)
    {
        RegCloseKey(m_hKey);
        m_hKey = 0;
    }
}


std::string WinRegistryKey::key() const
{
    std::string result;

    if (m_hRootKey == HKEY_CLASSES_ROOT)
    {
        result = "HKEY_CLASSES_ROOT";
    }

#if defined(HKEY_CURRENT_CONFIG)
    else if (m_hRootKey == HKEY_CURRENT_CONFIG)
    {
        result = "HKEY_CURRENT_CONFIG";
    }

#endif
    else if (m_hRootKey == HKEY_CURRENT_USER)
    {
        result = "HKEY_CURRENT_USER";
    }
    else if (m_hRootKey == HKEY_LOCAL_MACHINE)
    {
        result = "HKEY_LOCAL_MACHINE";
    }
    else if (m_hRootKey == HKEY_USERS)
    {
        result = "HKEY_USERS";
    }

#if defined(HKEY_PERFORMANCE_DATA)
    else if (m_hRootKey == HKEY_PERFORMANCE_DATA)
    {
        result = "HKEY_PERFORMANCE_DATA";
    }

#endif
    else
    {
        result = "(UNKNOWN)";
    }

    result += '\\';
    result += m_subKey;
    return result;
}


std::string WinRegistryKey::key(const std::string& valueName) const
{
    std::string result = key();

    if (!valueName.empty())
    {
        result += '\\';
        result += valueName;
    }

    return result;
}


HKEY WinRegistryKey::handle()
{
    open();
    return m_hKey;
}


HKEY WinRegistryKey::handleFor(const std::string& rootKey)
{
    if (rootKey == std::string("HKEY_CLASSES_ROOT"))
    {
        return HKEY_CLASSES_ROOT;
    }

#if defined(HKEY_CURRENT_CONFIG)
    else if (rootKey == "HKEY_CURRENT_CONFIG")
    {
        return HKEY_CURRENT_CONFIG;
    }

#endif
    else if (rootKey == "HKEY_CURRENT_USER")
    {
        return HKEY_CURRENT_USER;
    }

    else if (rootKey == "HKEY_LOCAL_MACHINE")
    {
        return HKEY_LOCAL_MACHINE;
    }

    else if (rootKey == "HKEY_USERS")
    {
        return HKEY_USERS;
    }

#if defined(HKEY_PERFORMANCE_DATA)
    else if (rootKey == "HKEY_PERFORMANCE_DATA")
    {
        return HKEY_PERFORMANCE_DATA;
    }

#endif
    else
    {
        return 0;
    }
}


void WinRegistryKey::subKeys(WinRegistryKey::Keys& keys)
{
    open();
    DWORD subKeyCount = 0;
    DWORD valueCount = 0;

    if (RegQueryInfoKey(m_hKey, nullptr, nullptr, nullptr, &subKeyCount, nullptr, nullptr, &valueCount, nullptr, nullptr, nullptr, nullptr) != ERROR_SUCCESS)
    {
        return;
    }

    char buf[256];
    DWORD bufSize = sizeof(buf);

    for (DWORD i = 0; i< subKeyCount; ++i)
    {
        if (RegEnumKeyExA(m_hKey, i, buf, &bufSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
        {
            std::string name(buf);
            keys.push_back(name);
        }

        bufSize = sizeof(buf);
    }
}


void WinRegistryKey::values(WinRegistryKey::Values& vals)
{
    open();
    DWORD valueCount = 0;

    if (RegQueryInfoKey(m_hKey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &valueCount, nullptr, nullptr, nullptr, nullptr) != ERROR_SUCCESS)
    {
        return;
    }

    char buf[256];
    DWORD bufSize = sizeof(buf);

    for (DWORD i = 0; i< valueCount; ++i)
    {
        if (RegEnumValueA(m_hKey, i, buf, &bufSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
        {
            std::string name(buf);
            vals.push_back(name);
        }

        bufSize = sizeof(buf);
    }
}

