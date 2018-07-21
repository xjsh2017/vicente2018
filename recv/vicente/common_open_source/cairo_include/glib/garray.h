/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

#if defined(G_DISABLE_SINGLE_INCLUDES) && !defined (__GLIB_H_INSIDE__) && !defined (GLIB_COMPILATION)
#error "Only <glib.h> can be included directly."
#endif

#ifndef __G_ARRAY_H__
#define __G_ARRAY_H__

#include <glib/gtypes.h>

G_BEGIN_DECLS

typedef struct _GArray		GArray;
typedef struct _GByteArray	GByteArray;
typedef struct _GPtrArray	GPtrArray;

struct _GArray
{
  gchar *data;
  guint len;
};

struct _GByteArray
{
  guint8 *data;
  guint	  len;
};

struct _GPtrArray
{
  gpointer *pdata;
  guint	    len;
};

/* Resizable arrays. remove fills any cleared spot and shortens the
 * arg_array, while preserving the order. remove_fast will distort the
 * order by moving the last element to the position of the removed.
 */

#define g_array_append_val(a,v)	  g_array_append_vals (a, &(v), 1)
#define g_array_prepend_val(a,v)  g_array_prepend_vals (a, &(v), 1)
#define g_array_insert_val(a,i,v) g_array_insert_vals (a, i, &(v), 1)
#define g_array_index(a,t,i)      (((t*) (void *) (a)->data) [(i)])

GArray* g_array_new               (gboolean          zero_terminated,
				   gboolean          clear_,
				   guint             element_size);
GArray* g_array_sized_new         (gboolean          zero_terminated,
				   gboolean          clear_,
				   guint             element_size,
				   guint             reserved_size);
gchar*  g_array_free              (GArray           *arg_array,
				   gboolean          free_segment);
GArray *g_array_ref               (GArray           *arg_array);
void    g_array_unref             (GArray           *arg_array);
guint   g_array_get_element_size  (GArray           *arg_array);
GArray* g_array_append_vals       (GArray           *arg_array,
				   gconstpointer     data,
				   guint             len);
GArray* g_array_prepend_vals      (GArray           *arg_array,
				   gconstpointer     data,
				   guint             len);
GArray* g_array_insert_vals       (GArray           *arg_array,
				   guint             index_,
				   gconstpointer     data,
				   guint             len);
GArray* g_array_set_size          (GArray           *arg_array,
				   guint             length);
GArray* g_array_remove_index      (GArray           *arg_array,
				   guint             index_);
GArray* g_array_remove_index_fast (GArray           *arg_array,
				   guint             index_);
GArray* g_array_remove_range      (GArray           *arg_array,
				   guint             index_,
				   guint             length);
void    g_array_sort              (GArray           *arg_array,
				   GCompareFunc      compare_func);
void    g_array_sort_with_data    (GArray           *arg_array,
				   GCompareDataFunc  compare_func,
				   gpointer          user_data);

/* Resizable pointer arg_array.  This interface is much less complicated
 * than the above.  Add appends a pointer.  Remove fills any cleared 
 * spot and shortens the arg_array. remove_fast will again distort order.  
 */
#define    g_ptr_array_index(arg_array,index_) ((arg_array)->pdata)[index_]
GPtrArray* g_ptr_array_new                (void);
GPtrArray* g_ptr_array_new_with_free_func (GDestroyNotify    element_free_func);
GPtrArray* g_ptr_array_sized_new          (guint             reserved_size);
gpointer*  g_ptr_array_free               (GPtrArray        *arg_array,
					   gboolean          free_seg);
GPtrArray* g_ptr_array_ref                (GPtrArray        *arg_array);
void       g_ptr_array_unref              (GPtrArray        *arg_array);
void       g_ptr_array_set_free_func      (GPtrArray        *arg_array,
                                           GDestroyNotify    element_free_func);
void       g_ptr_array_set_size           (GPtrArray        *arg_array,
					   gint              length);
gpointer   g_ptr_array_remove_index       (GPtrArray        *arg_array,
					   guint             index_);
gpointer   g_ptr_array_remove_index_fast  (GPtrArray        *arg_array,
					   guint             index_);
gboolean   g_ptr_array_remove             (GPtrArray        *arg_array,
					   gpointer          data);
gboolean   g_ptr_array_remove_fast        (GPtrArray        *arg_array,
					   gpointer          data);
void       g_ptr_array_remove_range       (GPtrArray        *arg_array,
					   guint             index_,
					   guint             length);
void       g_ptr_array_add                (GPtrArray        *arg_array,
					   gpointer          data);
void       g_ptr_array_sort               (GPtrArray        *arg_array,
					   GCompareFunc      compare_func);
void       g_ptr_array_sort_with_data     (GPtrArray        *arg_array,
					   GCompareDataFunc  compare_func,
					   gpointer          user_data);
void       g_ptr_array_foreach            (GPtrArray        *arg_array,
					   GFunc             func,
					   gpointer          user_data);


/* Byte arrays, an arg_array of guint8.  Implemented as a GArray,
 * but type-safe.
 */

GByteArray* g_byte_array_new               (void);
GByteArray* g_byte_array_sized_new         (guint             reserved_size);
guint8*     g_byte_array_free              (GByteArray       *arg_array,
					    gboolean          free_segment);
GByteArray *g_byte_array_ref               (GByteArray       *arg_array);
void        g_byte_array_unref             (GByteArray       *arg_array);
GByteArray* g_byte_array_append            (GByteArray       *arg_array,
					    const guint8     *data,
					    guint             len);
GByteArray* g_byte_array_prepend           (GByteArray       *arg_array,
					    const guint8     *data,
					    guint             len);
GByteArray* g_byte_array_set_size          (GByteArray       *arg_array,
					    guint             length);
GByteArray* g_byte_array_remove_index      (GByteArray       *arg_array,
					    guint             index_);
GByteArray* g_byte_array_remove_index_fast (GByteArray       *arg_array,
					    guint             index_);
GByteArray* g_byte_array_remove_range      (GByteArray       *arg_array,
					    guint             index_,
					    guint             length);
void        g_byte_array_sort              (GByteArray       *arg_array,
					    GCompareFunc      compare_func);
void        g_byte_array_sort_with_data    (GByteArray       *arg_array,
					    GCompareDataFunc  compare_func,
					    gpointer          user_data);

G_END_DECLS

#endif /* __G_ARRAY_H__ */
