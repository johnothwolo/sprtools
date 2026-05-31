//
//  svg.cpp
//  sprtools
//
//  BSD 3-Clause License
//
//  Copyright (c) 2026, John Othwolo
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this
//     list of conditions and the following disclaimer.

//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created by John Othwolo on 5/28/26.
//


#include "spr.h"
#include "SprObject.h"
#include "SprAttribute.h"
#include "SprFileAttribute.h"

#include <sstream>
#include <iostream>
#include <cmath>

typedef long SvgId;

static SvgId objId = 0;
static SvgId getNewObjectId(void)
{
    return objId++;
}

// we can convert ARGB to rgba(r,g,b,a)
static std::string createSvgColorStr(SprColor::ARGB &argb, float &opacity)
{
    char buffer[128] = {0};
    snprintf(buffer, sizeof(buffer), "rgb(%d,%d,%d)", argb.red, argb.green, argb.blue);
    opacity = argb.alpha;
    return std::string(buffer);
}

static std::string createSvgColorStr(SprColor::RGBA &rgba)
{
    char buffer[128] = {0};
    snprintf(buffer, sizeof(buffer), "#%X%X%X%X", rgba.red, rgba.green, rgba.blue, rgba.alpha);
    return std::string(buffer);
}

static SvgId createGradient(SprColoredAttribute *color, std::stringstream &stream, std::stringstream &artags)
{
    SvgId id = getNewObjectId();
    
    if (color->colorType == SprAttributeFill::ColorType::LinearGradient)
    {
        artags << R"(<linearGradient id=grad")" << id << R"(")";
        artags << R"( x1=")" << color->gradient.record.linear.x1 << R"( x2=")" << color->gradient.record.linear.x2 << R"(")";
        artags << R"( y1=")" << color->gradient.record.linear.y1 << R"( y2=")" << color->gradient.record.linear.y2 << R"(">)" << std::endl;;
        for(int i = 0; i < color->gradient.colors.size(); i++)
            artags << R"(<stop offset=")" << color->gradient.positions[i] << R"(" stop-color="#)" << createSvgColorStr(color->gradient.colors[i]) << R"("/stop>)" << std::endl;
        artags << R"(</linearGradient>)" << std::endl;
    }
    else if (color->colorType == SprAttributeFill::ColorType::RadialGradient)
    {
        artags << R"(<radialGradient id=grad")" << id << R"(")";
        artags << R"( cr=")" << color->gradient.record.radial.r << R"(")";
        artags << R"( cx=")" << color->gradient.record.radial.cx << R"(")";
        artags << R"( cy=")" << color->gradient.record.radial.cy << std::endl;
        for(int i = 0; i < color->gradient.colors.size(); i++)
            artags << R"(<stop offset=")" << color->gradient.positions[i] << R"(" stop-color="#)" << createSvgColorStr(color->gradient.colors[i]) << R"("/stop>)" << std::endl;
        artags << R"(</radialGradient>)" << std::endl;
    }
    return id;
}

static void addColorAttribute(SprColoredAttribute *color, std::stringstream &stream, std::stringstream &artags)
{
    float opacity = 255.0f;
    const char *attrname;
    
    // stroke or fill, depending on color-attribute type.
    if (color->type == SprColoredAttribute::Type::Fill)
        attrname = " fill";
    if (color->type == SprColoredAttribute::Type::Stroke)
        attrname = " stroke";
    
    stream << attrname << "=";
    
    if (color->colorType == SprAttributeFill::ColorType::ARGB)
        stream << "\"" << createSvgColorStr(color->color, opacity) << "\" ";
    else if (color->colorType == SprAttributeFill::ColorType::LinearGradient || color->colorType == SprAttributeFill::ColorType::RadialGradient)
        stream << "\"url(#grad" << createGradient(color, stream, artags) << ")\" " ;
    else if (color->colorType == SprAttributeFill::ColorType::Link)
        throw "not yet implemented"; // FIXME: is this support to be a swatch?
    
    if (opacity != 255.0f)
        stream << attrname << "-opacity=\"" << opacity << "\" ";
}

