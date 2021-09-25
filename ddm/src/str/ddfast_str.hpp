
#ifndef ddfast_str_basic_hpp
#define ddfast_str_basic_hpp

#include "g_def.h"
#include "ddassert.h"

BEG_NSP_DDM
template <class t>
class ddfast_str_basic {
public:
    using t_str = std::basic_string<t>;
    using t_size = typename t_str::size_type;
    static constexpr auto xnpos = t_str::npos;

public:
    ddfast_str_basic(const t* buff);
    ddfast_str_basic(const t_str& str);
    ddfast_str_basic(const t* buff, t_size len);
    ddfast_str_basic(const t* buff, t_size l, t_size r);

    inline const t* data() const;
    inline t_size length() const;
    inline t operator[](t_size index) const;
    inline bool operator==(const ddfast_str_basic& r) const;

public:
    inline t_str to_str() const;
    inline const t* c_str() const;

    void trim();

    bool beg_with(const ddfast_str_basic& cmp) const;
    bool end_with(const ddfast_str_basic& cmp) const;

    ddfast_str_basic sub_str(t_size l, t_size len = xnpos) const;
    void to_sub_str(t_size l, t_size len = xnpos);

    t_size find(t c) const;
    t_size find(const ddfast_str_basic& cmp) const;
    t_size find_last_of(t c) const;
    t_size find_last_of(const ddfast_str_basic& cmp) const;

    void split(const ddfast_str_basic& cmp, std::vector<ddfast_str_basic>& out) const;

private:
    const t* m_buff;
    t_size m_l_pos = 0; // °üº¬
    t_size m_r_pos = 0; // ²»°üº¬
};

template <class t>
ddfast_str_basic<t>::ddfast_str_basic(const t* buff) : m_buff(buff), m_l_pos(0)
{
    DD_ASSERT(buff != nullptr);
    t_str tmp(m_buff);
    m_r_pos = tmp.length();
}

template <class t>
ddfast_str_basic<t>::ddfast_str_basic(const t_str& str) :
    m_buff(str.c_str()), m_l_pos(0), m_r_pos(str.length()) { }

template <class t>
ddfast_str_basic<t>::ddfast_str_basic(const t* buff, t_size len) :
    m_buff(buff), m_l_pos(0), m_r_pos(len) 
{
    DD_ASSERT(buff != nullptr);
}

template <class t>
ddfast_str_basic<t>::ddfast_str_basic(const t* buff, t_size l, t_size r) :
    m_buff(buff), m_l_pos(l), m_r_pos(r)
{
    DD_ASSERT(buff != nullptr);
    DD_ASSERT(l <= r);
}

template <class t>
inline const t* ddfast_str_basic<t>::data() const
{
    return m_buff;
}

template <class t>
inline typename ddfast_str_basic<t>::t_size ddfast_str_basic<t>::length() const
{
    return m_r_pos - m_l_pos;
}

template <class t>
inline t ddfast_str_basic<t>::operator[](t_size index) const
{
    DD_ASSERT(index < length());
    return m_buff[m_l_pos + index];
}

template <class t>
inline bool ddfast_str_basic<t>::operator==(const ddfast_str_basic& r) const
{
    if (length() != r.length()) {
        return false;
    }

    for (t_size i = 0; i < length(); ++i) {
        if ((*this)[i] != r[i]) {
            return false;
        }
    }

    return true;
}

template <class t>
inline typename ddfast_str_basic<t>::t_str ddfast_str_basic<t>::to_str() const
{
    return t_str(m_buff + m_l_pos, m_buff + m_r_pos);
}

template <class t>
inline const t* ddfast_str_basic<t>::c_str() const
{
    return m_buff + m_l_pos;
}

template <class t>
void ddfast_str_basic<t>::trim()
{
    for (t_size i = m_l_pos; i < m_r_pos; ++i) {
        if (m_buff[i] != ' ') {
            break;
        }

        ++m_l_pos;
    }

    for (t_size i = m_r_pos; i > m_l_pos + 1; --i) {
        if (m_buff[i - 1] != ' ') {
            break;
        }
        --m_r_pos;
    }
}

