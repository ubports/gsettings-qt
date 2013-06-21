/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Lars Uebernickel <lars.uebernickel@canonical.com>
 */

#include "gsettings-qml.h"
#include "qconftypes.h"
#include <gio/gio.h>

struct GSettingsQmlPrivate
{
    QByteArray schema;
    QByteArray path;
    GSettings *settings;
};

GSettingsQml::GSettingsQml(): QQmlPropertyMap(this, NULL)
{
    priv = new GSettingsQmlPrivate;
    priv->settings = NULL;
}

GSettingsQml::~GSettingsQml()
{
  if (priv->settings)
    g_object_unref(priv->settings);

  delete priv;
}

QByteArray GSettingsQml::schema() const
{
    return priv->schema;
}

void GSettingsQml::setSchema(const QByteArray &schema)
{
    if (priv->settings != NULL) {
            qWarning("GSettings.schema may only be set on construction");
            return;
    }

    priv->schema = schema;
}

QByteArray GSettingsQml::path() const
{
    return priv->path;
}

void GSettingsQml::setPath(const QByteArray &path)
{
    if (priv->settings != NULL) {
            qWarning("GSettings.path may only be set on construction");
            return;
    }

    priv->path = path;
}

/* convert 'some-key' to 'someKey' or 'SomeKey'.
 * the second form is needed for appending to 'set' for 'setSomeKey'
 */
static QString qtify_name(const char *name)
{
    bool next_cap = false;
    QString result;

    while (*name) {
        if (*name == '-') {
            next_cap = true;
        } else if (next_cap) {
            result.append(toupper(*name));
            next_cap = false;
        } else {
            result.append(*name);
        }

        name++;
    }

    return result;
}

/* Convert 'someKey' to 'some-key'
 *
 * This is the inverse function of qtify_name, iff qtify_name was called with a
 * valid gsettings key name (no capital letters, no consecutive dashes).
 *
 * Returns a newly-allocated string.
 */
static gchar * unqtify_name(const QString &name)
{
    const gchar *p;
    GString *str;

    str = g_string_new_len (NULL, name.size() + 5);

    for (p = name.toUtf8().constData(); *p; p++) {
        if (isupper(*p)) {
            g_string_append_c (str, '-');
            g_string_append_c (str, tolower(*p));
        }
        else {
            g_string_append_c (str, *p);
        }
    }

    return g_string_free(str, FALSE);
}

void GSettingsQml::updateKey(const gchar *gkey)
{
    QString qkey;
    GVariant *value;
    QVariant qvalue;

    qkey = qtify_name(gkey);
    value = g_settings_get_value(priv->settings, gkey);
    qvalue = qconf_types_to_qvariant(value);
    this->insert(qkey, qvalue);

    g_variant_unref(value);
}

void GSettingsQml::classBegin()
{
}

static void settings_key_changed(GSettings *, const gchar *key, gpointer user_data)
{
    GSettingsQml *self = (GSettingsQml *)user_data;

    self->updateKey(key);
}

void GSettingsQml::componentComplete()
{
    gchar **keys;
    gint i;

    if (priv->path.isEmpty())
        priv->settings = g_settings_new(priv->schema.constData());
    else
        priv->settings = g_settings_new_with_path(priv->schema.constData(), priv->path.constData());

    g_signal_connect(priv->settings, "changed", G_CALLBACK(settings_key_changed), this);

    keys = g_settings_list_keys(priv->settings);
    for (i = 0; keys[i]; i++)
        this->updateKey(keys[i]);
    g_strfreev(keys);
}

QVariant GSettingsQml::updateValue(const QString& key, const QVariant &value)
{
    GVariant *cur;
    GVariant *new_value;
    gchar *gkey;

    if (priv->settings == NULL)
        return value;

    gkey = unqtify_name(key);

    /* fetch current value to find out the exact type */
    cur = g_settings_get_value(priv->settings, gkey);

    new_value = qconf_types_collect_from_variant(g_variant_get_type (cur), value);
    if (new_value)
        g_settings_set_value(priv->settings, gkey, new_value);

    g_free(gkey);
    g_variant_unref (cur);
    return value;
}
