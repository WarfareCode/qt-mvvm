// ************************************************************************** //
//
//  Prototype of mini MVVM framework for bornagainproject.org
//
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher
//
// ************************************************************************** //

#include "standarditemcatalogue.h"
#include "mvvm_types.h"
#include "vectoritem.h"
#include "propertyitem.h"
#include "compounditem.h"

std::unique_ptr<ModelView::ItemCatalogue> ModelView::CreateStandardItemCatalogue()
{
    auto result = std::make_unique<ModelView::ItemCatalogue>();
    result->registerItem<ModelView::SessionItem>();
    result->registerItem<ModelView::PropertyItem>();
    result->registerItem<ModelView::CompoundItem>();
    result->registerItem<ModelView::VectorItem>();
    return result;
}
