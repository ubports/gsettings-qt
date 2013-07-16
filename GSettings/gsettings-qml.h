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

#include <QtQml>
#include <QQmlParserStatus>

class GSettingsQml: public QQmlPropertyMap, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QByteArray schema READ schema WRITE setSchema)
    Q_PROPERTY(QByteArray path   READ path   WRITE setPath)

public:
    GSettingsQml(QObject *parent = NULL);
    ~GSettingsQml();

    QByteArray schema() const;
    void setSchema(const QByteArray &schema);

    QByteArray path() const;
    void setPath(const QByteArray &path);

    void updateKey(const char *gkey);

    void classBegin();
    void componentComplete();

private:
    struct GSettingsQmlPrivate *priv;

    QVariant updateValue(const QString& key, const QVariant &value);
};
