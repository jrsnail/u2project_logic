#include "U2TrackedObjs.h"

#include "U2StringUtil.h"


U2EG_NAMESPACE_USING


#if U2_COMPILER == U2_COMPILER_MSVC
// MSDN says to #include <intrin.h>, but that breaks the VS2005 build.
extern "C" 
{
    void* _ReturnAddress();
}
#endif
//---------------------------------------------------------------------
//---------------------------------------------------------------------
Location::Location(const char* function_name,
    const char* file_name,
    int line_number,
    const void* program_counter)
    : function_name_(function_name)
    , file_name_(file_name)
    , line_number_(line_number)
    , program_counter_(program_counter) 
{
}
//---------------------------------------------------------------------
Location::Location()
    : function_name_("Unknown")
    , file_name_("Unknown")
    , line_number_(-1)
    , program_counter_(nullptr) 
{
}
//---------------------------------------------------------------------
String Location::ToString() const 
{
    StringStream ss;
    ss << function_name_ << "@" << file_name_ << ":" << line_number_;
    return ss.str();
}
//---------------------------------------------------------------------
String Location::WriteAsHtml(bool display_filename, bool display_function_name)
{
    StringStream ss;
    ss << display_filename ? file_name_ : "line";
    ss << line_number_;

    if (display_function_name)
    {
        ss << WriteFunctionNameAsHtml();
        ss << ' ';
    }

    return ss.str();
}
//---------------------------------------------------------------------
String Location::WriteFunctionNameAsHtml()
{
    // Translate "<" to "&lt;" for HTML safety.
    // TODO(jar): Support ASCII or html for logging in ASCII.
    StringStream ss;
    for (const char *p = function_name_; *p; p++) 
    {
        switch (*p) 
        {
        case '<':
            ss << "&lt;";
            break;

        case '>':
            ss << "&gt;";
            break;

        default:
            ss << *p;
            break;
        }
    }
    return ss.str();
}
//---------------------------------------------------------------------
const Location::InfoMap& Location::ToValue()
{
    m_LocationInfo["file_name"] = file_name_;
    // Note: This function name is not escaped, and templates have less than
    // characters, which means this is not suitable for display as HTML unless
    // properly escaped.
    m_LocationInfo["function_name"] = function_name_;
    m_LocationInfo["line_number"] = StringUtil::toString(line_number_);
    return m_LocationInfo;
}
//---------------------------------------------------------------------
// #if U2_COMPILER == U2_COMPILER_MSVC
// __declspec(noinline)
// #endif
//---------------------------------------------------------------------
_U2Export const void* GetProgramCounter() 
{
#if U2_COMPILER == U2_COMPILER_MSVC
    return _ReturnAddress();
#elif U2_COMPILER == U2_COMPILER_GCCE
    return __builtin_extract_return_addr(__builtin_return_address(0));
#endif  // COMPILER_GCC

    return nullptr;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
BirthOnThread::BirthOnThread(const Location& location, const ThreadData& current)
    : location_(location)
    , birth_thread_(&current) 
{
}
//---------------------------------------------------------------------
const Location BirthOnThread::location() const
{ 
    return location_; 
}
//---------------------------------------------------------------------
const ThreadData* BirthOnThread::birth_thread() const 
{ 
    return birth_thread_; 
}
//---------------------------------------------------------------------
/*
void BirthOnThread::ToValue(const String& prefix, InfoMap* dictionary) const
{
    m_InfoMap[] = 
    dictionary->Set(prefix + "_location", location_.ToValue());
    dictionary->Set(prefix + "_thread",
        base::Value::CreateStringValue(birth_thread_->thread_name()));
}
*/
//---------------------------------------------------------------------
//---------------------------------------------------------------------
Births::Births(const Location& location, const ThreadData& current)
    : BirthOnThread(location, current)
    , birth_count_(1) 
{ 
}
//---------------------------------------------------------------------
int Births::birth_count() const 
{ 
    return birth_count_; 
}
//---------------------------------------------------------------------
void Births::RecordBirth() 
{ 
    ++birth_count_; 
}
//---------------------------------------------------------------------
void Births::ForgetBirth() 
{ 
    --birth_count_; 
}
//---------------------------------------------------------------------
void Births::Clear()
{ 
    birth_count_ = 0; 
}