static void addSvgObjectAttibutes(std::vector<std::unique_ptr<SprAttribute>> &attrs, std::stringstream &stream, std::stringstream &artags)
{
    for (auto &attr : attrs) {
        switch (attr->type) {
            case SprAttribute::Type::Clip:
                stream << "";
                break;
            case SprAttribute::Type::ClipPath: {
                // This is a ref-tag attribute.
                // Add its definition, then add the attribte (which points to the definition)
                long id = getNewObjectId();
                artags << "<clipPath id=\"url(#grad" << id << ")\">" << std::endl;
//                artags << R"(<rect x=")" << ((SprAttributeClipPath*)attr.get())->record;
                throw "idk where this link points to";
                artags << "</clipPath>" << std::endl;
                
                stream << R"( clip-path="clip)" << id << R"(" )";
                break;
            }
            case SprAttribute::Type::Fill:
                addColorAttribute((SprAttributeFill*) attr.get(), stream, artags);
                break;
            case SprAttribute::Type::Stroke:
                addColorAttribute((SprAttributeStroke*) attr.get(), stream, artags);
                break;
            case SprAttribute::Type::Matrix: {
                SprMatrix &matrix = ((SprAttributeMatrix*) attr.get())->record.matrix;
                stream << " transform=\"matrix(";
                stream << matrix.a << "," << matrix.b << "," << matrix.c << "," << matrix.d << "," << matrix.e << "," << matrix.f;
                stream << ")\" ";
                break;
            }
            case SprAttribute::Type::AnimatorSet:
                throw "not yet implemented"; // FIXME: use <animator> tag within this object's tag
                break;
            case SprAttribute::Type::Shadow:
                throw "not yet implemented";
                stream << "";
                break;
            case SprAttribute::Type::StrokeLinecap:
                stream << " stroke-linecap=\"" << ((SprAttributeStrokeLinecap*)attr.get())->record.linecap << "\" ";
                break;
            case SprAttribute::Type::StrokeLinejoin:
                stream << " stroke-linejoin=\"" << ((SprAttributeStrokeLinejoin*)attr.get())->record.linejoin << "\" ";
                break;
            case SprAttribute::Type::StrokeWidth:
                stream << " stroke-width=\"" << ((SprAttributeStrokeWidth*)attr.get())->record.strokewidth << "\" ";
                break;
            case SprAttribute::Type::StrokeMiterlimit:
                throw "implementation incomplete";
                // FIXME: convert the float to an integer?
                stream << " stroke-linejoin=\"miter\" stroke-miterlimit=\""<< ((SprAttributeStrokeMiterlimit*)attr.get())->record.miterlimit << "\" ";
                break;
            default:
                break;
        }   
    }
}

#define PRINT_SHAPE_OPENING(obj, stream, shape, id, artags) {    \
    stream << "<" shape " ";                                \
    if (!id.empty()) stream << "id=\"" << id << "\" ";      \
    addSvgObjectAttibutes(obj->attributes, stream, artags); \
    stream << ">" << std::endl;                            \
}

#define PRINT_SHAPE_CLOSING(stream, shape) {    \
    stream << "</" shape ">" << std::endl;      \
}

#define PRINT_SHAPE_SELF_CLOSING(obj, stream, shape, id, artags) { \
    stream << "<" shape " ";                                  \
    if (!id.empty()) stream << "id=\"" << id << "\" ";        \
    addSvgObjectAttibutes(obj->attributes, stream, artags);   \
    stream << "/>" << std::endl;                             \
}

static void drawSvgCircle(SprObjectCircle *obj, std::stringstream &stream, std::stringstream &artags, std::string const &id = "")
{
    PRINT_SHAPE_SELF_CLOSING(obj, stream, "circle", id, artags);
}

static void drawSvgEllipse(SprObjectEllipse *obj, std::stringstream &stream, std::stringstream &artags, std::string const &id = "")
{
    PRINT_SHAPE_SELF_CLOSING(obj, stream, "ellipse", id, artags);
}

static void drawSvgLine(SprObjectLine *obj, std::stringstream &stream, std::stringstream &artags, std::string const &id = "")
{
    PRINT_SHAPE_SELF_CLOSING(obj, stream, "line", id, artags);
}

static void drawSvgRectangle(SprObjectRectangle *obj, std::stringstream &stream, std::stringstream &artags, std::string const &id = "")
{
    PRINT_SHAPE_SELF_CLOSING(obj, stream, "rect", id, artags);
}

