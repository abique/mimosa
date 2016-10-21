#include "url.hh"

namespace mimosa
{
namespace uri
{
Url::Url()
    : raw_url_(),
      scheme_(kSchemeUnknown),
      host_(),
      port_(80),
      path_(),
      query_(),
      anchor_()
{
}

Url::Url(const std::string & url)
{
    parse(url, nullptr);
}

Url::Url(const Url &url)
{
    *this = url;
}

Url &Url::operator=(const Url &url)
{
    parse(url.rawUrl(), nullptr);
    return *this;
}

bool
Url::parse(const std::string & url, std::string *parse_error)
{
    raw_url_ = url;

    StringRef in(raw_url_);

    if (!parseScheme(in, parse_error))
        return false;
    if (!parseAuthority(in, parse_error))
        return false;

    parsePath(in);
    if (!in.empty() && in[0] == '?') {
        in = in.substr(1);
        parseQuery(in);
    }
    if (!in.empty() && in[0] == '#') {
        in = in.substr(1);
        parseAnchor(in);
    }

    assert(raw_url_.data() <= host_.data());
    assert(raw_url_.data() + raw_url_.size() >= host_.data() + host_.size());

    return true;
}

#define PARSE_ERROR(Str)			\
    do {					\
    if (parse_error)				\
    *parse_error = Str;			\
    return false;				\
} while (0)

bool
Url::parseScheme(StringRef & in, std::string *parse_error)
{
    if (in.size() < 8)
        PARSE_ERROR("url too short");

    if (in.strncaseeq("http://")) {
        scheme_ = kSchemeHttp;
        port_ = 80;
        in = in.substr(7);
    } else if (in.strncaseeq("https://")) {
        scheme_ = kSchemeHttps;
        port_ = 443;
        in = in.substr(8);
    } else
        PARSE_ERROR("unknown scheme");
    return true;
}

bool
Url::parseAuthority(StringRef & in, std::string *parse_error)
{
    if (!parseUserInfo(in, parse_error))
        return false;
    if (!parseHost(in, parse_error))
        return false;

    if (!in.empty() && in[0] == ':') {
        in = in.substr(1);
        if (!parsePort(in, parse_error))
            return false;
    }
    return true;
}

bool
Url::parseUserInfo(StringRef & in, std::string *parse_error)
{
    // XXX
    return true;
}

bool
Url::parseHost(StringRef & in, std::string *parse_error)
{
    auto it = in.begin();
    for (; it != in.end(); ++it)
        if (*it == ':' || *it == '/')
            break;
    auto len = it - in.begin();
    host_ = in.substr(0, len);
    in = in.substr(len);
    return !host_.empty();
}

bool
Url::parsePort(StringRef & in, std::string *parse_error)
{
    uint16_t port = 0;
    port_ = 0;
    auto it = in.begin();
    for (; it != in.end(); ++it) {
        if (*it < '0' || '9' < *it)
            break;
        port = port * 10 + *it - '0';
    }

    if (it != in.end() && *it != '/')
        PARSE_ERROR("expected / after port");
    if (it != in.begin())
        port_ = port;

    in = in.substr(it - in.begin());
    return true;
}

void
Url::parsePath(StringRef & in)
{
    auto it = in.begin();
    for (; it != in.end(); ++it)
        if (*it == '?' || *it == '#')
            break;
    auto len = it - in.begin();
    if (len > 0) {
        path_ = in.substr(0, len);
        in = in.substr(len);
    } else
        path_ = "/";
}

void
Url::parseQuery(StringRef & in)
{
    auto it = in.begin();
    for (; it != in.end(); ++it)
        if (*it == '#')
            break;
    auto len = it - in.begin();
    query_ = in.substr(0, len);
    in = in.substr(len);
}

void
Url::parseAnchor(StringRef & in)
{
    anchor_ = in;
    in.clear();
}
}
}
