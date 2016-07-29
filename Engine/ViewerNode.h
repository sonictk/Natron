/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2016 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

#ifndef NATRON_ENGINE_VIEWERNODE_H
#define NATRON_ENGINE_VIEWERNODE_H

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "Global/Macros.h"

#include <string>

#if !defined(Q_MOC_RUN) && !defined(SBK_RUN)
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#endif

#include "Engine/NodeGroup.h"
#include "Engine/ViewIdx.h"
#include "Engine/EngineFwd.h"

NATRON_NAMESPACE_ENTER;

struct ViewerNodePrivate;
class ViewerNode
    : public NodeGroup
{
    GCC_DIAG_SUGGEST_OVERRIDE_OFF
    Q_OBJECT
    GCC_DIAG_SUGGEST_OVERRIDE_ON

private: // derives from EffectInstance
    // TODO: enable_shared_from_this
    // constructors should be privatized in any class that derives from boost::enable_shared_from_this<>
    ViewerNode(const NodePtr& node);

public:
    static EffectInstancePtr create(const NodePtr& node) WARN_UNUSED_RETURN;

    ViewerNodePtr shared_from_this() {
        return boost::dynamic_pointer_cast<ViewerNode>(KnobHolder::shared_from_this());
    }

    ViewerInstancePtr getInternalViewerNode() const;

    virtual ~ViewerNode();

    // Called upon node creation and then never changed
    void setUiContext(OpenGLViewerI* viewer);

    /**
     * @brief Set the uiContext pointer to NULL, preventing the gui to be deleted twice when
     * the node is deleted.
     **/
    void invalidateUiContext();

    void refreshViewsKnobVisibility();
    
    OpenGLViewerI* getUiContext() const WARN_UNUSED_RETURN;

    NodePtr getCurrentAInput() const;

    NodePtr getCurrentBInput() const;

    ViewerCompositingOperatorEnum getCurrentOperator() const;

    void connectInputToIndex(int groupInputIndex, int internalInputIndex);

    void setPickerEnabled(bool enabled);

    void setCurrentView(ViewIdx view);

    virtual ViewIdx getCurrentView() const OVERRIDE FINAL;

    void setZoomComboBoxText(const std::string& text);

    void setDisplayChannels(int index, bool setBoth);

    void setRefreshButtonDown(bool down);

    bool isViewersSynchroEnabled() const;

    void setViewersSynchroEnabled(bool enabled);

    bool isLeftToolbarVisible() const;

    bool isTopToolbarVisible() const;

    bool isClipToProjectEnabled() const;

    double getWipeAmount() const;

    double getWipeAngle() const;

    QPointF getWipeCenter() const;

    void resetWipe(); 

    bool isCheckerboardEnabled() const;

    bool isUserRoIEnabled() const;

    bool isOverlayEnabled() const;

    bool isFullFrameProcessingEnabled() const;

    double getGain() const;

    double getGamma() const;

    virtual void getPluginShortcuts(std::list<PluginActionShortcut>* shortcuts) const OVERRIDE FINAL;

    virtual int getMajorVersion() const OVERRIDE FINAL
    {
        return 1;
    }

    virtual int getMinorVersion() const OVERRIDE FINAL
    {
        return 0;
    }

    virtual std::string getPluginID() const OVERRIDE FINAL
    {
        return PLUGINID_NATRON_VIEWER_GROUP;
    }

    virtual std::string getPluginLabel() const OVERRIDE FINAL;

    virtual void getPluginGrouping(std::list<std::string>* grouping) const OVERRIDE FINAL;

    virtual std::string getPluginDescription() const OVERRIDE FINAL;


public Q_SLOTS:


    void onInputNameChanged(int,QString);

private:

    virtual void onInputChanged(int inputNb) OVERRIDE FINAL;

    virtual bool hasOverlay() const OVERRIDE FINAL
    {
        return true;
    }


    virtual bool onOverlayPenDown(double time, const RenderScale & renderScale, ViewIdx view, const QPointF & viewportPos, const QPointF & pos, double pressure, double timestamp, PenType pen) OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual void drawOverlay(double time, const RenderScale & renderScale, ViewIdx view) OVERRIDE FINAL;
    virtual bool onOverlayPenMotion(double time, const RenderScale & renderScale, ViewIdx view,
                                    const QPointF & viewportPos, const QPointF & pos, double pressure, double timestamp) OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool onOverlayPenUp(double time, const RenderScale & renderScale, ViewIdx view, const QPointF & viewportPos, const QPointF & pos, double pressure, double timestamp) OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool onOverlayPenDoubleClicked(double time,
                                           const RenderScale & renderScale,
                                           ViewIdx view,
                                           const QPointF & viewportPos,
                                           const QPointF & pos) OVERRIDE FINAL WARN_UNUSED_RETURN;
    virtual bool onOverlayKeyDown(double time, const RenderScale & renderScale, ViewIdx view, Key key, KeyboardModifiers modifiers) OVERRIDE FINAL;
    virtual bool onOverlayKeyUp(double time, const RenderScale & renderScale, ViewIdx view, Key key, KeyboardModifiers modifiers) OVERRIDE FINAL;
    virtual bool onOverlayKeyRepeat(double time, const RenderScale & renderScale, ViewIdx view, Key key, KeyboardModifiers modifiers) OVERRIDE FINAL;
    virtual bool onOverlayFocusGained(double time, const RenderScale & renderScale, ViewIdx view) OVERRIDE FINAL;
    virtual bool onOverlayFocusLost(double time, const RenderScale & renderScale, ViewIdx view) OVERRIDE FINAL;


    virtual bool knobChanged(const KnobIPtr& k, ValueChangedReasonEnum reason,
                             ViewSpec /*view*/,
                             double /*time*/,
                             bool /*originatedFromMainThread*/) OVERRIDE FINAL;


    virtual void initializeKnobs() OVERRIDE FINAL;


private:

    boost::scoped_ptr<ViewerNodePrivate> _imp;
};

inline ViewerNodePtr
toViewerNode(const EffectInstancePtr& effect)
{
    return boost::dynamic_pointer_cast<ViewerNode>(effect);
}

NATRON_NAMESPACE_EXIT;

#endif // NATRON_ENGINE_VIEWERNODE_H
