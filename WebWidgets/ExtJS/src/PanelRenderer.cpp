//
// PanelRenderer.cpp
//
// $Id: //poco/Main/WebWidgets/ExtJS/src/PanelRenderer.cpp#5 $
//
// Library: ExtJS
// Package: Core
// Module:  PanelRenderer
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/WebWidgets/ExtJS/PanelRenderer.h"
#include "Poco/WebWidgets/ExtJS/Utility.h"
#include "Poco/WebWidgets/Panel.h"
#include "Poco/WebWidgets/Layout.h"


namespace Poco {
namespace WebWidgets {
namespace ExtJS {


PanelRenderer::PanelRenderer()
{
}


PanelRenderer::~PanelRenderer()
{
}


void PanelRenderer::renderHead(const Renderable* pRenderable, const RenderContext& context, std::ostream& ostr)
{
	poco_assert_dbg (pRenderable != 0);
	poco_assert_dbg (pRenderable->type() == typeid(Poco::WebWidgets::Panel));
	const Panel* pPanel = static_cast<const Poco::WebWidgets::Panel*>(pRenderable);
	ostr << "new Ext.Window({";
	
	Utility::writeRenderableProperties(pRenderable, ostr);
	ostr <<		",manager: winGrp";
	if (pPanel->hasPosition())
		ostr << ",x:" << pPanel->getPosition().posX << ",y:" << pPanel->getPosition().posY;
	if (!pPanel->getName().empty() && pPanel->showHeader())
		ostr << ",title:'" << pPanel->getTitle() << "'";
	if (pPanel->getWidth() > 0)
		ostr << ",width:" << pPanel->getWidth();
	else
		ostr << ",width:100"; // required!
	if (pPanel->getHeight() > 0)
		ostr << ",height:" << pPanel->getHeight();
	else
		ostr << ",height:100"; // required!
	if (pPanel->getModal())
		ostr << ",modal:true";
	if (!pPanel->hasCloseIcon())
		ostr << ",closable:false";
	else
		ostr << ",closeAction:'hide'";
	if (pPanel->isVisible())	
		ostr << ",renderTo:Ext.getBody()";
	
	
	// minimizable set to true only fires a minimize event, without an event handler attached
	// it is pretty useless, instead use collapsible
	if (pPanel->showHeader())
		ostr << ",header:true,maximizable:true,collapsible:true";
	else
		ostr << ",header:false";
	
	if (!pPanel->enabled())	
		ostr << ",disabled:true";

	// a panel has exactly one child
	// if this child is a layout we are fine otherwise we have to generate the items array
	View::Ptr pChild = pPanel->getChild();
	if (pChild)
	{
		Layout::Ptr pLayout = pChild.cast<Layout>();
		if (pLayout)
		{
			ostr << ",";
			pLayout->renderHead(context, ostr);
		}
		else
		{
			ostr << ",items:[";
			pChild->renderHead(context, ostr);
			ostr << "]";
		}
	}
	ostr << "})";
}


void PanelRenderer::renderHeadWithoutChildren(const Panel* pPanel, const RenderContext& context, std::ostream& ostr)
{
	ostr << "new Ext.Window({";
	
	Utility::writeRenderableProperties(pPanel, ostr);
	ostr <<		",manager: winGrp";
	if (!pPanel->getName().empty() && pPanel->showHeader())
		ostr << ",title:'" << pPanel->getTitle() << "'";
	if (pPanel->hasPosition())
		ostr << ",x:" << pPanel->getPosition().posX << ",y:" << pPanel->getPosition().posY;
	if (pPanel->getWidth() > 0)
		ostr << ",width:" << pPanel->getWidth();
	else
		ostr << ",width:100"; // required!
	if (pPanel->getHeight() > 0)
		ostr << ",height:" << pPanel->getHeight();
	else
		ostr << ",height:100"; // required!
	if (pPanel->getModal())
		ostr << ",modal:true";
	if (!pPanel->hasCloseIcon())
		ostr << ",closable:false";
	else
		ostr << ",closeAction:'hide'";
	if (pPanel->isVisible())	
		ostr << ",renderTo:Ext.getBody()";
	
	
	// minimizable set to true only fires a minimize event, without an event handler attached
	// it is pretty useless, instead use collapsible
	if (pPanel->showHeader())
		ostr << ",header:true,maximizable:true,collapsible:true";
	else
		ostr << ",header:false";
	
	if (!pPanel->enabled())	
		ostr << ",disabled:true";

	ostr << "})";
}



void PanelRenderer::renderAsPanelChild(const Panel* pPanel, const RenderContext& context, std::ostream& ostr)
{
	ostr << "new Ext.Panel({";

	ostr << "border:false,bodyBorder:false,autoScroll:true";
	View::Ptr pChild = pPanel->getChild();
	if (pChild)
	{
		Layout::Ptr pLayout = pChild.cast<Layout>();
		if (pLayout)
		{
			ostr << ",";
			pLayout->renderHead(context, ostr);
		}
		else
		{
			ostr << ",items:[";
			pChild->renderHead(context, ostr);
			ostr << "]";
		}
	}
	ostr << "})";
}



void PanelRenderer::renderBody(const Renderable* pRenderable, const RenderContext& context, std::ostream& ostr)
{
	const Panel* pPanel = static_cast<const Poco::WebWidgets::Panel*>(pRenderable);
	View::Ptr pChild = pPanel->getChild();
	ostr << "<div id=\"p" << pPanel->id() << "\" />";
	if (pChild)
		pChild->renderBody(context, ostr);
}


} } } // namespace Poco::WebWidgets::ExtJS