static void drawSvgUse(SprObjectUse *obj, std::stringstream &stream, std::stringstream &artags, std::string const &id = "")
{
    PRINT_SHAPE_SELF_CLOSING(obj, stream, "use", id, artags);
}

static int isLargeArc(float delta)
{
    if (delta < 0) delta = fmod(360, delta);
    return (delta > 180) ? 1 : 0;
}

static void drawSvgPath(SprObjectPath *obj, std::stringstream &stream, std::stringstream &artags, std::string const &id = "")
{
    bool close_called = false;
    
    stream << "<path ";
    if (!id.empty()) stream << "id=\"" << id << "\" ";
    addSvgObjectAttibutes(obj->attributes, stream, artags);
    
    stream << "d=\"";
    
    for (auto &cmd : obj->commands) {
        if (cmd.type == SprObjectPath::Command::Type::NONE)
            continue;
        else if (cmd.type == SprObjectPath::Command::Type::MOVETO)  // (x, y)
        {
            stream << "M" << cmd.u.moveto.x << " " << cmd.u.moveto.y << " ";
        }
        else if (cmd.type == SprObjectPath::Command::Type::LINETO)  // (x, y)
        {
            stream << "L" << cmd.u.lineto.x << " " << cmd.u.lineto.y << " ";
        }
        else if (cmd.type == SprObjectPath::Command::Type::QUADRATIC_CURVETO)  // (x1 y1 x y)
        {
            stream << "Q" << cmd.u.quadratic_curveto.x << "," << cmd.u.quadratic_curveto.y << " ";
            stream << cmd.u.quadratic_curveto.lastX << "," << cmd.u.quadratic_curveto.lastY << " ";
        }
        else if (cmd.type == SprObjectPath::Command::Type::BEZIER_CURVETO)  // (x1 y1 x2 y2 x y)
        {
            stream << "C" << cmd.u.bezier_curveto.x1 << "," << cmd.u.bezier_curveto.y1 << " ";
            stream << cmd.u.bezier_curveto.x2 << "," << cmd.u.bezier_curveto.y2 << " ";
            stream << cmd.u.bezier_curveto.lastX << "," << cmd.u.bezier_curveto.lastY << " ";
        }
        else if (cmd.type == SprObjectPath::Command::Type::ELLIPTICAL_ARC)  // (rx ry x-axis-rotation large-arc-flag sweep-flag x y)
        {
            float angledelta = cmd.u.elliptical_arc.endAngle - cmd.u.elliptical_arc.startAngle;
            stream << "A" << cmd.u.elliptical_arc.x1 << "," << cmd.u.elliptical_arc.y1 << " "; // rx, ry
            stream << "0 "; // x-axis-rotation
            stream << (angledelta > 0.0 ? 1 : 0) << "," << isLargeArc(angledelta) << " "; // large-arc-flag, sweep-flag
            stream << cmd.u.elliptical_arc.x2 << "," << cmd.u.elliptical_arc.y2 << " "; // x, y
        }
        else if (cmd.type == SprObjectPath::Command::Type::CLOSE)
        {
            stream << "Z";
            close_called = true;
            break; // we force a break here to avoid parsing incorrect paths
        }
        else throw "unrecognized path command";
    }
    
    // samsung doesn't always seem to close the path in my sample. So we fix that.
    if (!close_called)
        stream << "Z";
        
    stream << "\" />" << std::endl;
}

static void drawSvgGroup(SprObjectGroup *group, std::stringstream &stream, std::stringstream &artags, std::string const &id = "")
{
    PRINT_SHAPE_OPENING(group, stream, "g", id, artags);
    
    for (auto &obj : group->objects) {
        switch (obj->type) {
            case SprObject::Type::Circle:
                drawSvgCircle((SprObjectCircle*)obj.get(), stream, artags);
                break;
            case SprObject::Type::Ellipse:
                drawSvgEllipse((SprObjectEllipse*)obj.get(), stream, artags);
                break;
            case SprObject::Type::Line:
                drawSvgLine((SprObjectLine*)obj.get(), stream, artags);
                break;
            case SprObject::Type::Path:
                drawSvgPath((SprObjectPath*)obj.get(), stream, artags);
                break;
            case SprObject::Type::Rectangle:
                drawSvgRectangle((SprObjectRectangle*)obj.get(), stream, artags);
                break;
            case SprObject::Type::Group:
                drawSvgGroup((SprObjectGroup*)obj.get(), stream, artags);
                break;
            case SprObject::Type::Use:
                drawSvgUse((SprObjectUse*)obj.get(), stream, artags);
                break;
            default:
                fprintf(stderr, "unknown object type: %d", (int) obj->type);
                throw "";
        }
    }
    
    PRINT_SHAPE_CLOSING(stream, "g");
}

