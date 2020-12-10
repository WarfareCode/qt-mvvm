// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_SIGNALS_ITEMLISTENER_H
#define MVVM_SIGNALS_ITEMLISTENER_H

#include <mvvm/signals/itemlistenerbase.h>

namespace ModelView
{

class SessionItem;

//! Base class to subscribe to signals generated by SessionItem of certin type.

template <typename T> class ItemListener : public ItemListenerBase
{
public:
    T* currentItem() const { return static_cast<T*>(item()); }
};

} // namespace ModelView

#endif // MVVM_SIGNALS_ITEMLISTENER_H