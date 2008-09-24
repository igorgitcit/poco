//
// ButtonCellRenderer.cpp
//
// $Id: //poco/Main/WebWidgets/ExtJS/src/ButtonCellRenderer.cpp#8 $
//
// Library: ExtJS
// Package: Core
// Module:  ButtonCellRenderer
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


#include "Poco/WebWidgets/ExtJS/ButtonCellRenderer.h"
#include "Poco/WebWidgets/ExtJS/FormRenderer.h"
#include "Poco/WebWidgets/ExtJS/Utility.h"
#include "Poco/WebWidgets/ButtonCell.h"
#include "Poco/WebWidgets/Button.h"
#include "Poco/WebWidgets/RequestHandler.h"
#include "Poco/WebWidgets/WebApplication.h"
#include "Poco/NumberFormatter.h"


namespace Poco {
namespace WebWidgets {
namespace ExtJS {


const std::string ButtonCellRenderer::EV_CLICK("click");


ButtonCellRenderer::ButtonCellRenderer()
{
}


ButtonCellRenderer::~ButtonCellRenderer()
{
}


void ButtonCellRenderer::renderHead(const Renderable* pRenderable, const RenderContext& context, std::ostream& ostr)
{
	poco_assert_dbg (pRenderable != 0);
	const ButtonCell* pButtonCell = static_cast<const Poco::WebWidgets::ButtonCell*>(pRenderable);
	std::string content = "'" + Utility::safe(pButtonCell->getString()) + "'";
	ButtonCellRenderer::renderButton(pButtonCell, content, true, false,ostr);
}


void ButtonCellRenderer::renderCell(const Cell* pCell, const RenderContext& context, std::ostream& ostr)
{
	poco_assert_dbg (pCell != 0);
	const ButtonCell* pButtonCell = static_cast<const Poco::WebWidgets::ButtonCell*>(pCell);

	// is now a variable name
	ButtonCellRenderer::renderButton(pButtonCell, "val", false, false, ostr);
}


void ButtonCellRenderer::renderProperties(const ButtonCell* pButtonCell, const std::string& content, bool writeId, bool submitButton, std::ostream& ostr, bool showText)
{
	if (showText && !content.empty())
		ostr << "text:" << content << ",";
	if (submitButton)
	{
		ostr << "type:'submit',";
		Form::Ptr pForm = Utility::insideForm(pButtonCell);
		ostr << "handler: function(){Ext.getCmp('" << pForm->id() << "').getForm().submit();},";
	}
	View* pView = pButtonCell->getOwner();
	poco_check_ptr (pView);
	Button* pOwner = dynamic_cast<Button*>(pView);
	// a buttoncell inside  a table will have no parent of type button!
	
	if (writeId)
		Utility::writeRenderableProperties(pView, ostr);
	if (!pButtonCell->isEnabled())
		ostr << ",disabled:true";
	
	if (!pView->getName().empty())
		ostr << ",name:'" << pOwner->getName() << "'";
	if (pView->getWidth() != 0)
		ostr << ",minWidth:" << pOwner->getWidth();
	if (!pView->isVisible())
		ostr << ",hidden:true";
	if (pView->hasPosition())
		ostr << ",x:" << pView->getPosition().posX << ",y:" << pView->getPosition().posY;
		
	if (pOwner)
	{
		if (pOwner->buttonClicked.hasJavaScriptCode())
		{
			ostr << ",listeners:{";
			Utility::writeJSEvent(ostr, EV_CLICK, pOwner->buttonClicked, &ButtonCellRenderer::createClickServerCallback, pOwner);
			ostr << "}";
		}
	}

	std::string toolTip(pButtonCell->getToolTip());
	if (!toolTip.empty())
		ostr << ",tooltip:'" << Utility::safe(toolTip) << "'";
		
	WebApplication::instance().registerAjaxProcessor(Poco::NumberFormatter::format(pView->id()), const_cast<ButtonCell*>(pButtonCell));
}


void ButtonCellRenderer::renderButton(const ButtonCell* pCell, const std::string& content, bool writeId, bool submitButton, std::ostream& ostr, bool showText)
{
	ostr << "new Ext.Button({";
	ButtonCellRenderer::renderProperties(pCell, content, writeId, submitButton, ostr);
	ostr << "})";
}


void ButtonCellRenderer::writeConfigData(const Cell* pCell, const RenderContext& context, std::ostream& ostr)
{
	ostr << "'" << Utility::safe(pCell->getString()) << "'";
}



JSDelegate ButtonCellRenderer::createClickServerCallback(const Button* pButton)
{
	//click : ( Button this, EventObject e )
	static const std::string signature("function(but,e)");
	std::map<std::string, std::string> addParams;
	addParams.insert(std::make_pair(RequestHandler::KEY_EVID, ButtonCell::EV_BUTTONCLICKED));
	return Utility::createServerCallback(signature, addParams, pButton->id(), pButton->buttonClicked.getOnSuccess(), pButton->buttonClicked.getOnFailure());
}


} } } // namespace Poco::WebWidgets::ExtJS
