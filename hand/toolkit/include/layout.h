/*
 *  Copyright 2012 Alex Stefaniuc
 *
 *  This file is part of Hand.
 *
 *  Hand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  Hand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Hand. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HAND_LAYOUT_H
#define HAND_LAYOUT_H

#include "defines.h"
#include "factory.h"


enum alignment
{
    HORIZONTAL,
    VERTICAL
};

class Rgb;
class Rect;

class Layout : public FunctoidNode
{
    public:
        Layout(string name, string type);
        virtual ~Layout(){};
//        void Reset();

        FunctoidNode* GetField(string position);
        FunctoidNode* AddField(string name, string type);
        void AddForUpdate(Functoid* sublayout);
        bool Execute(Functoid* surface);
        void Reset();
};


template <class I>
class LayoutFactory : public Factory
{
    typedef bool (I::*Method)(Functoid*);

    public:
        LayoutFactory(string output_type, I* theme, Method func) :
            Factory("LayoutFactory", GUI_LAYOUT, output_type, "")
        {
            Producer = theme;
            Function = func;
        };
        virtual ~LayoutFactory(){};

        Functoid* Produce(Functoid* in_out)
        {
            Layout* descr = dynamic_cast<Layout*>(in_out);
            if(!descr || !descr->IsType(GetInputType()))
                return NULL;
            // TODO Already initialized?
            (Producer->*Function)(in_out);
            return in_out;
        };

        void TakeBack(Functoid* product)
        {
            delete(product);
        };

        bool IsValidInput(Functoid* input)
        {
            Layout* descr = dynamic_cast<Layout*>(input);
            if(!descr)
                return false;
            return descr->IsType(GetInputType());
        };

    private:
        Method Function;
        I* Producer;
        Data<Functoid*>* Container;
};


template <class I>
class PropertyFactory : public Factory
{
    typedef bool (I::*Method)(Functoid*);

    public:
    PropertyFactory(string output_type, I* theme, Method func) :
            Factory("PropertyFactory", output_type, output_type, "")
        {
            Producer = theme;
            Function = func;
            Container = new Data<Functoid*>("Container", NULL);
        };
        virtual ~PropertyFactory(){};

        Functoid* Produce(Functoid* in_out)
        {
            Link* descr = dynamic_cast<Link*>(in_out);
            if(!descr || !descr->IsType(GetInputType()))
                return NULL;
            Functoid* layout = descr->Get();
            // Already initialized?
            if(layout)
                return layout;
            (Producer->*Function)(Container);
            layout = Container->Get();
            descr->Set(layout);
            return layout;
        };

        void TakeBack(Functoid* product)
        {
            delete(product);
        };

        bool IsValidInput(Functoid* input)
        {
            Link* descr = dynamic_cast<Link*>(input);
            if(!descr)
                return false;
            return descr->IsType(GetInputType());
        };

    private:
        Method Function;
        I* Producer;
        Data<Functoid*>* Container;
};

#endif /* HAND_LAYOUT_H */
