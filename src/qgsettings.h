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

#ifndef __QGSETTINGS_H__
#define __QGSETTINGS_H__

#include <QObject>
#include <QStringList>

class QGSettings: public QObject
{
    Q_OBJECT

public:
    QGSettings(const QByteArray &schema_id, const QByteArray &path = QByteArray(), QObject *parent = NULL);
    ~QGSettings();

    QVariant get(const QString &key) const;
    void set(const QString &key, const QVariant &value);

    QStringList keys() const;
    QVariantList choices(const QString &key) const;

Q_SIGNALS:
    void changed(const QString &key);

private:
    struct QGSettingsPrivate *priv;
    friend struct QGSettingsPrivate;
};

#endif
