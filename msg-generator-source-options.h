/*
 * Copyright (c) 2018 Balabit
 * Copyright (c) 2018 László Várady <laszlo.varady@balabit.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#ifndef MSG_GENERATOR_SOURCE_OPTIONS_H
#define MSG_GENERATOR_SOURCE_OPTIONS_H

#include "logsource.h"
#include "template/templates.h"
#include "logmsg/logmsg.h"
#include <compat/glib.h>

typedef struct
{
  LogSourceOptions super;
  guint freq;
  gint max_num;
  LogTemplate *template;
  GHashTable *name_value;
} MsgGeneratorSourceOptions;

static inline void
msg_generator_source_options_defaults(MsgGeneratorSourceOptions *self)
{
  log_source_options_defaults(&self->super);
  self->freq = 1000;
  self->name_value = g_hash_table_new(g_str_hash, g_str_equal);  
}

static inline void
msg_generator_source_options_init(MsgGeneratorSourceOptions *self, GlobalConfig *cfg, const gchar *group_name)
{
  log_source_options_init(&self->super, cfg, group_name);
}

static inline void
msg_generator_source_options_destroy(MsgGeneratorSourceOptions *self)
{
  log_template_unref(self->template);
  log_source_options_destroy(&self->super);
}

static inline void
msg_generator_source_options_set_freq(MsgGeneratorSourceOptions *self, gdouble freq)
{
  self->freq = (guint) (freq * 1000);
}

static inline void
msg_generator_source_options_set_num(MsgGeneratorSourceOptions *self, gint max_num)
{
  self->max_num = max_num;
}

static inline void
msg_generator_source_options_set_template(MsgGeneratorSourceOptions *self, LogTemplate *template)
{
  log_template_unref(self->template);
  self->template = log_template_ref(template);
}
static inline void
create_log_name_value_pair(MsgGeneratorSourceOptions *self, const gchar *name, LogTemplate *template)
{ 
  GString *msg_body = g_string_sized_new(128);
  LogMessage *msg = log_msg_new_empty();
  log_template_format(template, msg, NULL, LTZ_LOCAL, 0, NULL, msg_body);
  gchar *value=msg_body->str;
  g_hash_table_insert(self->name_value, g_strdup(name), g_strdup(value));
  g_string_free(msg_body, TRUE);
}
#endif

