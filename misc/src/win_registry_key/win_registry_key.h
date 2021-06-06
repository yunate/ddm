//
// WinRegistryKey.h
//
// 从POCO的Util库中的WinRegistryKey修改而来
// 能够完成注册表的基本操作
//

#ifndef __WinRegistryKey_H_
#define __WinRegistryKey_H_

#include <Windows.h>
#include <string>
#include <vector>


// This class implements a convenient interface to the
// Windows Registry.
// This class is only available on Windows platforms.
class WinRegistryKey
{
public:
    typedef std::vector<std::string> Keys;
    typedef std::vector<std::string> Values;

    typedef __int64 Int64;

    enum Type
    {
        REGT_NONE = 0,
        REGT_STRING = 1,
        REGT_STRING_EXPAND = 2,
        REGT_BINARY = 3,
        REGT_DWORD = 4,
        REGT_DWORD_BIG_ENDIAN = 5,
        REGT_LINK = 6,
        REGT_MULTI_STRING = 7,
        REGT_RESOURCE_LIST = 8,
        REGT_FULL_RESOURCE_DESCRIPTOR = 9,
        REGT_RESOURCE_REQUIREMENTS_LIST = 10,
        REGT_QWORD = 11,
        REGT_ERROR = 12
    };

    // Creates the WinRegistryKey.
    //
    // The key must start with one of the root key names
    // like HKEY_CLASSES_ROOT, e.g. HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services.
    //
    // If readOnly is true, then only read access to the registry
    // is available and any attempt to write to the registry will
    // result in an exception.
    //
    // extraSam is used to pass extra flags (in addition to KEY_READ and KEY_WRITE)
    // to the samDesired argument of RegOpenKeyEx() or RegCreateKeyEx().
    WinRegistryKey(const std::string& key, bool readOnly = false, REGSAM extraSam = 0);
        
    // Creates the WinRegistryKey.
    //
    // If readOnly is true, then only read access to the registry
    // is available and any attempt to write to the registry will
    // result in an exception.
    //
    // extraSam is used to pass extra flags (in addition to KEY_READ and KEY_WRITE)
    // to the samDesired argument of RegOpenKeyEx() or RegCreateKeyEx().
    WinRegistryKey(HKEY hRootKey, const std::string& subKey, bool readOnly = false, REGSAM extraSam = 0);

    // Destroys the WinRegistryKey.
    ~WinRegistryKey();
        
    // Sets the string value (REG_SZ) with the given name.
    // An empty name denotes the default value.
    bool setString(const std::string& name, const std::string& value);
        
    // Returns the string value (REG_SZ) with the given name.
    // An empty name denotes the default value.
    std::string getString(const std::string& name);
        
    // Sets the expandable string value (REG_EXPAND_SZ) with the given name.
    // An empty name denotes the default value.
    bool setStringExpand(const std::string& name, const std::string& value);
    
    // Returns the string value (REG_EXPAND_SZ) with the given name.
    // An empty name denotes the default value.
    // All references to environment variables (%VAR%) in the string
    // are expanded.
    bool getStringExpand(const std::string& name, std::string& value);
        
    // Sets the string value (REG_BINARY) with the given name.
    // An empty name denotes the default value.
    bool setBinary(const std::string& name, const std::vector<char>& value);
        
    // Returns the string value (REG_BINARY) with the given name.
    // An empty name denotes the default value.
    std::vector<char> getBinary(const std::string& name);
        
    // Sets the numeric (REG_DWORD) value with the given name.
    // An empty name denotes the default value.
    bool setInt(const std::string& name, int value);
        
    // Returns the numeric value (REG_DWORD) with the given name.
    // An empty name denotes the default value.
    bool getInt(const std::string& name, int& value);

    // Sets the numeric (REG_QWORD) value with the given name.
    // An empty name denotes the default value.
    bool setInt64(const std::string& name, Int64 value);
        
    // Returns the numeric value (REG_QWORD) with the given name.
    // An empty name denotes the default value.
    bool getInt64(const std::string& name, WinRegistryKey::Int64& value);
        
    // Deletes the value with the given name.
    bool deleteValue(const std::string& name);
        

    // Recursively deletes the key and all subkeys.
    bool deleteKey();

    // Returns true iff the key exists.
    bool exists();

    // Returns the type of the key value.
    Type type(const std::string& name);

    // Returns true iff the given value exists under that key.
    bool exists(const std::string& name);

    // Appends all subKey names to keys.
    void subKeys(Keys& keys);

    // Appends all value names to vals;
    void values(Values& vals);

    // Returns true iff the key has been opened for read-only access only.
    inline bool isReadOnly() const
    {
        return m_readOnly;
    };

protected:
    bool open();
    void close();
    std::string key() const;
    std::string key(const std::string& valueName) const;
    HKEY handle();

    static HKEY handleFor(const std::string& rootKey);

private:
    WinRegistryKey()=delete;
    WinRegistryKey(const WinRegistryKey&)=delete;
    WinRegistryKey& operator = (const WinRegistryKey&)=delete;

    HKEY        m_hRootKey;
    std::string m_subKey;
    HKEY        m_hKey;
    bool        m_readOnly;
    REGSAM      m_extraSam;
};

#endif // __WinRegistryKey_H_