const std::string&
SprDocument::drawSvg(void)
{
    std::stringstream mainstream, refstream;
    std::vector<std::stringstream> docstreams;
    // some SPR attributes are actually svg def/ref tags.
    // So we need to actively register them and lqter add them when merging the streams.
    std::stringstream attrreftags; // or 'artags' for short
    
    for (auto &fattr : fileattrs)
    {
        switch (fattr->type) {
            case SprFileAttribute::Type::NinePatch:
                throw "ninepatch isn't yet supported";
                break;
            case SprFileAttribute::Type::None:
                break;
            default:
                throw "unknown file attribute";
                break;
        }
    }
    
    // it's okay to have an empty defs segment
    for (auto &refobj : refobjects)
    {
        auto id = getNewObjectId();
        std::stringstream idstr;
        
        switch (refobj->type) {
            case SprObject::Type::Circle:
                idstr << "circle" << id;
                drawSvgCircle((SprObjectCircle*)refobj.get(), refstream, attrreftags, idstr.str());
                break;
            case SprObject::Type::Ellipse:
                idstr << "ellipse" << id;
                drawSvgEllipse((SprObjectEllipse*)refobj.get(), refstream, attrreftags, idstr.str());
                break;
            case SprObject::Type::Line:
                idstr << "line" << id;
                drawSvgLine((SprObjectLine*)refobj.get(), refstream, attrreftags, idstr.str());
                break;
            case SprObject::Type::Path:
                idstr << "path" << id;
                drawSvgPath((SprObjectPath*)refobj.get(), refstream, attrreftags, idstr.str());
                break;
            case SprObject::Type::Rectangle:
                idstr << "rectangle" << id;
                drawSvgRectangle((SprObjectRectangle*)refobj.get(), refstream, attrreftags, idstr.str());
                break;
            case SprObject::Type::Group:
                idstr << "group" << id;
                drawSvgGroup((SprObjectGroup*)refobj.get(), refstream, attrreftags, idstr.str());
                break;
            case SprObject::Type::Use:
                idstr << "use" << id;
                drawSvgUse((SprObjectUse*)refobj.get(), refstream, attrreftags, idstr.str());
                break;
            default:
                fprintf(stderr, "unknown object type: %d", (int) refobj->type);
                throw "";
        }
    }
    
    
    for (auto &obj : documents)
    {
        std::stringstream docstream;
        if (obj->type != SprObject::Type::Group) {
            fprintf(stderr, "unexpected object type: %d", (int) obj->type);
            throw "";
        }
        
        // root SPR documents don't have attributes.
        // With our design, attributes will only be printed if they exist.
        drawSvgGroup((SprObjectGroup*)obj.get(), docstream, attrreftags);
        docstreams.push_back(std::move(docstream));
    }
    
    // MARK: merge all the streams
    
    // open svg tag
    mainstream << "<svg version=\"1.1\" width=\"" << header.base.dimensions.width << "\" ";
    mainstream << "height=\"" << header.base.dimensions.height << "\" ";
    mainstream << "xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
    
    // add defs/refs
    mainstream << "<defs>" << std::endl;
    mainstream << attrreftags.str(); // add the SPR attributes that are SVG def tags
    mainstream << refstream.str();
    mainstream << "</defs>" << std::endl;
    
    // add documents
    mainstream << "<g>" << std::endl;
    for (auto &docstream : docstreams)
        mainstream << docstream.str() << std::endl;
    mainstream << "</g>" << std::endl;

    // finally close the svg tag
    mainstream << "</svg>";
    
    // cache the svg output
    cached_svg = mainstream.str();
    
    return cached_svg;
}
