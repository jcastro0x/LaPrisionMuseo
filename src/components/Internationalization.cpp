// Copyright (c) 2022 Javier Castro - jcastro0x@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "Internationalization.hpp"

#include <fstream>

using namespace lpm;

Internationalization::Internationalization()
{
    std::ifstream f("i18n.json");
    json_ = nlohmann::json::parse(f);
}

sf::String Internationalization::getString(std::string ns, std::string key) const
{
    for(auto& entry : json_)
    {
        if(entry["language"] == currentLanguageName_)
        {
            for(auto& entry_ns : entry["namespaces"])
            {
                if(entry_ns["name"] == ns)
                {
                    for(auto& content : entry_ns["content"])
                    {
                        if(content["key"] == key)
                        {
                            return content["value"];
                        }
                    }
                }
            }
        }
    }

    return ns + "_" + key;
}