template <class t>
bool ddfast_str_basic<t>::beg_with(const ddfast_str_basic& cmp) const
{
    if (length() < cmp.length()) {
        return false;
    }

    for (t_size i = 0; i < cmp.length(); ++i) {
        if ((*this)[i] != cmp[i]) {
            return false;
        }
    }

    return true;
}

template <class t>
bool ddfast_str_basic<t>::end_with(const ddfast_str_basic& cmp) const
{
    if (length() < cmp.length()) {
        return false;
    }

    for (t_size i = 0; i < cmp.length(); ++i) {
        if ((*this)[length() - cmp.length() + i] != cmp[i]) {
            return false;
        }
    }

    return true;
}

template <class t>
ddfast_str_basic<t> ddfast_str_basic<t>::sub_str(t_size l, t_size len/* = xnpos*/) const
{
    DD_ASSERT(l <= length());
    if (len == xnpos) {
        return ddfast_str_basic(m_buff, m_l_pos + l, m_r_pos);
    } else {
        DD_ASSERT(l + len <= m_r_pos);
        return ddfast_str_basic(m_buff, m_l_pos + l, m_l_pos + l + len);
    }
}

template <class t>
void ddfast_str_basic<t>::to_sub_str(ddfast_str_basic<t>::t_size l, ddfast_str_basic<t>::t_size len/* = xnpos*/)
{
    DD_ASSERT(l <= length());
    m_l_pos += l;
    if (len != xnpos) {
        DD_ASSERT(l + len <= m_r_pos);
        m_r_pos = m_l_pos + len;
    }
}

template <class t>
typename ddfast_str_basic<t>::t_size ddfast_str_basic<t>::find(t c) const
{
    for (t_size i = 0; i < length(); ++i) {
        if ((*this)[i] == c) {
            return i;
        }
    }

    return xnpos;
}

template <class t>
typename ddfast_str_basic<t>::t_size ddfast_str_basic<t>::find(const ddfast_str_basic& cmp) const
{
    if (cmp.length() == 0) {
        return 0;
    }

    if (cmp.length() > length()) {
        return xnpos;
    }

    for (t_size i = 0; i < length(); ++i) {
        if ((*this)[i] != cmp[0]) {
            continue;
        }

        if (sub_str(i).beg_with(cmp)) {
            return i;
        }
    }

    return xnpos;
}

template <class t>
typename ddfast_str_basic<t>::t_size ddfast_str_basic<t>::find_last_of(t c) const
{
    for (t_size i = length(); i > 0; --i) {
        if ((*this)[i - 1] == c) {
            return i - 1;
        }
    }

    return xnpos;
}

template <class t>
typename ddfast_str_basic<t>::t_size ddfast_str_basic<t>::find_last_of(const ddfast_str_basic& cmp) const
{
    if (cmp.length() == 0) {
        return 0;
    }

    if (cmp.length() > length()) {
        return xnpos;
    }

    for (t_size i = length() - cmp.length() + 1; i > 0; --i) {
        if ((*this)[i - 1] != cmp[0]) {
            continue;
        }

        if (sub_str(i - 1).beg_with(cmp)) {
            return i - 1;
        }
    }

    return xnpos;
}

template <class t>
void ddfast_str_basic<t>::split(const ddfast_str_basic& cmp, std::vector<ddfast_str_basic>& out) const
{
    DD_ASSERT(cmp.length() > 0);
    ddfast_str_basic src(*this);
    t_size pos = src.find(cmp);
    while (pos != xnpos)
    {
        out.emplace_back(src.sub_str(0, pos));
        src.to_sub_str(pos + cmp.length());
        pos = src.find(cmp);
    }

    out.emplace_back(src);
}

using ddfast_str = ddfast_str_basic<char>;
using ddfast_strw = ddfast_str_basic<wchar_t>;

END_NSP_DDM
#endif
