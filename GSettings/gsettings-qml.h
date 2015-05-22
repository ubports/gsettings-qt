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

class GSettingsSchemaQml: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray id      READ id      WRITE setId)
    Q_PROPERTY(QByteArray path    READ path    WRITE setPath)
    Q_PROPERTY(bool       isValid READ isValid NOTIFY isValidChanged)

public:
    GSettingsSchemaQml(QObject *parent = NULL);
    ~GSettingsSchemaQml();

    QByteArray id() const;
    void setId(const QByteArray &id);

    QByteArray path() const;
    void setPath(const QByteArray &path);

    bool isValid() const;
    void setIsValid(bool valid);

    Q_INVOKABLE QVariantList choices(const QByteArray &key) const;
    Q_INVOKABLE void reset(const QByteArray &key);

Q_SIGNALS:
    void isValidChanged();

private:
    struct GSettingsSchemaQmlPrivate *priv;
};

class GSettingsQml: public QQmlPropertyMap, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(GSettingsSchemaQml* schema READ schema NOTIFY schemaChanged)

public:
    GSettingsQml(QObject *parent = NULL);
    ~GSettingsQml();

    GSettingsSchemaQml * schema() const;

    void classBegin();
    void componentComplete();

Q_SIGNALS:
    void schemaChanged();
    void changed (const QString &key, const QVariant &value);

private Q_SLOTS:
    void settingChanged(const QString &key);

private:
    struct GSettingsQmlPrivate *priv;

    QVariant updateValue(const QString& key, const QVariant &value);

    friend class GSettingsSchemaQml;
};
