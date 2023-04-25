#pragma once

#include "Type.h"
#include "tinyxml.h"

using XMLElement = TiXmlElement;
class XMLDocument : public TiXmlDocument
{
public:
    template <typename Callable>
    void each(Callable callback)
    {
        XMLElement *root = this->RootElement();
        for (XMLElement *element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
        {
            callback(element);
        }
    }
};

class XMLHelper
{
public:
    // Init document
    static XMLDocument *load(String filePath)
    {
        XMLDocument *document = new XMLDocument();

        if (document->LoadFile(filePath.toArray()) == false)
        {
            return NULL;
        }

        return document;
    }

    // Itergrate through each element
    template <typename Callable>
    static bool forEach(String filePath, Callable callback)
    {
        // Load document
        XMLDocument *document = XMLHelper::load(filePath);
        if ((document) == NULL)
        {
            return false;
        }

        // Apply callback for each element
        document->each(callback);

        return true;
    }
};