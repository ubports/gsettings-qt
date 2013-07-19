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
#include "util.h"

struct GSettingsSchemaQmlPrivate
{
    QByteArray id;
    QByteArray path;
};

struct GSettingsQmlPrivate
{
    GSettingsSchemaQml *schema;
    GSettings *settings;
};

GSettingsSchemaQml::GSettingsSchemaQml(QObject *parent): QObject(parent)
{
    priv = new GSettingsSchemaQmlPrivate;
}

GSettingsSchemaQml::~GSettingsSchemaQml()
{
    delete priv;
}

QByteArray GSettingsSchemaQml::id() const
{
    return priv->id;
}

void GSettingsSchemaQml::setId(const QByteArray &id)
{
    if (!priv->id.isEmpty()) {
        qWarning("GSettings.schema.id may only be set on construction");
        return;
    }

    priv->id = id;
}

QByteArray GSettingsSchemaQml::path() const
{
    return priv->path;
}

void GSettingsSchemaQml::setPath(const QByteArray &path)
{
    if (!priv->path.isEmpty()) {
        qWarning("GSettings.schema.path may only be set on construction");
        return;
    }

    priv->path = path;
}

QVariantList GSettingsSchemaQml::choices(const QByteArray &qkey) const
{
    GSettingsQml *parent = (GSettingsQml *) this->parent();
    gchar *key;
    GVariant *range;
    const gchar *type;
    GVariant *value;
    QVariantList choices;

    if (parent->priv->settings == NULL)
        return choices;

    if (!parent->contains(qkey))
        return choices;

    key = unqtify_name (qkey);
    range = g_settings_get_range (parent->priv->settings, key);
    g_free (key);

    if (range == NULL)
        return choices;

    g_variant_get (range, "(&sv)", &type, &value);

    if (g_str_equal (type, "enum")) {
        GVariantIter iter;
        GVariant *child;

        g_variant_iter_init (&iter, value);
        while ((child = g_variant_iter_next_value (&iter))) {
            choices.append(qconf_types_to_qvariant(child));
            g_variant_unref (child);
        }
    }

    g_variant_unref (value);
    g_variant_unref (range);

    return choices;
}

GSettingsQml::GSettingsQml(QObject *parent): QQmlPropertyMap(this, parent)
{
    priv = new GSettingsQmlPrivate;
    priv->schema = new GSettingsSchemaQml(this);
    priv->settings = NULL;
}

GSettingsQml::~GSettingsQml()
{
    if (priv->settings)
        g_object_unref (priv->settings);

    delete priv;
}

GSettingsSchemaQml * GSettingsQml::schema() const
{
    return priv->schema;
}

void GSettingsQml::updateKey(const gchar *gkey, bool emitChanged)
{
    QString qkey;
    GVariant *value;
    QVariant qvalue;

    qkey = qtify_name(gkey);
    value = g_settings_get_value(priv->settings, gkey);
    qvalue = qconf_types_to_qvariant(value);
    this->insert(qkey, qvalue);

    if (emitChanged)
        Q_EMIT (changed (qkey, qvalue));

    g_variant_unref(value);
}

void GSettingsQml::classBegin()
{
}

static void settings_key_changed(GSettings *, const gchar *key, gpointer user_data)
{
    GSettingsQml *self = (GSettingsQml *)user_data;

    self->updateKey(key, true);
}

void GSettingsQml::componentComplete()
{
    gchar **keys;
    gint i;

    if (priv->schema->path().isEmpty())
        priv->settings = g_settings_new(priv->schema->id().constData());
    else
        priv->settings = g_settings_new_with_path(priv->schema->id().constData(), priv->schema->path().constData());

    g_signal_connect(priv->settings, "changed", G_CALLBACK(settings_key_changed), this);

    keys = g_settings_list_keys(priv->settings);
    for (i = 0; keys[i]; i++)
        this->updateKey(keys[i], false);
    g_strfreev(keys);

    Q_EMIT(schemaChanged());
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
