/*
 *  xfdesktop - xfce4's desktop manager
 *
 *  Copyright (c) 2006 Brian Tarricone, <bjt23@cornell.edu>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __XFDESKTOP_ICON_VIEW_H__
#define __XFDESKTOP_ICON_VIEW_H__

#include <gtk/gtk.h>

#include "xfdesktop-icon.h"

G_BEGIN_DECLS

#define XFDESKTOP_TYPE_ICON_VIEW     (xfdesktop_icon_view_get_type())
#define XFDESKTOP_ICON_VIEW(obj)     (G_TYPE_CHECK_INSTANCE_CAST((obj), XFDESKTOP_TYPE_ICON_VIEW, XfdesktopIconView))
#define XFDESKTOP_IS_ICON_VIEW(obj)  (G_TYPE_CHECK_INSTANCE_TYPE((obj), XFDESKTOP_TYPE_ICON_VIEW))

typedef struct _XfdesktopIconView         XfdesktopIconView;
typedef struct _XfdesktopIconViewClass    XfdesktopIconViewClass;
typedef struct _XfdesktopIconViewPrivate  XfdesktopIconViewPrivate;

typedef void (*XfdesktopIconViewIconInitFunc)(XfdesktopIconView *icon_view);
typedef void (*XfdesktopIconViewIconFiniFunc)(XfdesktopIconView *icon_view);

struct _XfdesktopIconView
{
    GtkWidget parent;
    
    /*< private >*/
    XfdesktopIconViewPrivate *priv;
};

struct _XfdesktopIconViewClass
{
    GtkWidgetClass parent;
};

GType xfdesktop_icon_view_get_type() G_GNUC_CONST;

GtkWidget *xfdesktop_icon_view_new(XfdesktopIconViewIconInitFunc init_func,
                                   XfdesktopIconViewIconFiniFunc fini_func);

void xfdesktop_icon_view_add_item(XfdesktopIconView *icon_view,
                                  XfdesktopIcon *icon);

void xfdesktop_icon_view_remove_item(XfdesktopIconView *icon_view,
                                     XfdesktopIcon *icon);
void xfdesktop_icon_view_remove_all(XfdesktopIconView *icon_view);

void xfdesktop_icon_view_set_selection_mode(XfdesktopIconView *icon_view,
                                            GtkSelectionMode mode);
GtkSelectionMode xfdesktop_icon_view_get_selection_mode(XfdesktopIconView *icon_view);

void xfdesktop_icon_view_set_allow_overlapping_drops(XfdesktopIconView *icon_view,
                                                     gboolean allow_overlap);
gboolean xfdesktop_icon_view_get_allow_overlapping_drops(XfdesktopIconView *icon_view);

XfdesktopIcon *xfdesktop_icon_view_widget_coords_to_item(XfdesktopIconView *icon_view,
                                                         gint wx,
                                                         gint wy);

GList *xfdesktop_icon_view_get_selected_items(XfdesktopIconView *icon_view);

void xfdesktop_icon_view_select_item(XfdesktopIconView *icon_view,
                                     XfdesktopIcon *icon);
void xfdesktop_icon_view_unselect_item(XfdesktopIconView *icon_view,
                                       XfdesktopIcon *icon);
void xfdesktop_icon_view_unselect_all(XfdesktopIconView *icon_view);

G_END_DECLS

#endif  /* __XFDESKTOP_ICON_VIEW_H__ */