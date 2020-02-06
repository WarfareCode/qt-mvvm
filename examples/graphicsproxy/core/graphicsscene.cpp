// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "graphicsscene.h"
#include "colormapproxywidget.h"

GraphicsScene::GraphicsScene(QObject* parent) : QGraphicsScene(parent) {}

void GraphicsScene::setColorMap(ModelView::ColorMapCanvas* colormap)
{
    auto proxy = new ColorMapProxyWidget(colormap);
    addItem(proxy);
}
