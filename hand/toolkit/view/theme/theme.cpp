#include <iostream>
#include "view/theme/theme.h"
#include "view/datatypes/rgb.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/layout.h"
#include "view/layer/virtualsurface.h"


Theme::Theme(const std::string& name) : HandApp(name)
{
    if(TTF_Init() == -1)
    {
        std::cout << TTF_GetError() << std::endl;
        exit(22);
    }
    type(THEME);
}


Theme::~Theme()
{
    std::map<int, TTF_Font*>::iterator curr = Fonts.begin();
    while(curr!=Fonts.end())
    {
        TTF_CloseFont((*curr).second);
        curr++;
    }
}


bool Theme::execute(Vertex* input)
{
    if(!dynamic_cast<Layout*>(input))
        return false;

    // Get the layout factory
    Vertex* req = input->Vertex::get(ANY, REQUEST);
    if(req)
    {
        req = req->get();
        if(req)
            get(LAYOUT)->get(req->name())->execute(input);
    }

    Vertex* sub;
    unsigned i = 0;
    while((sub=input->get(++i)) != nullptr)
    {
        std::string name = sub->name();
        if((name==LAYER_FACTORIES) || (name==TARGET) || (name==THEME))
            continue;

        if(name == FIELDS)
        {
            unsigned j = 0;
            Vertex* child;
            while((child=sub->get(++j)) != nullptr)
                execute(child);
        }
        else
            FillOut(sub);
    }
    return true;
}


bool Theme::FillOut(Vertex* request)
{
    Vertex* req = request->Vertex::get(ANY, REQUEST);
    if(req)
        req = req->get();
    if(!req)
        // Nothing to do
        return false;
    if(request->get(req->name(), ANY))
        // Already resolved
        return false;
    // TODO: Vertex::get(Vertex* path)
    Vertex* repo = get(ANY, req->name());
    if(!repo)
        return false;
    while((req=req->get()) != nullptr)
    {
        repo = repo->get(ANY, req->name());
        if(!repo)
            return false;
    }
    if(repo->is(FACTORY))
        return request->set(repo->get());
    return request->set(repo);
}


TTF_Font* Theme::GetFont(int size)
{
    // Loading font:
    TTF_Font* font = Fonts[size];
    if(!font)
    {
        font = TTF_OpenFont(FONT_FILE, size);
        if(!font)
        {
            std::cout << "Unable to load font:" << FONT_FILE
                    << "\nTTF_OpenFont: " << TTF_GetError() << std::endl;
            exit(25);
        }
        Fonts[size] = font;
    }
    return font;
}


void Theme::GetFontHeight(Vertex* layout, unsigned& max_size)
{
    Vertex* pref = layout->get(DIM_FONT_PREFERRED);
    if(pref)
        max_size = ((Data<unsigned>*)pref)->get();
    else
    {
        unsigned max = ((Data<unsigned>*)layout->get(DIM_FONT_MAX))->get();
        if(max_size >= max)
            max_size = max;
    }
}


VirtualSurface* Theme::GetSurface(Vertex* in)
{
    return dynamic_cast<VirtualSurface*>(in->get(LAYER, ANY));
}


void Theme::FillRect(SDL_Surface* sf, SDL_Rect* r, Rgb* c)
{
    SDL_FillRect(sf, r, SDL_MapRGB(sf->format, c->r, c->g, c->b));
}


SDL_Surface* Theme::RenderText(const std::string& text, int size, Rgb* color)
{
    SDL_Color sdl_color;
    sdl_color.r = color->r;
    sdl_color.g = color->g;
    sdl_color.b = color->b;
    return TTF_RenderText_Blended(GetFont(size), text.c_str(), sdl_color);
}


void Theme::PlaceCentered(SDL_Surface* source, SDL_Rect& target, Rel_Rect& out)
{
    out.w = double(source->w) / double(target.w);
    out.h = double(source->h) / double(target.h);
    out.x = (1 - out.w) / 2;
    out.y = (1 - out.h) / 2;
}
