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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "xfdesktop-icon.h"
#include "xfdesktop-window-icon.h"

struct _XfdesktopWindowIconPrivate
{
    gint16 row;
    gint16 col;
    GdkPixbuf *pix;
    gint cur_pix_size;
    gchar *label;
    GdkRectangle extents;
    NetkWindow *window;
};

static void xfdesktop_window_icon_icon_init(XfdesktopIconIface *iface);
static void xfdesktop_window_icon_finalize(GObject *obj);

static GdkPixbuf *xfdesktop_window_icon_peek_pixbuf(XfdesktopIcon *icon,
                                                   gint size);
static G_CONST_RETURN gchar *xfdesktop_window_icon_peek_label(XfdesktopIcon *icon);

static void xfdesktop_window_icon_set_position(XfdesktopIcon *icon,
                                               gint16 row,
                                               gint16 col);
static gboolean xfdesktop_window_icon_get_position(XfdesktopIcon *icon,
                                                   gint16 *row,
                                                   gint16 *col);

static void xfdesktop_window_icon_set_extents(XfdesktopIcon *icon,
                                              const GdkRectangle *extents);
static gboolean xfdesktop_window_icon_get_extents(XfdesktopIcon *icon,
                                                  GdkRectangle *extents);

static void xfdesktop_window_icon_selected(XfdesktopIcon *icon);
static void xfdesktop_window_icon_activated(XfdesktopIcon *icon);
static void xfdesktop_window_icon_menu_popup(XfdesktopIcon *icon);


G_DEFINE_TYPE_EXTENDED(XfdesktopWindowIcon, xfdesktop_window_icon,
                       G_TYPE_OBJECT, 0,
                       G_IMPLEMENT_INTERFACE(XFDESKTOP_TYPE_ICON,
                                             xfdesktop_window_icon_icon_init))

static void
xfdesktop_window_icon_class_init(XfdesktopWindowIconClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;
    
    gobject_class->finalize = xfdesktop_window_icon_finalize;
}

static void
xfdesktop_window_icon_init(XfdesktopWindowIcon *icon)
{
    icon->priv = g_new0(XfdesktopWindowIconPrivate, 1);
}

static void
xfdesktop_window_icon_finalize(GObject *obj)
{
    XfdesktopWindowIcon *window_icon = XFDESKTOP_WINDOW_ICON(obj);
    
    if(window_icon->priv->pix)
        g_object_unref(G_OBJECT(window_icon->priv->pix));
    
    g_free(window_icon->priv->label);
    
    /* TODO: disconnect signal handlers from ->window */
};

static void
xfdesktop_window_icon_icon_init(XfdesktopIconIface *iface)
{
    iface->peek_pixbuf = xfdesktop_window_icon_peek_pixbuf;
    iface->peek_label = xfdesktop_window_icon_peek_label;
    iface->set_position = xfdesktop_window_icon_set_position;
    iface->get_position = xfdesktop_window_icon_get_position;
    iface->set_extents = xfdesktop_window_icon_set_extents;
    iface->get_extents = xfdesktop_window_icon_get_extents;
    iface->selected = xfdesktop_window_icon_selected;
    iface->activated = xfdesktop_window_icon_activated;
    iface->menu_popup = xfdesktop_window_icon_menu_popup;
}

XfdesktopWindowIcon *
xfdesktop_window_icon_new(NetkWindow *window)
{
    XfdesktopWindowIcon *icon = g_object_new(XFDESKTOP_TYPE_WINDOW_ICON, NULL);
    
    icon->priv->window = window;
    
    return icon;
}

static GdkPixbuf *
xfdesktop_window_icon_peek_pixbuf(XfdesktopIcon *icon,
                                 gint size)
{
    XfdesktopWindowIcon *window_icon = XFDESKTOP_WINDOW_ICON(icon);
    
    if(!window_icon->priv->pix || window_icon->priv->cur_pix_size != size) {
        if(window_icon->priv->pix)
            g_object_unref(G_OBJECT(window_icon->priv->pix));
        
        /* TODO: set up the icon */
    }
    
    return window_icon->priv->pix;
}

static G_CONST_RETURN gchar *
xfdesktop_window_icon_peek_label(XfdesktopIcon *icon)
{
    XfdesktopWindowIcon *window_icon = XFDESKTOP_WINDOW_ICON(icon);
    
    if(!window_icon->priv->label) {
        /* TODO: get label */
    }
    
    return window_icon->priv->label;
}

static void
xfdesktop_window_icon_set_position(XfdesktopIcon *icon,
                                   gint16 row,
                                   gint16 col)
{
    XfdesktopWindowIcon *window_icon = XFDESKTOP_WINDOW_ICON(icon);
    
    window_icon->priv->row = row;
    window_icon->priv->row = col;
}
    
static gboolean
xfdesktop_window_icon_get_position(XfdesktopIcon *icon,
                                   gint16 *row,
                                   gint16 *col)
{
    XfdesktopWindowIcon *window_icon = XFDESKTOP_WINDOW_ICON(icon);
    
    *row = window_icon->priv->row;
    *col = window_icon->priv->col;
    
    return TRUE;
}

static void
xfdesktop_window_icon_set_extents(XfdesktopIcon *icon,
                                  const GdkRectangle *extents)
{
    XfdesktopWindowIcon *window_icon = XFDESKTOP_WINDOW_ICON(icon);
    
    memcpy(&window_icon->priv->extents, extents, sizeof(GdkRectangle));
}

static gboolean
xfdesktop_window_icon_get_extents(XfdesktopIcon *icon,
                                  GdkRectangle *extents)
{
    XfdesktopWindowIcon *window_icon = XFDESKTOP_WINDOW_ICON(icon);
    
    if(window_icon->priv->extents.width > 0
       && window_icon->priv->extents.height > 0)
    {
        memcpy(extents, &window_icon->priv->extents, sizeof(GdkRectangle));
        return TRUE;
    }
    
    return FALSE;
}

static void
xfdesktop_window_icon_selected(XfdesktopIcon *icon)
{
    /* nada */
}

static void
xfdesktop_window_icon_activated(XfdesktopIcon *icon)
{
    XfdesktopWindowIcon *window_icon = XFDESKTOP_WINDOW_ICON(icon);
    
    (void)window_icon;
    /* TODO: activate window */
}
static void
xfdesktop_window_icon_menu_popup(XfdesktopIcon *icon)
{
    XfdesktopWindowIcon *window_icon = XFDESKTOP_WINDOW_ICON(icon);
    
    (void)window_icon;
    /* TODO: popup action menu */
}